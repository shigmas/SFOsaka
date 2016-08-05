#include "SFOSubmitWordModel.h"

#include "SFOContext.h"
#include "SFOValidator.h"

#include <QJSValue>
#include <QQmlContext>
#include <QDebug>


SFOSubmitWordModel::SFOSubmitWordModel(QQmlContext *context, QObject *parent) :
    QAbstractListModel(parent),
    _context(context)
{
    _roleNames[IndexRole] = "index";
    _roleNames[TranslationRole] = "translation";
    _roleNames[PhoneticRole] = "phonetic";
    _roleNames[AddVisibleRole] = "addVisible";
    _roleNames[RemoveVisibleRole] = "removeVisible";
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

QString
SFOSubmitWordModel::GetPhonetic() const
{
    return _phonetic;
}

void
SFOSubmitWordModel::SetPhonetic(const QString& phonetic)
{
    if (phonetic != _phonetic) {
        _phonetic = phonetic;
        emit PhoneticChanged();
    }
}

SFOValidatorId _ConvertValueToId(const QJSValue& identifier)
{
    if (identifier.isArray()) {
        QList<QVariant> vList = identifier.toVariant().toList();
        if (vList.size() == 2) {
            return qMakePair(vList[0],vList[1]);
        }
    } else {
        qDebug() << "Error: identifier cannot be converted to pair type";
    }
    return qMakePair(QVariant(), QVariant());
}

void
SFOSubmitWordModel::AddValidator(const QVariant& identifier,
                                 SFOValidator *validator)
{
    SFOValidatorId valId = identifier.canConvert<QJSValue>()
        ? _ConvertValueToId(identifier.value<QJSValue>())
        : qMakePair(identifier, QVariant());
    _validators[valId] = validator;
}


void
SFOSubmitWordModel::UpdateElement(const QString& element,
                                  const QVariant& identifier,
                                  const QVariant& secondaryId)
{
    if (identifier.toString() == QString("word")) {
        _word = element;
    } else if (identifier.toString() == QString("phonetic")) {
        _phonetic = element;
    } else if (identifier.canConvert<int>()) {
        int index = identifier.toInt();
        // Is it a translation or phonetic?
        QString secondId = secondaryId.toString();
        if (secondId == QString("translation")) {
            if (index < _translations.size()) {
                _translations.replace(index, element);
            } else {
                _translations.append(element);
            }
        } else if (secondId == QString("phonetic")) {
            if (index < _phonetics.size()) {
                _phonetics.replace(index, element);
            } else {
                _phonetics.append(element);
            }
        }
    }
}

void
SFOSubmitWordModel::AddTranslation(const QString& translation,
                                   const QString& phonetic)
{
    qDebug() << "Adding " << translation;
    _translations.append(translation);
    _phonetics.append(phonetic);
    _SyncFromValidators();
    qDebug() << "Setting model property again";
    // Kludgy, but we clear it, then set it again so we pull the data again.
    _context->setContextProperty("submitModel", NULL);
    _context->setContextProperty("submitModel", this);
}

void
SFOSubmitWordModel::RemoveTranslation(int row)
{
    qDebug() << "removing " << row;
    _translations.removeAt(row);
    _phonetics.removeAt(row);
    qDebug() << "Setting model property again";
    // Kludgy, but we clear it, then set it again so we pull the data again.
    _context->setContextProperty("submitModel", NULL);
    _context->setContextProperty("submitModel", this);
}

void
SFOSubmitWordModel::CommitModel()
{
    _SyncFromValidators();

    qDebug() << "Submitting " << _word << " (" << _phonetic << ") with "
             << _translations.size() << " translations. "
             << _phonetics.size() << " phonetics";
    
    QVariantMap transPhonetic;
    for (int i = 0 ; i < _translations.size() ; ++i) {
        if (i < _phonetics.size()) {
            transPhonetic[_translations[i]] = _phonetics[i];
        }
    }

    if (!_word.isEmpty() && (transPhonetic.size() > 0)) {
        qDebug() << "Submitting data";
        // XXX build up the translations/phonetics
        SFOContext::GetInstance()->AddWordTranslation(_word, _phonetic,
                                                      transPhonetic);
    }
    _ClearModel();
}

QVariant
SFOSubmitWordModel::data(const QModelIndex &index,
                      int role) const
{
    if (index.row() <= _translations.count()) {
        QString translation =
           index.row() == _translations.count()
                ? "" : _translations.at(index.row());
        QString phonetic =
            index.row() == _phonetics.count()
                ? "" : _phonetics.at(index.row());
        switch(role) {
        case IndexRole:
            return QVariant(index.row());
        case TranslationRole:
            return QVariant(translation);
        case PhoneticRole:
            return QVariant(phonetic);
        case AddVisibleRole:
            return QVariant(index.row() == _translations.count());
        case RemoveVisibleRole:
            return QVariant(index.row() != _translations.count());
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

void
SFOSubmitWordModel::_SyncFromValidators()
{
    // Iterator through the validators and update the model
    for (SFOValidatorMap::key_iterator it = _validators.keyBegin() ;
         it != _validators.keyEnd() ; ++it) {
        SFOValidator *validator = _validators[*it];
        UpdateElement(validator->GetValidated(), it->first, it->second);
    }
}

void
SFOSubmitWordModel::_ClearModel()
{
    _word = "";
    _phonetic = "";
    _translations.clear();
    _phonetics.clear();
    _context->setContextProperty("submitModel", NULL);
    _context->setContextProperty("submitModel", this);
}
