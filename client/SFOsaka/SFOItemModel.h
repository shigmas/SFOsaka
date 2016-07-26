#ifndef SFOITEMMODEL_H
#define SFOITEMMODEL_H

#include "SFOPartner.h"

#include <FJTypes.h>

#include <QObject>
#include <QAbstractListModel>
#include <QPlace>
#include <QPlaceSearchResult>
#include <QGeoCoordinate>

class QQmlContext;

class SFOItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate position READ GetPosition WRITE SetPosition NOTIFY PositionChanged )

public:
    static const QString ModelIdentifier;
    static const QString PartnerModelIdentifier;

    explicit SFOItemModel(QQmlContext *context, QObject *parent = Q_NULLPTR);
    virtual ~SFOItemModel();
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;

    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QHash<int, QByteArray> roleNames() const;

    QGeoCoordinate GetPosition() const;
    void SetPosition(const QGeoCoordinate& position);

Q_SIGNALS:
    void PositionChanged();

public slots:
    // This is just a debug test to see how to connect w/ signals in QML. But
    // we'll likely need this for a manual refresh.
    void HandleRefresh();
    void HandleItemSelected(const QString& title);

protected slots:
    void _HandlePartnerUpdate();

private:
    QQmlContext *_context;
    QGeoCoordinate _position;
    QHash<int, QByteArray> _roleNames;
    SFOPartner _emptyPartner;
    // mutable because creating an index requires a non-const pointer.
    mutable QPlace _root;
};

#endif // SFOITEMMODEL_H
