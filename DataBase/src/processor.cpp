#include "dbmapper.h"
#include "processor.h"
#include "selector.h"
#include <QVariantList>

namespace DB
{
struct Processor::ProcessorPrivate
{
    Selector selector;
};

Processor::Processor()
    : _m {std::make_unique<ProcessorPrivate>()}
{

}

Processor::~Processor()
{}

std::pair<DBResult, std::vector<QVariantList> > Processor::requestTableData(DB::DBTables table)
{
    return _m->selector.selectAll(TableMapper.at(table));
}
}

