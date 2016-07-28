#include "SFOValidatorReceiver.h"

QValidator::State
SFOValidatorReceiver::Validate( QString & , int & )
{
    return QValidator::Acceptable;
}
