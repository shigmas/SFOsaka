#include "SFOEventFilter.h"

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QtQuick/QQuickItem>

#include <QInputMethodEvent>


SFOEventFilter::SFOEventFilter()
{

}

bool
SFOEventFilter::eventFilter(QObject * watched, QEvent * event )
{
    if (watched->objectName() != "translatorInput") {
        return false;
    }
    if (event->type() == QEvent::InputMethod) {
        QInputMethodEvent *inputMethod = static_cast<QInputMethodEvent *>(event);
        if (!inputMethod) {
            qDebug() << "Unable to cast correct type";
        } else {
            QString commit = inputMethod->commitString();
            if (!commit.isEmpty()) {
                QString current = watched->property("text").toString();
                int cursorPosition = watched->property("cursorPosition").toInt();
                current.insert(cursorPosition,commit);
                emit TextChanged(current);
            }
        }
    }
    return false;
}
