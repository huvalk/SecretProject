#pragma once
#include "dbtypes.h"
#include <map>
#include <QString>

// TODO мапить команду к функтуру - методу класса процессора. Придумать как. Или сделать просто методы
namespace DB
{
static const std::map<DBTables, QString> TableMapper {
    {DBTables::MapFiles, "MapFiles"}
};

static const std::map<QString, QString> TablesMapping {
};
}
