#ifndef TREEDATA_H
#define TREEDATA_H

#include <QObject>

#include "src/qqmlhelpers.h"

class TreeData : public QObject
{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(int, id)
    QML_WRITABLE_PROPERTY(QString, text)

public:
    TreeData(QObject *parent = nullptr);
    TreeData(const TreeData &that);
};

Q_DECLARE_METATYPE(TreeData)

#endif // TREEDATA_H
