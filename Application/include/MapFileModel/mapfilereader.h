#pragma once
#include "MapFileModel/mapfile.h"
#include <memory>
#include <vector>

namespace DB
{
class Processor;
}
class MapFileReader
{
public:
    MapFileReader();
    ~MapFileReader();

    std::pair<bool, std::vector<MapFile>> requestFilesBrowse();

private:
    std::unique_ptr<DB::Processor> _processor;
};
