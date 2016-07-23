#include "SFOContext.h"

#include <FJClient.h>
#include <FJOperation.h>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QThread>

// Declare the pointers for us, so we can pass ourselves as a weakptr
FJ_DECLARE_PTRS(SFOContext)

SFOContext* SFOContext::_instance = NULL;

const QString SFOContext::DateTimeStampFileName   = "datestamps.json";
const QString SFOContext::PartnerCacheFileName    = "partners.json";
const QString SFOContext::DictionaryCacheFileName = "dictionary.json";

const QString SFOContext::LastPartnerDateKey = "last_partner_date";
const QString SFOContext::LastDictDateKey    = "last_dict_date";

//const QStringPair SFOContext::ServerInfo = qMakePair(QString("localhost:8000"),QString("http"));
const QStringPair SFOContext::ServerInfo = qMakePair(QString("malttest.futomen.net:8143"),QString("https"));

SFOContext::SFOContext(QObject *parent) : 
    FJCaller(parent), _partnersDirty(false), _dictDirty(false)
{
    FJClient *client = new FJClient(ServerInfo.first, "/mobapp/",
                                    ServerInfo.second);
    _client = FJClientSharedPtr(client);

    // Listen on the queue completed signal and queue our refresh.
    QObject::connect(client, &FJClient::QueueCompleted,
                     this, &SFOContext::_OnQueueCompleted);

    qDebug() << "Data location: "
             << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    LoadFromDisk();
    Refresh(true);
    // Run the refresh right away when we're first starting up
}

SFOContext::~SFOContext()
{
    FlushToDisk();
}

QString
SFOContext::GetHost() const
{
    return ServerInfo.second;
}

SFOContext*
SFOContext::GetInstance()
{
    if (!_instance) {
        _instance = new SFOContext();
    }

    return _instance;
}

void
SFOContext::Refresh(bool immediately)
{
    /*
    qDebug() << "Refreshing...";
    qDebug() << "Current partners: " << _partners.size();
    qDebug() << "jp dict: " << _jpToEnDict.size();
    qDebug() << "en dict: " << _enToJpDict.size();
    */

    // Kick off start request
    FJOperationSharedPtr start(new FJOperation("start","","/start/", this));
    start->SetIsPost(false);
    _client->ClearCookies();
    _client->AddOperation(start);

    // Create the update operations to be queued after start finishes
    FJOperationSharedPtr partner(new FJOperation("partners","/partner_meta/",
                                                 "/partner_data/", this));
    QJsonDocument content = _CreateJsonContent(_lastPartnerDate);
    if (!content.isNull()) {
        partner->SetCheckFetchContent(content);
    }
    _pendingOperations.append(partner);
    FJOperationSharedPtr dict(new FJOperation("dict","/dict_meta/","/dict_data/",
                                              this));
    content = _CreateJsonContent(_lastDictDate);
    if (!content.isNull()) {
        dict->SetCheckFetchContent(content);
    }
    _pendingOperations.append(dict);

    if (immediately) {
        _client->FlushQueue();
    }
}

void
SFOContext::AddWordTranslation(const QString& word, const QString& phonetic,
                               const QVariantMap& translations)
{
    FJOperationSharedPtr start(new FJOperation("start","","/start/", this));
    start->SetIsPost(false);
    _client->ClearCookies();
    _client->AddOperation(start);

    FJOperationSharedPtr submit(new FJOperation("submit","","/dict_submit/",
                                                this));
    QVariantMap fetchData;
    fetchData["submit_word"] = word;
    fetchData["submit_word_phonetic"] = phonetic;
    fetchData["submit_trans"] = translations;
    submit->SetFetchDataContent(_CreateJsonContent(fetchData));
    _pendingOperations.append(submit);

    // When we submit data, we should do it right away.
    _client->FlushQueue();
}

SFOPartnerList
SFOContext::GetPartners() const
{
    return _partners;
}

QPairMap
SFOContext::GetEnToJpDict() const
{
    return _enToJpDict;
}

QPairMap
SFOContext::GetJpToEnDict() const
{
    return _jpToEnDict;
}

void
SFOContext::LoadFromDisk()
{
    QJsonDocument doc;

    doc = _LoadCacheFile(DateTimeStampFileName);
    QVariantMap dateMap = _GetMapFromJson(doc);
    if (!dateMap.empty()) {
        QString dateString = dateMap[LastPartnerDateKey].toString();
        if (!dateString.isEmpty()) {
            _lastPartnerDate = QDateTime::fromString(dateString, Qt::ISODate);
        }
        dateString = dateMap[LastDictDateKey].toString();
        if (!dateString.isEmpty()) {
            _lastDictDate = QDateTime::fromString(dateString, Qt::ISODate);
        }
    }

    doc = _LoadCacheFile(PartnerCacheFileName);
    //qDebug() << "Cached partner data: " << doc;
    QVariantMap partnerMap = _GetMapFromJson(doc);
    if (!partnerMap.empty()) {
        SFOPartner *p;
        foreach (p, _partners) {
            delete p;
        }
        _partners.clear();
        QString key;
        foreach(key, partnerMap.keys()) {
            _partners.append(new SFOPartner(partnerMap[key].toMap()));
        }
    } else {
        qDebug() << "empty partner cache file";
    }

    qDebug() << "Dict cache: " << DictionaryCacheFileName;
    doc = _LoadCacheFile(DictionaryCacheFileName);
    QVariantMap allDicts = _GetMapFromJson(doc);
    if (!allDicts.empty()) {
        _enToJpDict.clear();
        _jpToEnDict.clear();
        QString key;
        foreach(key, allDicts.keys()) {
            //qDebug() << "Word: " << key;
            // Look at the first character to see if it's japanese
            if (_IsJapanese(key)) {
                // means val is english, and we can ignore the pronunciation
                // (if it even has one)
                _jpToEnDict[key] = _CreatePairFromVariant(allDicts[key]);
            } else {
                _enToJpDict[key] = _CreatePairFromVariant(allDicts[key]);
            }
        }
    } else {
        qDebug() << "empty dict cache file";
    }
}

void
SFOContext::FlushToDisk()
{
    // Create the JSON document for the date meta data
    if (!_lastPartnerDate.isNull() or !_lastDictDate.isNull()) {
        QJsonObject obj;
        // Add a second to both dates, because sub-second times are not in
        obj.insert(LastPartnerDateKey,
                   _lastPartnerDate.addSecs(1).toString(Qt::ISODate));
        obj.insert(LastDictDateKey,
                   _lastDictDate.addSecs(1).toString(Qt::ISODate));
        QJsonDocument doc;
        doc.setObject(obj);
        _WriteCacheFile(doc, DateTimeStampFileName);
    }

    if (_partners.size()) {
        QVariantMap partnerMap;
        SFOPartner* partner;
        foreach (partner, _partners) {
            partnerMap[partner->GetId()] = partner->ToJson();
        }
        QJsonObject obj = QJsonObject::fromVariantMap(partnerMap);
        QJsonDocument doc;
        doc.setObject(obj);
        qDebug() << "Saving " << _partners.size() << " partners";
        _WriteCacheFile(doc, PartnerCacheFileName);
    }

    QJsonDocument doc;
    QVariantMap allDicts;
    QVariantMap vMap = _PairDictToVariantMap(_enToJpDict);
    allDicts.unite(vMap);
    vMap = _PairDictToVariantMap(_jpToEnDict);
    allDicts.unite(vMap);
    if (allDicts.size() > 0) {
        QJsonObject obj = QJsonObject::fromVariantMap(allDicts);
        QJsonDocument doc;
        doc.setObject(obj);
        qDebug() << "Saving " << allDicts.size() << " words";
        _WriteCacheFile(doc, DictionaryCacheFileName);
    }
}

void
SFOContext::_OnQueueCompleted()
{
    Refresh();
}

void
SFOContext::_WriteCacheFile(const QJsonDocument& doc, const QString& filename)
{
    QByteArray data = doc.toJson();

    QFile file(_GetFilePath(filename, true));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Unable to open " << file.fileName();
    }
    //qDebug() << "Writing " << file.fileName();
    file.write(data);
    file.close();
}

QJsonDocument
SFOContext::_LoadCacheFile(const QString& filename)
{
    QFile file(_GetFilePath(filename));
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open " << _GetFilePath(filename);
        return QJsonDocument();
    }
    //    qDebug() << "Reading " << file.fileName();
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc;
}

QVariantMap
SFOContext::_PairDictToVariantMap(const QPairMap& dict) const
{
    QString key;
    QVariantMap vMap;
    foreach (key, dict.keys()) {
        vMap[key] = QVariant(QStringList({dict[key].first,dict[key].second}));
    }

    return vMap;
}

QVariantMap
SFOContext::_GetMapFromJson(const QJsonDocument& doc) const
{
    if (!doc.isNull()) {
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            return obj.toVariantMap();
        } else {
            qDebug() << "SFOContext::_GetMapFromJson: not a variantmap";
        }
    }
    return QVariantMap();
}

QString
SFOContext::_GetFilePath(const QString& filename, bool createDir) const
{
    QString path =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir pathDir(path);
    if (!pathDir.exists() and createDir) {
        pathDir.mkpath(path);
    }
    return path + "/" + filename;
}

QStringPair
SFOContext::_CreatePairFromVariant(const QVariant& variant) const
{
    // Old versions, this value is a string. Newer ones have this as
    // a pair.
    QStringPair dictPair;
    QString val;
    QString phonetic;

    if (variant.canConvert<QString>()) {
        return qMakePair(variant.toString(), QString());
    } else if (variant.canConvert<QStringList>()) {
        QStringList strList = variant.value<QStringList>();
        return qMakePair(strList[0],strList[1]);
    } else {
        qDebug() << "Couldn't create pair from variant. type: "
                 << variant.typeName() << "(" << variant.type() << ")";
        return qMakePair(QString(), QString());
    }
}
        
bool
SFOContext::_IsJapanese(const QString& word) const
{
    if (word.length() == 0)
        return false;

    QChar inputChar = word.at(0);
    switch (inputChar.script()) {
    case QChar::Script_Latin:
        return false;
    case QChar::Script_Hiragana:
    case QChar::Script_Katakana:
    case QChar::Script_Han:
        return true;
    default:
        return false;
    }

}

void
SFOContext::_HandleStartResponse(const QJsonDocument& data)
{
    QVariantMap results = _GetMapFromJson(data);
    if (results.isEmpty()) {
        qDebug() << "start response is empty!";
        return;
    }
    if (results["result"].canConvert<bool>()) {
        bool success = results["result"].toBool();
        if (success) {
            FJOperationSharedPtr op;
            foreach(op, _pendingOperations) {
                _client->AddOperation(op);
            }
            _pendingOperations.clear();
        }
        _client->FlushQueue();
    } else {
        qDebug() << "result is not a bool.Start request failed";
    }
}

void
SFOContext::_HandlePartnersResponse(const QJsonDocument& data)
{
    QVariantMap results = _GetMapFromJson(data);
    if (results.isEmpty()) {
        qDebug() << "partner response is empty!";
        return;
    }
    bool success = false;
    if (results["result"].canConvert<bool>()) {
        success = results["result"].toBool();
    }
    if (success) {
        //qDebug() << "Partners Data: " << results;
        QVariantMap partnerMap = results["partners_list"].toMap();
        QVariant pData;
        QDateTime latest;
        SFOPartner* p;
        foreach (p, _partners) {
            delete p;
        }
        _partners.clear();
        foreach(pData, partnerMap.values()) {
            p = new SFOPartner(pData.toMap());
            //            qDebug() << "Created " << p.GetName() << ", ("
            //                     << p.GetDescription() << " created on: "
            //                     << p.GetModificationDate();
            _partners.append(p);
            if (latest.isNull()) {
                latest = p->GetModificationDate();
            } else if (latest < p->GetModificationDate()) {
                latest = p->GetModificationDate();
            }
        }
        if (!latest.isNull()) {
            _lastPartnerDate = latest;
            FlushToDisk();
            emit PartnersUpdated();
        } else {
            qDebug() << "No partners in partner data";
        }
    } else {
        qDebug() << "Partner fetch failed";
    }
}

void
SFOContext::_HandleDictResponse(const QJsonDocument& data)
{
    QVariantMap results = _GetMapFromJson(data);
    bool success = results["result"].toBool();
    if (success) {
        QVariantMap wordMap = results["words_list"].toMap();
        QString mDate = results["as_of_date"].toString();
        _lastDictDate = QDateTime::fromString(mDate, Qt::ISODate);
        QVariant wVariant;
        QVariantMap wData;
        foreach(wVariant, wordMap.values()) {
            wData = wVariant.toMap();
            // Get the values that we care about
            QString word = wData["word"].toString();
            QString langCode = wData["language"].toString();
            QString phonetic = wData["phonetic"].toString();
            QStringList translations = wData["translations"].toStringList();
            QString trans;
            foreach(trans, translations) {
                if (langCode == "jp") {
                    _enToJpDict[trans] = qMakePair(word,phonetic);
                } else {
                    _jpToEnDict[trans] = qMakePair(word,phonetic);
                }
            }
        }
        if (!_lastDictDate.isNull()) {
            FlushToDisk();
        }
        emit DictionariesUpdated();
    } else {
        qDebug() << "words fetch failed";
    }
}

void
SFOContext::_HandleSubmitResponse(const QJsonDocument& data)
{
    QVariantMap results = _GetMapFromJson(data);
    bool success = results["result"].toBool();
    if (!success) {
        qDebug() << "Submit new word failed";
    }
}

QJsonDocument
SFOContext::_CreateJsonContent(const QDateTime asOfDate) const
{
    QJsonDocument doc;
    if (!asOfDate.isNull()) {
        QJsonObject obj;
        obj.insert("as_of_date", asOfDate.toString(Qt::ISODate));
        doc.setObject(obj);
    }

    return doc;
}

QJsonDocument
SFOContext::_CreateJsonContent(const QVariantMap& dict) const
{
    QJsonDocument doc;
    QJsonObject obj = QJsonObject::fromVariantMap(dict);
    doc.setObject(obj);
    return doc;
}

void
SFOContext::HandleResponse(const QJsonDocument& document, FJError error,
                           const FJOperation* operation)
{
    if (error) {
        qDebug() << QThread::currentThreadId() << "HandleResponse: "
                 << operation->GetName()
                 << ", (" << error << ") Doc: [" << document << "]";
    }
    if (operation && (operation->GetName() == "start")) {
        _HandleStartResponse(document);
    } else if (operation && (operation->GetName() == "partners")) {
        _HandlePartnersResponse(document);
    } else if (operation && (operation->GetName() == "dict")) {
        _HandleDictResponse(document);
    } else if (operation && (operation->GetName() == "submit")) {
        _HandleSubmitResponse(document);
    }
}
