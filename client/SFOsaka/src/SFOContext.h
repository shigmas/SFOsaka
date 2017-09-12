#ifndef SFOCONTEXT_H
#define SFOCONTEXT_H

#include <FJCaller.h>

#include "SFOPartner.h"
#include "SFOPerformer.h"
#include "SFOAppHighlight.h"
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

    SFOOrganizationList GetPartnersByCategory(const SFOPartnerCategory& category = SFOAllCategory) const;

    // We really only care about food and not food partners, so we provide
    // this helper function to filter a little more quickly
    SFOOrganizationList GetNonFoodPartners() const;

    SFOOrganizationList GetPerformers() const;

    SFOOrganizationList GetAppHighlights() const;

    void Refresh(bool immediately=false);

    void AddWordTranslation(const QString& word, const QString& phonetic,
                            const QVariantMap& translations);

    QPairMap GetEnToJpDict() const;
    QPairMap GetJpToEnDict() const;

    void LoadFromDisk();
    void FlushToDisk();

    bool IsNetworkAccessible() const;

signals:
    // This signal covers most organizations
    void PartnersUpdated();
    void PerformersUpdated();
    void DictionariesUpdated();
    void AppHighlightsUpdated();

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
    SFOOrganizationList _LoadOrganization(const QString& fileName) const {
        QJsonDocument doc = _LoadCacheFile(fileName);
        QVariantMap orgMap = _GetMapFromJson(doc);
        SFOOrganizationList orgList;
        QString key;
        foreach(key, orgMap.keys()) {
            orgList.append(new T(orgMap[key].toMap()));
        }

        return orgList;
    }

    template <typename T>
    void _WriteOrganization(const QString& fileName,
                            const SFOOrganizationList& orgList) const {
        if (orgList.size()) {
            QVariantMap orgMap;
            SFOOrganization* org;
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
    QPair<QDateTime, SFOOrganizationList> _ParseOrgResponse(const QVariantMap& data) const {
        QVariant orgData;
        QDateTime latest;
        SFOOrganizationList orgList;

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

    void _HandleStartResponse(const QJsonDocument& data);
    void _HandlePartnersResponse(const QJsonDocument& data);
    void _HandlePerformersResponse(const QJsonDocument& data);
    void _HandleAppHighlightsResponse(const QJsonDocument& data);
    void _HandleDictResponse(const QJsonDocument& data);
    void _HandleSubmitResponse(const QJsonDocument& data);

    QJsonDocument _CreateJsonContent(const QDateTime asOfDate) const;
    QJsonDocument _CreateJsonContent(const QVariantMap& dict) const;

protected:
    static const QString DateTimeStampFileName;
    static const QString PartnerCacheFileName;
    static const QString AppHighlightCacheFileName;
    static const QString DictionaryCacheFileName;
    static const QString PerformerCacheFileName;

    static const QString LastPartnerDateKey;
    static const QString LastDictDateKey;
    static const QString LastPerformerDateKey;
    static const QString LastAppHighlightDateKey;

    static const QStringPair ServerInfo;
private:
    static SFOContext* _instance;

    FJClientSharedPtr _client;

    SFOOrganizationList _partners;
    SFOOrganizationList _performers;
    SFOOrganizationList _appHighlights;
    QDateTime _lastPartnerDate;
    QDateTime _lastPerformerDate;
    QDateTime _lastAppHighlightDate;
    QDateTime _lastDictDate;
    QPairMap _enToJpDict;
    QPairMap _jpToEnDict;

    FJOperationList _pendingOperations;
};

#endif // SFOCONTEXT_H
