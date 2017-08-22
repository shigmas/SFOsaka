#include "SFOTypes.h"

#include <QObject>


Q_DECLARE_METATYPE(QStringPair)

const QMap<SFOPartnerCategory, QString> SFOTypes::PartnerMap = {
    { SFOAllCategory,           "all"},
    { SFOFoodCategory,          "food"},
    { SFOLocationCategory,      "location"},
    { SFOCommunityCategory,     "community"},
    { SFOFinancialCategory,     "financial"},
    { SFOEntertainmentCategory, "entertainment"},
};

SFOInputLanguage
SFOTypes::GetInputLanguage(const QString& input)
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
