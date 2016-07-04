#ifndef SFOSUBMITWORDMODEL_H
#define SFOSUBMITWORDMODEL_H

#include <QObject>
#include <QAbstractListModel>

class QQmlContext;

class SFOSubmitWordModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString word READ GetWord WRITE SetWord NOTIFY WordChanged)
public:
    explicit SFOSubmitWordModel(QQmlContext *context, QObject *parent = 0);
    ~SFOSubmitWordModel();

    QString GetWord() const;
    void SetWord(const QString& word);


    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void WordChanged();

public slots:
    void AddTranslation(const QString& translation);
    void CommitModel();

private:
    QQmlContext *_context;

    // Meta model stuff for QML model types
    QHash<int, QByteArray> _roleNames;

    QString _word;
    QStringList _translations;
};

#endif // SFOSUBMITWORDMODEL_H
