#include "treeitem.h"

#include <QStringList>


TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

TreeItem::~TreeItem()
{
    clearChilds();
}

TreeItem * TreeItem::appendChild(const QList<QVariant> &data)
{
    TreeItem * child = new TreeItem(data,this);
    m_childItems.append(child);
    return child;
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}

QList<TreeItem *> &TreeItem::allChilds()
{
    return m_childItems;
}

void TreeItem::clearChilds()
{
    qDeleteAll(m_childItems);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return m_itemData.value(column);
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
