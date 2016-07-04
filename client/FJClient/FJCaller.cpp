#include "FJCaller.h"

#include <QDebug>

FJCaller::FJCaller(QObject *parent) : QObject(parent)
{
}

FJCaller::~FJCaller()
{
    qDebug() << "FJCaller::~FJCaller()";
}
