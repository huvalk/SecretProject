#pragma once
#include <QAbstractListModel>
#include <vector>
#include <MapFileModel/mapfile.h>
#include "mapfilereader.h"
class MapFileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MapFileModel();

    static void registerMe(const std::string& moduleName);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE int getMapFileID(int index) const;

private:
    MapFileReader _mapFileReader;
    std::vector<MapFile> _files;
    enum MapFileRole {
        MapID = Qt::UserRole + 1,
        TagRole,
        NameRole
    };

    bool updateMapFiles();
};
