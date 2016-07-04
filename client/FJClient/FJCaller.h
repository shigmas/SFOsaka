#ifndef FJCALLER_H
#define FJCALLER_H

#include "FJTypes.h"

#include <QObject>

FJ_DECLARE_PTRS(FJOperation)

enum FJError {
    FJNoError, // i.e. success
    FJNetworkError,
    FJParsingError,
};


class FJCaller : public QObject
{
    Q_OBJECT
public:
    explicit FJCaller(QObject *parent = 0);
    virtual ~FJCaller();

    // Callback for the operation. It passes a weak copy of us so that we can
    // know which operation we are handling.
    virtual void HandleResponse(const QJsonDocument& document, FJError error,
                                const FJOperation* operation) = 0;

signals:

public slots:
};

#endif // FJCALLER_H
