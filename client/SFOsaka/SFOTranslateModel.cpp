#include "SFOTranslateModel.h"

// types!
#include "SFOContext.h"

#include <QDebug>

SFOTranslateModel::SFOTranslateModel(QObject *parent) :
    QAbstractListModel(parent),
    _numColumns(2)
{
    _roleNames[1] = "word";
    _roleNames[2] = "translation";
    _roleNames[3] = "pronunciation";
}

void
SFOTranslateModel::SetTranslations(const QPairMap& translations)
{
    _numColumns = 2;

    _words = translations.keys();
    QStringPair pair;
    foreach(pair, translations) {
        if (!pair.second.isEmpty()) {
            // If there are any phonetic pronunciations, add the second column
            _numColumns = 3;
            break;
        }
    }
    _translations = translations;
}

int
SFOTranslateModel::columnCount(const QModelIndex &) const
{
    return _numColumns;
}

QVariant
SFOTranslateModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < _words.size()) {
        QString word = _words.at(index.row());
        QStringPair trans = _translations[word];
        switch(role) {
        case 1:
            return word;
        case 2:
            return trans.first;
        case 3:
            return trans.second.isEmpty() ? "" : "(" + trans.second + ")";
        default:
            qDebug() << "Unknown role " << role;
            return QVariant();
        }
    }
    return QVariant();
}

int
SFOTranslateModel::rowCount(const QModelIndex &) const
{
    return _words.size();
}

QHash<int, QByteArray>
SFOTranslateModel::roleNames() const
{
    return _roleNames;
}
