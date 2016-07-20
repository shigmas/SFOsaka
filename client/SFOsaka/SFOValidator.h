#ifndef SFOVALIDATOR_H
#define SFOVALIDATOR_H

#include <QValidator>

// Looks like the compiler needs to know about the model to use it as a property
#include "SFOSubmitWordModel.h"

class SFOValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(SFOSubmitWordModel * receiver READ GetReceiver WRITE SetReceiver NOTIFY ReceiverChanged )
    Q_PROPERTY(QVariant identifier READ GetIdentifier WRITE SetIdentifier NOTIFY IdentifierChanged )

public:
    SFOValidator();

    virtual QValidator::State validate (QString & input,
                                        int & pos ) const;

    QString GetValidated() const;

    SFOSubmitWordModel * GetReceiver() const;
    void SetReceiver(SFOSubmitWordModel *receiver);
    QVariant GetIdentifier() const;
    void SetIdentifier(const QVariant& identifier);

signals:
    void ReceiverChanged();
    void IdentifierChanged();

protected:
    void _AddToReceiver();

private:
    SFOSubmitWordModel * _receiver;
    QVariant _identifier;
    mutable QString _validated;

};

#endif // SFOVALIDATOR_H
