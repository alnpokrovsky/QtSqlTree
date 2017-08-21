#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;
class QSqlQuery;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum TreeModelRoles
    {
        TreeModelRoleName = Qt::UserRole + 1,
    };

    //TreeModel(const QString &data, QObject *parent = nullptr);
    TreeModel(QSqlQuery * sqlQuery, QObject *parent = nullptr);
    ~TreeModel();

    /* QAbstractItemModel interface */
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void updateModel(TreeItem * parent);

protected:
    QVariant newTreeData(int id, const QString &text);

private:
    QSqlQuery * _sqlQuery;
    TreeItem *_rootItem;
    QHash<int, QByteArray> _roleNameMapping;
};

#endif // TREEMODEL_H
