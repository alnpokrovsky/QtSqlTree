#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "src/treemodel/treemodel.h"
#include "src/db/database.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Database database;
    if (!database.connectToDB())
        return 1;

    TreeModel treeModel(database.sqlQuery());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("treeModel", &treeModel);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
