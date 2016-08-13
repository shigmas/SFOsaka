#ifndef SFOTYPES_H
#define SFOTYPES_H

#include <QList>
#include <QMap>
#include <QPair>
#include <QString>

typedef QPair<QString, QString> QStringPair;

typedef QMap<QString, QStringPair> QPairMap;
typedef QList<QStringPair> QPairList;

enum SFOPartnerCategory {
    SFOAllCategory = 0,        // Database ID's start at 1, so we start at 0
    SFOFoodCategory,
    SFOLocationCategory,
    SFOCommunityCategory,
    SFOFinancialCategory,
    SFOEntertainmentCategory,
};

enum SFOInputLanguage {
    SFOJapaneseInput,
    SFOEnglishInput,
    SFOInvalidInput,
};

struct SFOTypes {
    static const QMap<SFOPartnerCategory, QString> PartnerMap;
    static SFOInputLanguage GetInputLanguage(const QString& input);
};

#endif // SFOTYPES_H
