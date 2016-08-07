#include "SFOValidatorReceiver.h"

QValidator::State
SFOValidatorReceiver::Validate( const QVariant& , QString & , int & )
{
    return QValidator::Acceptable;
}
