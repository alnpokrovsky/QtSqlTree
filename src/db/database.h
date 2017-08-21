#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

#define DB_NAME "vsimulator.db"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connectToDB();
    void closeDB();
    QSqlQuery * sqlQuery();

signals:

public slots:

protected:
    static QString dbPath();
    bool openDB(QString &dbName);
    void createTables();
private:
    QSqlDatabase _db;
};

#endif // DATABASE_H
