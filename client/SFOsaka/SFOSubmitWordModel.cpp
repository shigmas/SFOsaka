#include "SFOSubmitWordModel.h"

#include "SFOContext.h"

#include <QQmlContext>
#include <QDebug>


SFOSubmitWordModel::SFOSubmitWordModel(QQmlContext *context, QObject *parent) :
    QAbstractListModel(parent),
    _context(context)
{
    _roleNames[1] = "translation";
    _roleNames[2] = "addVisible";
    _translations.append("translation");

}

SFOSubmitWordModel::~SFOSubmitWordModel()
{

}

QString
SFOSubmitWordModel::GetWord() const
{
    return _word;
}

void
SFOSubmitWordModel::SetWord(const QString& word)
{
    if (word != _word) {
        _word = word;
        emit WordChanged();
    }
}

void
SFOSubmitWordModel::AddTranslation(const QString& translation)
{
    qDebug() << "Adding " << translation;
    _translations.append(translation);    
    qDebug() << "Setting model property again";
    // Kludgy, but we clear it, then set it again so we pull the data again.
    _context->setContextProperty("submitModel", NULL);
    _context->setContextProperty("submitModel", this);
}

void
SFOSubmitWordModel::CommitModel()
{
    qDebug() << "Submitting " << _word << " with " << _translations;
    if (!_word.isEmpty() && (_translations.size() > 0)) {
        qDebug() << "Submitting data";
        SFOContext::GetInstance()->AddWordTranslation(_word, _translations);
    }
}

QVariant
SFOSubmitWordModel::data(const QModelIndex &index,
                      int role) const
{
    if (index.row() <= _translations.count()) {
        QString translation =
           index.row() == _translations.count()
                ? "" : _translations.at(index.row());
        switch(role) {
        case 1:
            return QVariant(translation);
        case 2:
            return QVariant(index.row() == _translations.count());
        default:
            qDebug() << "Unknown role " << role;
            return QVariant();
        }
    }
    return QVariant();
}

int
SFOSubmitWordModel::rowCount(const QModelIndex &) const
{
    return _translations.size() + 1;
}

QHash<int, QByteArray>
SFOSubmitWordModel::roleNames() const
{
    return _roleNames;
}
