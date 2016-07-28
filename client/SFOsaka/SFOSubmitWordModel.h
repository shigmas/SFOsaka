#ifndef SFOSUBMITWORDMODEL_H
#define SFOSUBMITWORDMODEL_H

#include "SFOValidatorReceiver.h"

#include <QAbstractListModel>
#include <QMap>
#include <QObject>

class QQmlContext;

class SFOValidator;

typedef QMap<QString, QString> SFOStringMap;

typedef QPair<QVariant, QVariant> SFOValidatorId;
typedef QMap<SFOValidatorId, SFOValidator *> SFOValidatorMap;

class SFOSubmitWordModel : public QAbstractListModel,
                           public SFOValidatorReceiver
{
    Q_OBJECT
    Q_PROPERTY(QString word READ GetWord WRITE SetWord NOTIFY WordChanged)
    Q_PROPERTY(QString phonetic READ GetPhonetic WRITE SetPhonetic NOTIFY PhoneticChanged)
public:
    explicit SFOSubmitWordModel(QQmlContext *context, QObject *parent = 0);
    ~SFOSubmitWordModel();

    enum SubmitRole {
        IndexRole = 0,
        TranslationRole,
        PhoneticRole,
        AddVisibleRole,
        RemoveVisibleRole,
    };

    QString GetWord() const;
    void SetWord(const QString& word);
    QString GetPhonetic() const;
    void SetPhonetic(const QString& phonetic);

    void UpdateElement(const QString& element, const QVariant& identifier,
                       const QVariant& secondaryId = QVariant());

    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QHash<int, QByteArray> roleNames() const;

    // Overridden for SFOValidatorReceiver
    virtual void AddValidator(const QVariant& identifier,
                              SFOValidator *validator);


signals:
    void WordChanged();
    void PhoneticChanged();

public slots:
    void AddTranslation(const QString& translation,
                        const QString& phonetic);
    void RemoveTranslation(int row);
    void CommitModel();

protected:
    void _SyncFromValidators();


private:
    QQmlContext *_context;

    // Meta model stuff for QML model types
    QHash<int, QByteArray> _roleNames;

    QString _word;
    QString _phonetic;
    QStringList _translations;
    QStringList _phonetics;

    SFOValidatorMap _validators;
};

#endif // SFOSUBMITWORDMODEL_H
