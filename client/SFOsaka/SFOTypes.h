#ifndef SFOTYPES_H
#define SFOTYPES_H

#include <QList>
#include <QMap>
#include <QPair>
#include <QString>

typedef QPair<QString, QString> QStringPair;

typedef QMap<QString, QStringPair> QPairMap;
typedef QList<QStringPair> QPairList;

enum SFOInputLanguage {
    SFOJapaneseInput,
    SFOEnglishInput,
    SFOInvalidInput,
};

SFOInputLanguage SFOGetInputLanguage(const QString& input);

#endif // SFOTYPES_H
