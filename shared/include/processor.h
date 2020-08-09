#pragma once
#include "dbtypes.h"
#include <memory>
#include <QVariantList>

namespace DB
{
class Processor
{
public:
    Processor();
    ~Processor();
    std::pair<DBResult, std::vector<QVariantList>> requestTableData(DBTables table);

private:
    struct ProcessorPrivate;
    std::unique_ptr<ProcessorPrivate> _m;
};
}
