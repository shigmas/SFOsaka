#include "SFOValidator.h"

#include "SFOSubmitWordModel.h"
#include "SFOTranslateController.h"

#include <QDebug>

SFOValidator::SFOValidator()
{

}

QString
SFOValidator::GetValidated() const
{
    return _validated;
}

QValidator::State
SFOValidator::validate ( QString & input, int & pos ) const
{
    _validated = input;
    SFOValidatorReceiver *receiver = _GetReceiverFromVariant(_receiver);
    if (receiver) {
        return receiver->Validate(_identifier, input, pos);
    }
    return QValidator::Acceptable;
}

QVariant
SFOValidator::GetReceiver() const
{
    return _receiver;
}

void
SFOValidator::SetReceiver(const QVariant& receiver)
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

// We can't store the base/abstract class because it needs to be concrete and
// constructable as a QVariant for one, but also, we can't pass it through as
// that base class *and* be known as a QObject.
SFOValidatorReceiver*
SFOValidator::_GetReceiverFromVariant(const QVariant& var) const
{
    if (var.isValid()) {
        if (var.canConvert<SFOSubmitWordModel *>()) {
            return var.value<SFOSubmitWordModel *>();
        } else if (var.canConvert<SFOTranslateController *>()) {
            return var.value<SFOTranslateController *>();
        } else {
            qDebug() << "Receiver is an unknown type (" << var.typeName()
                     << ". Unable to add validator";
        }            
    }

    return NULL;
}

void
SFOValidator::_AddToReceiver()
{
    if (_identifier.isValid()) {
        SFOValidatorReceiver *receiver = _GetReceiverFromVariant(_receiver);
        if (receiver) {
            receiver->AddValidator(_identifier, this);
        }
    }
}
