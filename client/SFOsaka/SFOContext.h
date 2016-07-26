#ifndef SFOCONTEXT_H
#define SFOCONTEXT_H

#include <FJCaller.h>

#include "SFOPartner.h"
#include "SFOPerformer.h"
#include "SFOTypes.h"

#include <FJOperation.h>
#include <FJTypes.h>

#include <QDate>
#include <QJsonDocument>
#include <QPair>
#include <QString>

FJ_DECLARE_PTRS(FJClient)
FJ_DECLARE_PTRS(FJOperation)

typedef QList<FJOperationSharedPtr> FJOperationList;

class SFOContext : public FJCaller
{
Q_OBJECT

public:
    static SFOContext* GetInstance();

    virtual ~SFOContext();

    QString GetHost() const;

    SFOPartnerList GetPartners() const;

    SFOPerformerList GetPerformers() const;

    void Refresh(bool immediately=false);

    void AddWordTranslation(const QString& word, const QString& phonetic,
                            const QVariantMap& translations);

    QPairMap GetEnToJpDict() const;
    QPairMap GetJpToEnDict() const;

    void LoadFromDisk();
    void FlushToDisk();

signals:
    void PartnersUpdated();
    void PerformersUpdated();
    void DictionariesUpdated();

public slots:
    virtual void HandleResponse(const QJsonDocument& document, FJError error,
                                const FJOperation* operation);

protected slots:
    void _OnQueueCompleted();

protected:
    explicit SFOContext(QObject *parent = 0);

    void _WriteCacheFile(const QJsonDocument& doc, const QString& filename)const;
    QJsonDocument _LoadCacheFile(const QString& filename) const;

    template <typename T>
    void _eraseList(QList<T *>& orgList) {
        T *org;
        foreach (org, orgList) {
            delete org;
        }
        orgList.clear();
    }

    template <typename T>
    QList<T *> _LoadOrganization(const QString& fileName) const {
        QJsonDocument doc = _LoadCacheFile(fileName);
        QVariantMap orgMap = _GetMapFromJson(doc);
        QList<T *> orgList;
        QString key;
        foreach(key, orgMap.keys()) {
            orgList.append(new T(orgMap[key].toMap()));
        }

        return orgList;
    }

    template <typename T>
    void _WriteOrganization(const QString& fileName,
                            const QList<T *>& orgList) const {
        if (orgList.size()) {
            QVariantMap orgMap;
            T* org;
            foreach (org, orgList) {
                orgMap[org->GetId()] = org->ToJson();
            }
            QJsonObject obj = QJsonObject::fromVariantMap(orgMap);
            QJsonDocument doc;
            doc.setObject(obj);
            _WriteCacheFile(doc, fileName);
        }
    }

    template <typename T>
    QPair<QDateTime, QList< T *> > _ParseOrgResponse(const QVariantMap& data) const {
        QVariant orgData;
        QDateTime latest;
        QList< T *> orgList;

        foreach(orgData, data.values()) {
            T *o = new T(orgData.toMap());
            orgList.append(o);
            if (latest.isNull()) {
                latest = o->GetModificationDate();
            } else if (latest < o->GetModificationDate()) {
                latest = o->GetModificationDate();
            }
        }

        return qMakePair(latest, orgList);
    }

    QVariantMap _PairDictToVariantMap(const QPairMap& dict) const;
    QVariantMap _GetMapFromJson(const QJsonDocument& doc) const;

    QString _GetFilePath(const QString& filename, bool createDir=false) const;

    // If the variant is just a string, creates the pair with the second
    // empty. Otherwise, returns a genuine pair.
    QStringPair _CreatePairFromVariant(const QVariant& variant) const;

    bool _IsJapanese(const QString& word) const;

    void _HandleStartResponse(const QJsonDocument& data);
    void _HandlePartnersResponse(const QJsonDocument& data);
    void _HandlePerformersResponse(const QJsonDocument& data);
    void _HandleDictResponse(const QJsonDocument& data);
    void _HandleSubmitResponse(const QJsonDocument& data);

    QJsonDocument _CreateJsonContent(const QDateTime asOfDate) const;
    QJsonDocument _CreateJsonContent(const QVariantMap& dict) const;

protected:
    static const QString DateTimeStampFileName;
    static const QString PartnerCacheFileName;
    static const QString DictionaryCacheFileName;
    static const QString PerformerCacheFileName;

    static const QString LastPartnerDateKey;
    static const QString LastDictDateKey;
    static const QString LastPerformerDateKey;

    static const QStringPair ServerInfo;
private:
    static SFOContext* _instance;

    FJClientSharedPtr _client;

    QDateTime _lastPartnerDate;
    SFOPartnerList _partners;
    SFOPerformerList _performers;
    QDateTime _lastPerformerDate;
    QDateTime _lastDictDate;
    QPairMap _enToJpDict;
    QPairMap _jpToEnDict;

    FJOperationList _pendingOperations;
};

#endif // SFOCONTEXT_H
