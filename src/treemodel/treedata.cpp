#include "treedata.h"

TreeData::TreeData(QObject *parent)
    : QObject(parent),
      m_id(0)
{
    ;
}

TreeData::TreeData(const TreeData &that)
    : QObject(that.parent())
{
    m_text = that.get_text();
    m_id = that.get_id();
}
