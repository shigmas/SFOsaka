#ifndef SFOVALIDATOR_H
#define SFOVALIDATOR_H

#include <QValidator>

class SFOValidatorReceiver;

class SFOValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(QVariant receiver READ GetReceiver WRITE SetReceiver NOTIFY ReceiverChanged )
    Q_PROPERTY(QVariant identifier READ GetIdentifier WRITE SetIdentifier NOTIFY IdentifierChanged )

public:
    SFOValidator();

    virtual QValidator::State validate (QString & input,
                                        int & pos ) const;

    QString GetValidated() const;

    QVariant GetReceiver() const;
    void SetReceiver(const QVariant& receiver);
    QVariant GetIdentifier() const;
    void SetIdentifier(const QVariant& identifier);

signals:
    void ReceiverChanged();
    void IdentifierChanged();

protected:
    SFOValidatorReceiver* _GetReceiverFromVariant(const QVariant& var) const;
    void _AddToReceiver();

private:
    QVariant _receiver;
    QVariant _identifier;
    mutable QString _validated;

};

#endif // SFOVALIDATOR_H
