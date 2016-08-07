#ifndef SFOSUBMITWORDMODEL_H
#define SFOSUBMITWORDMODEL_H

#include "SFOValidatorReceiver.h"

#include <QAbstractListModel>
#include <QMap>
#include <QObject>

class QQmlContext;

class SFOValidator;

typedef QMap<QString, QString> SFOStringMap;

typedef QMap<QVariant, SFOValidator *> SFOValidatorMap;

class SFOSubmitWordModel : public QAbstractListModel,
                           public SFOValidatorReceiver
{
    Q_OBJECT
    Q_PROPERTY(QString word READ GetWord WRITE SetWord NOTIFY WordChanged)
    Q_PROPERTY(QString phonetic READ GetPhonetic WRITE SetPhonetic NOTIFY PhoneticChanged)
    Q_PROPERTY(bool submitEnabled READ GetSubmitEnabled NOTIFY SubmitEnabledChanged)
    Q_PROPERTY(QString submitEnabledText READ GetSubmitEnabledText NOTIFY SubmitEnabledTextChanged)
    
public:
    explicit SFOSubmitWordModel(QQmlContext *context, QObject *parent = 0);
    ~SFOSubmitWordModel();

    enum SubmitRole {
        IndexRole = 0,
        TranslationRole,
        AddVisibleRole,
        RemoveVisibleRole,
    };

    QString GetWord() const;
    void SetWord(const QString& word);
    QString GetPhonetic() const;
    void SetPhonetic(const QString& phonetic);

    bool GetSubmitEnabled() const;
    void SetSubmitEnabled(bool submitEnabled);

    QString GetSubmitEnabledText() const;

    void UpdateElement(const QString& element, const QVariant& identifier);

    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QHash<int, QByteArray> roleNames() const;

    // Overridden for SFOValidatorReceiver
    virtual void AddValidator(const QVariant& identifier,
                              SFOValidator *validator);

    // Global validator delegate for all validtors.
    virtual QValidator::State Validate( const QVariant& identifier,
                                        QString & input, int & pos );

signals:
    void WordChanged();
    void PhoneticChanged();
    void SubmitEnabledChanged();
    void SubmitEnabledTextChanged();

    // Alert on invalid input
    void InvalidInput();

public slots:
    void AddTranslation(const QString& translation);
    void RemoveTranslation(int row);
    void CommitModel();

protected:
    void _SyncFromValidators();

    void _ResetModel();

    void _ClearModel();

    static const QString InvalidInputForWord;
    static const QString InvalidInputPhonetic;
    static const QString InvalidInputTranslation;
    
private:
    QQmlContext *_context;

    // Meta model stuff for QML model types
    QHash<int, QByteArray> _roleNames;

    QString _word;
    QString _phonetic;
    QStringList _translations;
    bool _hasValidWord;
    bool _hasValidPhonetic;
    bool _hasValidTranslation;
    bool _submitEnabled;

    SFOValidatorMap _validators;
};

#endif // SFOSUBMITWORDMODEL_H
