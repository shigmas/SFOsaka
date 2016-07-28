#ifndef SFOVALIDATORRECEIVER_H
#define SFOVALIDATORRECEIVER_H

#include <QObject>
#include <QValidator.h>
#include <QVariant>

class SFOValidator;

// This is an interface, even though we can't enforce it anywhere, because
// we can't pass this class through Quick.
class SFOValidatorReceiver
{
public:
    virtual void AddValidator(const QVariant& identifier,
                              SFOValidator *validator) = 0;

    virtual QValidator::State Validate( QString & input, int & pos );
};

#endif // SFOVALIDATORRECEIVER_H
