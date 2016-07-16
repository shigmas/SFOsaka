#ifndef SFOEVENTFILTER_H
#define SFOEVENTFILTER_H

#include <QObject>

class SFOEventFilter : public QObject
{
Q_OBJECT

public:
    SFOEventFilter();

signals:
    void TextChanged(const QString&);

protected:
    virtual bool eventFilter(QObject * watched, QEvent * event );
};

#endif // SFOEVENTFILTER_H
