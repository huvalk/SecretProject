#include "DataBase/commondatabase.h"
#include "DataBase/dbtypes.h"
#include <QSqlQuery>
#include <QSqlRecord>

CommonDataBase::CommonDataBase(QObject *parent)
    : QObject(parent),
      _connectionManager{ConnectionManager::instance()}
{

}

CommonDataBase::~CommonDataBase()
{

}

CommonDataBase &CommonDataBase::instance()
{
    static CommonDataBase instance;
    return instance;
}

std::pair<DBResult, std::vector<QVariantList> > CommonDataBase::selectMapFiles()
{
    if (!_connectionManager.isValide())
    {
        qCritical() << "Connection to database is not valid";
        return std::make_pair(DBResult::FAIL, std::vector<QVariantList>{});
    }

    QSqlQuery query {
        "SELECT * FROM MapFiles;"
    };

    if (!query.exec() && query.lastError().isValid())
    {
        qCritical() << query.lastError().text();
        return std::make_pair(DBResult::FAIL, std::vector<QVariantList>{});
    }


    std::vector<QVariantList> returnData;

    while (query.next())
    {
        const QSqlRecord& resultRecord {query.record()};
        QVariantList resultData;
        resultData.reserve(resultRecord.count());

        for (int i = 0; i < resultRecord.count(); ++i)
        {
            resultData.push_back(resultRecord.value(i));
        }

        returnData.push_back(std::move(resultData));
    }


    return std::make_pair(DBResult::OK, std::move(returnData));
}

bool CommonDataBase::saveMap(const int mapID, const QString &jsonMap)
{
    if (!_connectionManager.isValide())
    {
        qCritical() << "Connection to database is not valid";
        return false;
    }

    QSqlQuery query {
        "INSERT INTO Maps VALUES (?, ?)"
    };

    query.bindValue(0, mapID);
    query.bindValue(1, jsonMap);

    bool result {true};

    if (!query.exec() && query.lastError().isValid())
    {
        qCritical() << query.lastError().text();
        result = false;
    }

    return result;
}

int CommonDataBase::saveMapFile(const QString &name)
{
    if (!_connectionManager.isValide())
    {
        qCritical() << "Connection to database is not valid";
        return false;
    }

    QSqlQuery query {
        "INSERT INTO MapFiles (map_name) VALUES (?)"
    };

    query.bindValue(0, name);

    if (!query.exec() && query.lastError().isValid())
    {
        qCritical() << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

