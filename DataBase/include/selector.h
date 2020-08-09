#pragma once
#include "dbtypes.h"
#include "executor.h"
#include "vector"
#include "QString"
#include "QVariantList"

namespace DB
{
class Selector
{
public:
    std::pair<DBResult, std::vector<QVariantList> > selectAll(const QString& tableName);

private:
    Executor _executor;

    QString generateQuery(const QString& tableName) const;
};
}

