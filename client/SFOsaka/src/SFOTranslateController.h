#ifndef SFOTRANSLATECONTROLLER_H
#define SFOTRANSLATECONTROLLER_H

#include "SFOTypes.h"
#include "SFOValidatorReceiver.h"

#include <QMap>
#include <QObject>

#include <memory>

class QQmlContext;
class SFOTranslateModel;

typedef std::shared_ptr<SFOTranslateModel> SFOTranslateModelSharedPtr;

class SFOTranslateController : public QObject,
                               public SFOValidatorReceiver
{
    Q_OBJECT

    Q_PROPERTY(QString resultsText READ GetResultsText NOTIFY ResultsTextChanged)
    
public:
    static const QString ControllerIdentifier;
    static const QString ModelIdentifier;

    explicit SFOTranslateController(QQmlContext *context,
                                    QObject *parent = 0);

    virtual ~SFOTranslateController();

    // SFOValidatorReceiver implementations.
    virtual void AddValidator(const QVariant& identifier,
                              SFOValidator *validator);

    virtual QValidator::State Validate( const QVariant& identifier,
                                        QString & input, int & pos );

    QString GetResultsText() const;
    
signals:
    void ResultsTextChanged();

public slots:
    void OnInputAccepted(const QString& text);
    void OnFilterAccepted(const QString& text);

protected slots:
    void _HandleDictionariesUpdate();

protected:
    void _ProcessInput(const QString& text);

    // Returns the entries of \p dict that match the string \p str
    QPairMap _GetMatch(const QString& str, const QPairMap& dict) const;

    // Returns the entries of \p dict that match phonetically to \p str
    void _AppendPhoneticJpMatch(QPairMap& current, const QString& str,
                                const QPairMap& dict) const;

private:
    QQmlContext *_context;

    SFOTranslateModelSharedPtr _translationModel;

    SFOValidator* _validator;

    bool _noMatchesFound;
    QString _resultsText;
};

#endif // SFOTRANSLATECONTROLLER_H
