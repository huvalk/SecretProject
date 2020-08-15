#pragma once

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include "DataBase/connectionmanager.h"
#include "DataBase/dbtypes.h"

class CommonDataBase : public QObject
{
    Q_OBJECT
public:
    CommonDataBase(const CommonDataBase&) = delete;
    CommonDataBase& operator=(const CommonDataBase&) = delete;
    ~CommonDataBase();

    static CommonDataBase& instance();
    std::pair<DBResult, std::vector<QVariantList> > selectMapFiles();

private:
    explicit CommonDataBase(QObject *parent = 0);
    ConnectionManager& _connectionManager;

public slots:
    bool saveMap(const int mapID, const QString& jsonMap);
    int saveMapFile(const QString& name);
};
