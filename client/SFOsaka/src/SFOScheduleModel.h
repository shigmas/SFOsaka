#ifndef SFOSCHEDULEMODEL_H
#define SFOSCHEDULEMODEL_H

#include <QAbstractListModel>

#include "SFOPerformer.h"

class QQmlContext;

class SFOScheduleModel : public QAbstractListModel
{
    Q_OBJECT

public:

    explicit SFOScheduleModel(QQmlContext *context, QObject *parent=0);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QHash<int, QByteArray> roleNames() const;

protected slots:
    void _HandlePerformerUpdate();

protected:
    void _SortPerformers();


    static const QString ScheduleModelIdentifier;

    static const QByteArray TitleDisplayRole;
    static const QByteArray TimeDisplayRole;
    static const QByteArray DescriptionRole;
    static const QByteArray ImageSourceRole;

    static const QHash<int, QByteArray> Roles;

private:
    QQmlContext *_context;
    QHash<int, QByteArray> _roleNames;
    SFOPerformerList _orderedPerfomers;
};

#endif // SFOSCHEDULEMODEL_H
