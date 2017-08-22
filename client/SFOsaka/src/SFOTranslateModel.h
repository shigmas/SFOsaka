#ifndef SFOTRANSLATEMODEL_H
#define SFOTRANSLATEMODEL_H

#include <FJTypes.h>

#include <QAbstractListModel>
#include <QList>
#include <QString>

// types
#include "SFOTypes.h"


class SFOTranslateModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int maxCharacters READ GetMaxCharacters NOTIFY MaxCharactersChanged)

public:
    static const QString TranslationIdentifier;
    static const QString PronunciationIdentifier;

    explicit SFOTranslateModel(QObject *parent = 0);

    void SetTranslations(const QPairMap& translations);

    // The longest entry count of characters
    int GetMaxCharacters() const;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void MaxCharactersChanged();

private:
    QStringList _words;
    QPairMap _translations;
    QHash<int, QByteArray> _roleNames;
    int _maxCharacters;
    int _numColumns;
};


#endif // SFOTRANSLATEMODEL_H
