#ifndef SFOTRANSLATECONTROLLER_H
#define SFOTRANSLATECONTROLLER_H

#include "SFOTypes.h"

#include <QMap>
#include <QObject>

#include <memory>

class QQmlContext;
class SFOTranslateModel;

typedef std::shared_ptr<SFOTranslateModel> SFOTranslateModelSharedPtr;

class SFOTranslateController : public QObject
{
    Q_OBJECT

    enum InputLanguage {
        JapaneseInput,
        EnglishInput,
        InvalidInput,
    };

public:
    static const QString ControllerIdentifier;
    static const QString ModelIdentifier;

    explicit SFOTranslateController(QQmlContext *context,
                                    QObject *parent = 0);

    virtual ~SFOTranslateController();

signals:

public slots:
    void OnInputAccepted(const QString& text);
    void OnFilterAccepted(const QString& text);

protected slots:
    void _HandleDictionariesUpdate();

protected:
    void _ProcessInput(const QString& text);

    InputLanguage _GetInputLanguage(const QString& input) const;

    // Returns the entries of \p dict that match the string \p str
    QPairMap _GetMatch(const QString& str, const QPairMap& dict) const;

private:
    QQmlContext *_context;

    SFOTranslateModelSharedPtr _translationModel;
};

#endif // SFOTRANSLATECONTROLLER_H
