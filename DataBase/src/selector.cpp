#include "selector.h"
#include "QSqlRecord"
#include "QSqlQuery"

namespace DB
{
std::pair<DBResult, std::vector<QVariantList>> Selector::selectAll(const QString& tableName)
{
    QString query {generateQuery(tableName)};

    DBResult result;
    QSqlQuery resultQuery;
    std::tie(result, resultQuery) = _executor.execute(query);

    std::vector<QVariantList> returnData;
    if (result == DBResult::OK)
    {
//        returnData.clear();
//        returnData.reserve(resultQuery.size());

        while (resultQuery.next())
        {
            const QSqlRecord& resultRecord {resultQuery.record()};
            QVariantList resultData;
            resultData.reserve(resultRecord.count());

            for (int i = 0; i < resultRecord.count(); ++i)
            {
                resultData.push_back(resultRecord.value(i));
            }

            returnData.push_back(std::move(resultData));
        }
    }

    return std::make_pair(result, std::move(returnData));
}

QString Selector::generateQuery(const QString& tableName) const
{
    QString query {"SELECT * FROM " + tableName + ";"};
    return query;
}
}

