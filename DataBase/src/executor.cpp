#include "executor.h"
#include <QDebug>
#include <QSqlError>

namespace DB {
Executor::Executor()
    : _connectionManager {ConnectionManager::instance()}
{

}

std::pair<DBResult, QSqlQuery> Executor::execute(const QString &queryText, const QVariantList &args)
{
    if (!_connectionManager.isValide())
    {
        qCritical() << "Connection to database is not valid";
        return std::make_pair(DBResult::FAIL, QSqlQuery{});
    }

    QSqlQuery query {queryText};

    for (int i = 0; i < args.size(); ++i)
    {
        query.bindValue(i, args[i]);
    }

    DBResult result {DBResult::OK};

    if (!query.exec() && query.lastError().isValid())
    {
        qCritical() << query.lastError().text();
        result = DBResult::FAIL;
    }

    return std::make_pair(result, query);
}
}
