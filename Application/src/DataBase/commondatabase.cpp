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
        "UPDATE Maps "
        "SET map = ? "
        "WHERE map_id = ?"
    };

    query.bindValue(0, jsonMap);
    query.bindValue(1, mapID);

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

    QSqlQuery queryMapFiles {
        "INSERT INTO MapFiles (map_name) VALUES (?)"
    };

    queryMapFiles.bindValue(0, name);

    if (!queryMapFiles.exec() && queryMapFiles.lastError().isValid())
    {
        qCritical() << queryMapFiles.lastError().text();
        return -1;
    }

    auto mapID = queryMapFiles.lastInsertId().toInt();

    QSqlQuery queryMaps {
        "INSERT INTO Maps VALUES (?, '{}')"
    };

    queryMaps.bindValue(0, mapID);

    bool result {true};

    if (!queryMaps.exec() && queryMaps.lastError().isValid())
    {
        qCritical() << queryMaps.lastError().text();
        result = false;
    }

    return mapID;
}

QString CommonDataBase::getMap(const int mapID)
{
    if (!_connectionManager.isValide())
    {
        qCritical() << "Connection to database is not valid";
        return "";
    }

    QSqlQuery query {
        "SELECT map FROM Maps "
        "WHERE map_id = ?"};
    query.bindValue(0, mapID);

    if (!query.exec() && query.lastError().isValid())
    {
        qCritical() << query.lastError().text();
        return "";
    }

    QString result{""};
    if (query.next())
    {
        const QSqlRecord& resultRecord {query.record()};

        result = resultRecord.value(0).toString();
    }


    return result;
}

