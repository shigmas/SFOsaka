#ifndef SFOTRANSLATECONTROLLER_H
#define SFOTRANSLATECONTROLLER_H

#include <QMap>
#include <QObject>

#include <memory>

class QQmlContext;
class QStringListModel;

typedef QMap<QString, QString> QStringMap;
typedef std::shared_ptr<QStringListModel> QStringListModelSharedPtr;
typedef std::shared_ptr<QStringList> QStringListSharedPtr;

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

protected slots:
    void _HandleDictionariesUpdate();

protected:
    InputLanguage _GetInputLanguage(const QString& input) const;

    QStringList _GetMatch(const QString& str, const QStringMap& dict) const;

private:
    QQmlContext *_context;

    QStringListSharedPtr _translatedText;
    QStringListModelSharedPtr _translationModel;

};

#endif // SFOTRANSLATECONTROLLER_H
