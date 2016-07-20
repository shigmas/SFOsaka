#include "SFOValidator.h"

#include "SFOSubmitWordModel.h"

#include <QDebug>

SFOValidator::SFOValidator() :
    _receiver(NULL)
{

}

QString
SFOValidator::GetValidated() const
{
    return _validated;
}

QValidator::State
SFOValidator::validate ( QString & input, int &  ) const
{
    _validated = input;
    return QValidator::Acceptable;
}

SFOSubmitWordModel *
SFOValidator::GetReceiver() const
{
    return _receiver;
}

void
SFOValidator::SetReceiver(SFOSubmitWordModel *receiver)
{
    if (_receiver != receiver) {
        _receiver = receiver;
        _AddToReceiver();
        emit ReceiverChanged();
    }
}

QVariant
SFOValidator::GetIdentifier() const
{
    return _identifier;
}

void
SFOValidator::SetIdentifier(const QVariant& identifier)
{
    if (!_identifier.isValid() and (_identifier != identifier)) {
        _identifier = identifier;
        _AddToReceiver();
        emit IdentifierChanged();
    }
}

void
SFOValidator::_AddToReceiver()
{
    if (_receiver and _identifier.isValid()) {
        _receiver->AddValidator(_identifier, this);
    }
}
