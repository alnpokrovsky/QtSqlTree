#include "treeitem.h"

#include "treemodel.h"
#include "treedata.h"

#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

TreeModel::TreeModel(QSqlQuery *sqlQuery, QObject *parent)
    : QAbstractItemModel(parent),
      _sqlQuery(sqlQuery)
{
    _roleNameMapping[TreeModelRoleName] = "title";

    QList<QVariant> rootData;
    //rootData << "Title";
    rootData << newTreeData(0, "Root");
    _rootItem = new TreeItem(rootData);

    updateModel(_rootItem);
}

TreeModel::~TreeModel()
{
    delete _rootItem;
    delete _sqlQuery;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    } else {
        return _rootItem->columnCount();
    }
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != TreeModelRoleName)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(role - Qt::UserRole - 1);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    return _roleNameMapping;
}

void TreeModel::updateModel(TreeItem *parent)
{
    parent->clearChilds();
    if (parent == _rootItem) {
        _sqlQuery->prepare("SELECT id, name FROM Exercises WHERE parentId IS NULL;");
    } else {
        _sqlQuery->prepare("SELECT id, name FROM Exercises WHERE parentId=?;");
        TreeData* treeData = (parent->data(0)).value<TreeData*>();
        _sqlQuery->addBindValue(treeData->get_id());
    }
    _sqlQuery->exec();

    // заполняем текущий уровень дерева
    QSqlRecord result = _sqlQuery->record();
    while (_sqlQuery->next())
    {
        QList<QVariant> columnData;
        columnData.clear();
        int id = _sqlQuery->value(result.indexOf("id")).toInt();
        QString str = _sqlQuery->value(result.indexOf("name")).toString();
        columnData << newTreeData(id, str);
        parent->appendChild(columnData);
    }

    // рекурсивно заполняем нижние уровни
    for (TreeItem * child : parent->allChilds()) {
        updateModel(child);
    }
}

QVariant TreeModel::newTreeData(int id, const QString &text)
{
    TreeData * newTreeData = new TreeData(this);
    newTreeData->set_text(text);
    newTreeData->set_id(id);
    QVariant val;
    val.setValue(newTreeData);
    return val;
}
