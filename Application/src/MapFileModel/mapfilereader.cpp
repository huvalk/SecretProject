#include "dbtypes.h"
#include "MapFileModel/mapfilereader.h"
#include "processor.h"

MapFileReader::MapFileReader()
    : _processor {std::make_unique<DB::Processor>()}
{

}

MapFileReader::~MapFileReader()
{

}

std::vector<MapFile> transform(const std::vector<QVariantList>& source)
{
    std::vector<MapFile> target;
    std::transform(source.begin(), source.end(), std::back_inserter(target),
                   [](const QVariantList& query) {
        return MapFile {query[1].toString(),
                        query[2].toString()};
    });

    return target;
}

std::pair<bool, std::vector<MapFile> > MapFileReader::requestFilesBrowse()
{
    DB::DBResult result;
    std::vector<QVariantList> query;
    std::tie(result, query) = _processor->requestTableData(DB::DBTables::MapFiles);

    return std::make_pair(result == DB::DBResult::OK, transform(query));
}
