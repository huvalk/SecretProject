#include "MapFileModel/mapfilemodel.h"
#include <QQmlEngine>
#include <QDebug>

MapFileModel::MapFileModel()
{
    const bool updateResults {updateMapFiles()};
    if (!updateResults) {
        qWarning() << "Failed to construct model";
    }
}

void MapFileModel::registerMe(const std::string& moduleName)
{
    qmlRegisterType<MapFileModel>(moduleName.c_str(), 1, 0, "MapFileModel");
}

QHash<int, QByteArray> MapFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MapFileRole::MapID] = "mapID";
    roles[MapFileRole::TagRole] = "tag";
    roles[MapFileRole::NameRole] = "name";

    return roles;
}

int MapFileModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(_files.size());
}

QVariant MapFileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    const MapFile& map_file {_files.at(static_cast<size_t>(index.row()))};

    switch (role) {
    case MapFileRole::MapID: {
        return QVariant::fromValue(map_file.id());
    }
    case MapFileRole::TagRole: {
        return QVariant::fromValue(map_file.tag());
    }
    case MapFileRole::NameRole: {
        return QVariant::fromValue(map_file.name());
    }
    default:
        return true;
    }
}

int MapFileModel::getMapFileID(int index) const
{
    return _files[static_cast<size_t>(index)].id();
}

bool MapFileModel::updateMapFiles()
{
    bool requestResult {false};
    std::vector<MapFile> mapFileResult;
    std::tie(requestResult, mapFileResult) = _mapFileReader.requestFilesBrowse();

    if (requestResult) {
        _files.swap(mapFileResult);
        emit dataChanged(createIndex(0, 0), createIndex(static_cast<int>(_files.size()), 0));
    }

    return requestResult;
}
