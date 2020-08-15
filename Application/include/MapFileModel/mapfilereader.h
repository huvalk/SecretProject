#pragma once
#include "MapFileModel/mapfile.h"
#include "DataBase/commondatabase.h"
#include <memory>
#include <vector>

class MapFileReader
{
public:
    MapFileReader();
    ~MapFileReader();

    std::pair<bool, std::vector<MapFile>> requestFilesBrowse();

private:
    CommonDataBase& _database;
};
