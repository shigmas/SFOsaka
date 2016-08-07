#include "SFOTypes.h"

#include <QObject>

Q_DECLARE_METATYPE(QStringPair)

SFOInputLanguage
SFOGetInputLanguage(const QString& input)
{
    if (input.length() == 0)
        return SFOInvalidInput;

    QChar inputChar = input.at(0);
    switch (inputChar.script()) {
    case QChar::Script_Latin:
        return SFOEnglishInput;
    case QChar::Script_Hiragana:
    case QChar::Script_Katakana:
    case QChar::Script_Han:
        return SFOJapaneseInput;
    default:
        return SFOInvalidInput;
    }

}
