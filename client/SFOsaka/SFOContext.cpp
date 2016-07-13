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

SFOContext::SFOContext(QObject *parent) : 
    FJCaller(parent), _partnersDirty(false), _dictDirty(false)
{
    //FJClient *client = new FJClient("localhost:8000","/mobapp/", "http");
    FJClient *client = new FJClient("malttest.futomen.net:8143", "/mobapp/");
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
    FJOperationSharedPtr operation(new FJOperation("start","","/start/",
                                                   this));
    operation->SetIsPost(false);
    _client->ClearCookies();
    _client->AddOperation(operation);
    if (immediately)
        _client->FlushQueue();
}

SFOPartnerList
SFOContext::GetPartners() const
{
    return _partners;
}

QStringMap
SFOContext::GetEnToJpDict() const
{
    return _enToJpDict;
}

QStringMap
SFOContext::GetJpToEnDict() const
{
    return _jpToEnDict;
}

void
SFOContext::AddWordTranslation(const QString& word,
                               const QStringList& translations)
{
    FJOperationSharedPtr operation(new FJOperation("submit","","/dict_submit/",
                                                   this));
    QVariantMap fetchData;
    fetchData["submit_word"] = word;
    fetchData["submit_trans"] = translations;
    operation->SetFetchDataContent(_CreateJsonContent(fetchData));
    _client->AddOperation(operation);
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
            qDebug() << "partner date: " << _lastPartnerDate;
        }
        dateString = dateMap[LastDictDateKey].toString();
        if (!dateString.isEmpty()) {
            _lastDictDate = QDateTime::fromString(dateString, Qt::ISODate);
            qDebug() << "dict date: " << _lastDictDate;
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
                _jpToEnDict[key] = allDicts[key].toString();
            } else {
                _enToJpDict[key] = allDicts[key].toString();
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
        obj.insert(LastPartnerDateKey,
                   _lastPartnerDate.toString(Qt::ISODate));
        obj.insert(LastDictDateKey,
                   _lastDictDate.toString(Qt::ISODate));
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
        _WriteCacheFile(doc, PartnerCacheFileName);
    }

    QJsonDocument doc;
    QVariantMap allDicts;
    QVariantMap vMap = _DictToVariantMap(_enToJpDict);
    allDicts.unite(vMap);
    vMap = _DictToVariantMap(_jpToEnDict);
    allDicts.unite(vMap);
    if (allDicts.size() > 0) {
        QJsonObject obj = QJsonObject::fromVariantMap(allDicts);
        QJsonDocument doc;
        doc.setObject(obj);
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
SFOContext::_DictToVariantMap(const QStringMap& dict) const
{
    QString key;
    QVariantMap vMap;
    foreach (key, dict.keys()) {
        vMap[key] = QVariant(dict[key]);
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
            _UpdatePartnersIfNecessary();
            _UpdateDictionaryIfNecessary();
        }
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
        qDebug() << "dict date: " << _lastDictDate;
        QVariant wVariant;
        QVariantMap wData;
        foreach(wVariant, wordMap.values()) {
            wData = wVariant.toMap();
            // Get the values that we care about
            QString word = wData["word"].toString();
            QString langCode = wData["language"].toString();
            QStringList translations = wData["translations"].toStringList();
            QString trans;
            foreach(trans, translations) {
                if (langCode == "jp") {
                    _enToJpDict[trans] = word;
                } else {
                    _jpToEnDict[trans] = word;
                }
            }
        }
        if (!_lastPartnerDate.isNull()) {
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
SFOContext::_UpdatePartnersIfNecessary()
{
    FJOperationSharedPtr operation(new FJOperation("partners","/partner_meta/",
                                                   "/partner_data/", this));
    QJsonDocument content = _CreateJsonContent(_lastPartnerDate);
    if (!content.isNull()) {
        operation->SetCheckFetchContent(content);
    }
    _client->AddOperation(operation);    
}

void
SFOContext::_UpdateDictionaryIfNecessary()
{
    FJOperationSharedPtr operation(new FJOperation("dict","/dict_meta/",
                                                   "/dict_data/", this));
    QJsonDocument content = _CreateJsonContent(_lastDictDate);
    if (!content.isNull()) {
        operation->SetCheckFetchContent(content);
    }
    _client->AddOperation(operation);
}

void
SFOContext::HandleResponse(const QJsonDocument& document, FJError /*error*/,
                           const FJOperation* operation)
{
   // qDebug() << QThread::currentThreadId() << "HandleResponse: "
    //         << operation->GetName() ;
    //             << ", (" << error << ") Doc: [" << document << "]";
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
