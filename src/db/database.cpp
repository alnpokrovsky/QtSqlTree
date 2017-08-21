#include "database.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>


Database::Database(QObject *parent) : QObject(parent)
{
    //connectToDB();
}

Database::~Database()
{
    closeDB();
}

QString Database::dbPath()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if (!QDir(path).exists()) {
        QDir().mkpath(path);
    }
    return path + QDir::separator() + DB_NAME;
}

bool Database::connectToDB()
{
    QString name = dbPath();
    if (!openDB(name))
        return false;
    createTables();
    return true;
}

void Database::closeDB()
{
    _db.close();
}

QSqlQuery *Database::sqlQuery()
{
    return new QSqlQuery(_db);
}

bool Database::openDB(QString & dbName)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(dbName);
    if (!_db.open()) {
        qCritical() << "Cannot open database!";
        return false;
    }
    return true;
}

void Database::createTables()
{
    QSqlQuery query(_db);
    query.exec("CREATE TABLE IF NOT EXISTS Maps ( id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL,"
               "name TEXT, folderPath TEXT, topLeftLatitude REAL, topLeftLongitude REAL );");

    query.exec("CREATE TABLE IF NOT EXISTS Entities ( id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL,"
               "type INTEGER, name TEXT, folderPath TEXT );");

    query.exec("CREATE TABLE IF NOT EXISTS Exercises ( id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL,"
               "type INTEGER, name TEXT, content TEXT,"
               "parentId INTEGER REFERENCES Exercises (id) ON DELETE CASCADE );");

    query.exec("CREATE TABLE IF NOT EXISTS TargetColors ( id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL,"
               "folderPath TEXT );");

    query.exec("CREATE TABLE IF NOT EXISTS Session ( id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL,"
               "date INTEGER, name TEXT, exerciseId INTEGER REFERENCES Exercise (id) ON DELETE SET NULL  );");

    query.exec("CREATE TABLE IF NOT EXISTS MapTargets ( sessionId INTEGER REFERENCES Session (id) ON DELETE CASCADE,"
               "mapId    INTEGER REFERENCES Maps (id) ON DELETE CASCADE,"
               "entityId INTEGER REFERENCES Entities (id) ON DELETE CASCADE,"
               "colorId  INTEGER REFERENCES TargetColors (id) ON DELETE SET NULL,"
               "showTime INTEGER, latutude REAL, longitude REAL, azimuth INTEGER );");

    query.exec("CREATE TABLE IF NOT EXISTS MapLandmarks ( sessionId INTEGER REFERENCES Session (id) ON DELETE CASCADE,"
               "mapId    INTEGER REFERENCES Maps (id) ON DELETE CASCADE,"
               "entityId INTEGER REFERENCES Entities (id) ON DELETE CASCADE, latitude REAL, longitude REAL, azimuth INTEGER );");

    query.exec("CREATE TABLE IF NOT EXISTS Ammunition ( sessionId INTEGER REFERENCES Session (id) ON DELETE CASCADE,"
               "type INTEGER, subtype INTEGER, quantity INTEGER );");

    query.exec("PRAGMA foreign_keys = ON;");
}
