#pragma once
#include "connectionmanager.h"
#include "dbtypes.h"
#include "QSqlQuery"
#include "QVariantList"


namespace DB {
class Executor
{
public:
    Executor();
    Executor(const Executor&) = delete;
    Executor& operator=(const Executor&) = delete;

    std::pair<DBResult, QSqlQuery> execute(const QString& queryText, const QVariantList& args = {});

private:
    ConnectionManager& _connectionManager;
};
}

