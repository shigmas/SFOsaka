#include "SFOSubmitWordModel.h"

#include "SFOContext.h"
#include "SFOValidator.h"

#include <QJSValue>
#include <QQmlContext>
#include <QDebug>


SFOSubmitWordModel::SFOSubmitWordModel(QQmlContext *context, QObject *parent) :
    QAbstractListModel(parent),
    _context(context),
    _hasValidWord(false),
    _hasValidPhonetic(false),
    _hasValidTranslation(false),
    _submitEnabled(false)
{
    _roleNames[IndexRole] = "index";
    _roleNames[TranslationRole] = "translation";
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

bool
SFOSubmitWordModel::GetSubmitEnabled() const
{
    return _submitEnabled;
}

void
SFOSubmitWordModel::SetSubmitEnabled(bool submitEnabled)
{
    qDebug() << "submit enabled: " << _submitEnabled;
    if (_submitEnabled != submitEnabled) {
        _submitEnabled = submitEnabled;
        emit SubmitEnabledChanged();
        emit SubmitEnabledTextChanged();
    }
}

QString
SFOSubmitWordModel::GetSubmitEnabledText() const
{
    if (!SFOContext::GetInstance()->IsNetworkAccessible()) {
        return tr("Submit disabled until network is accessible");
    } else {
        if (!_submitEnabled) {
            return tr("One or more fields are invalid");
        }
    }
    return "";
}

void
SFOSubmitWordModel::AddValidator(const QVariant& identifier,
                                 SFOValidator *validator)
{
    _validators[identifier] = validator;
}


QValidator::State
SFOSubmitWordModel::Validate( const QVariant& identifier, QString &input ,
                              int &  )
{
    if (identifier.toString() == QString("word")) {
        _hasValidWord = SFOTypes::GetInputLanguage(input) == SFOJapaneseInput;
    } else if (identifier.toString() == QString("phonetic")) {
        _hasValidPhonetic = SFOTypes::GetInputLanguage(input) == SFOEnglishInput;
    } else {
        // This is a translation, so we need to see if any validator has
        // valid input. Fetching the pair from the identifier is tricky
        // anyway.
        _hasValidTranslation = true;
        for (SFOValidatorMap::key_iterator it = _validators.keyBegin() ;
             it != _validators.keyEnd() ; ++it) {
            if ((it->type() == QVariant::Int) and
                (SFOTypes::GetInputLanguage(_validators[*it]->GetValidated()) !=
                 SFOEnglishInput)) {
                _hasValidTranslation = false;
                break;
            }
        }
    }
    SetSubmitEnabled(_hasValidWord and _hasValidPhonetic and
                     _hasValidTranslation);

    return QValidator::Acceptable;
}

void
SFOSubmitWordModel::UpdateElement(const QString& element,
                                  const QVariant& identifier)
{
    if (identifier.toString() == QString("word")) {
        _word = element;
    } else if (identifier.toString() == QString("phonetic")) {
        _phonetic = element;
    } else if (identifier.canConvert<int>()) {
        int index = identifier.toInt();
        // Is it a translation or phonetic?
        if (index < _translations.size()) {
            _translations.replace(index, element);
        } else {
            _translations.append(element);
        }
    }
}

void
SFOSubmitWordModel::AddTranslation(const QString& translation)
{
    qDebug() << "Adding " << translation;
    _translations.append(translation);
    _SyncFromValidators();
    _ResetModel();
}

void
SFOSubmitWordModel::RemoveTranslation(int row)
{
    qDebug() << "removing " << row;
    _translations.removeAt(row);
    qDebug() << "Setting model property again";
    // Kludgy, but we clear it, then set it again so we pull the data again.
    _ResetModel();
}

void
SFOSubmitWordModel::CommitModel()
{
    _SyncFromValidators();

    qDebug() << "Submitting " << _word << " (" << _phonetic << ") with "
             << _translations.size() << " translations.";
    
    QVariantMap transPhonetic;
    for (int i = 0 ; i < _translations.size() ; ++i) {
        transPhonetic[_translations[i]] = "";
    }

    if (!_word.isEmpty() && (transPhonetic.size() > 0)) {
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
        switch(role) {
        case IndexRole:
            return QVariant(index.row());
        case TranslationRole:
            return QVariant(translation);
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
        UpdateElement(validator->GetValidated(), *it);
    }
}

void
SFOSubmitWordModel::_ResetModel()
{
    _validators.clear();
    emit dataChanged(index(0), index(_translations.size()-1));
}

void
SFOSubmitWordModel::_ClearModel()
{
    _word = "";
    _phonetic = "";
    _translations.clear();
    _ResetModel();
}
