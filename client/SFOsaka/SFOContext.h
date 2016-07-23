#ifndef SFOCONTEXT_H
#define SFOCONTEXT_H

#include <FJCaller.h>

#include "SFOPartner.h"
#include "SFOTypes.h"

#include <FJOperation.h>
#include <FJTypes.h>

#include <QDate>
#include <QJsonDocument>
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

    void Refresh(bool immediately=false);

    void AddWordTranslation(const QString& word, const QString& phonetic,
                            const QVariantMap& translations);

    QPairMap GetEnToJpDict() const;
    QPairMap GetJpToEnDict() const;

    void LoadFromDisk();
    void FlushToDisk();

signals:
    void PartnersUpdated();
    void DictionariesUpdated();

public slots:
    virtual void HandleResponse(const QJsonDocument& document, FJError error,
                                const FJOperation* operation);

protected slots:
    void _OnQueueCompleted();

protected:
    explicit SFOContext(QObject *parent = 0);

    void _WriteCacheFile(const QJsonDocument& doc, const QString& filename);
    QJsonDocument _LoadCacheFile(const QString& filename);

    QVariantMap _PairDictToVariantMap(const QPairMap& dict) const;
    QVariantMap _GetMapFromJson(const QJsonDocument& doc) const;

    QString _GetFilePath(const QString& filename, bool createDir=false) const;

    // If the variant is just a string, creates the pair with the second
    // empty. Otherwise, returns a genuine pair.
    QStringPair _CreatePairFromVariant(const QVariant& variant) const;

    bool _IsJapanese(const QString& word) const;

    void _HandleStartResponse(const QJsonDocument& data);
    void _HandlePartnersResponse(const QJsonDocument& data);
    void _HandleDictResponse(const QJsonDocument& data);
    void _HandleSubmitResponse(const QJsonDocument& data);

    QJsonDocument _CreateJsonContent(const QDateTime asOfDate) const;
    QJsonDocument _CreateJsonContent(const QVariantMap& dict) const;

protected:
    static const QString DateTimeStampFileName;
    static const QString PartnerCacheFileName;
    static const QString DictionaryCacheFileName;

    static const QString LastPartnerDateKey;
    static const QString LastDictDateKey;

    static const QStringPair ServerInfo;
private:
    static SFOContext* _instance;

    FJClientSharedPtr _client;

    QDateTime _lastPartnerDate;
    SFOPartnerList _partners;
    bool _partnersDirty;
    QDateTime _lastDictDate;
    QPairMap _enToJpDict;
    QPairMap _jpToEnDict;
    bool _dictDirty;

    FJOperationList _pendingOperations;
};

#endif // SFOCONTEXT_H
