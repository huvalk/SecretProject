#include "MapFloorsModel/mapfloorsmodel.h"
#include <QQmlEngine>
#include <algorithm>
#include <QDebug>

MapFloorsModel::MapFloorsModel()
    : QAbstractListModel()
{
}

void MapFloorsModel::registerMe(const std::string& moduleName)
{
    qmlRegisterType<MapFloorsModel>(moduleName.c_str(), 1, 0, "MapFloorsModel");
}

QHash<int, QByteArray> MapFloorsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MapFloorsRole::Floor] = "floor";

    return roles;
}

int MapFloorsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(_floors.size());
}

QVariant MapFloorsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    const int& map_file {_floors.at(static_cast<size_t>(index.row()))};

    switch (role) {
    case MapFloorsRole::Floor: {
        return map_file;
    }
    default:
        return true;
    }
}

bool MapFloorsModel::updateMapFloors(QVector<int> mapFloors)
{
    auto oldRows = static_cast<int>(_floors.size());
    auto newRows = mapFloors.size();
    auto difRows = newRows - oldRows;
    if (difRows > 0)
    {
        beginInsertRows(QModelIndex(), oldRows, newRows - 1);
    } else if (difRows < 0)
    {
        beginRemoveRows(QModelIndex(), newRows, oldRows - 1);
    }

    if (mapFloors.size() != 0)
    {
        _floors = mapFloors.toStdVector();
        std::sort(_floors.begin(), _floors.end());
    } else
    {
        beginInsertRows(QModelIndex(), 0, 0);
        _floors.push_back(1);
    }
    if (difRows > 0)
    {
        endInsertRows();
    } else if (difRows < 0)
    {
        endRemoveRows();
    }
    emit dataChanged(createIndex(0, 0), createIndex(static_cast<int>(_floors.size()), 0));

    return true;
}

int MapFloorsModel::upFloor(int index)
{
    if (_floors.size() == 0)
    {
        beginInsertRows(QModelIndex(), 0, 0);
        _floors.push_back(1);
        endInsertRows();
        emit dataChanged(createIndex(0, 0), createIndex(0, 0));
        return 0;
    }

    auto currentFloor = _floors[static_cast<size_t>(index)];
    if (currentFloor > 99)
    {
        return currentFloor;
    }

    if (index == static_cast<int>(_floors.size() - 1))
    {
        beginInsertRows(QModelIndex(), index, index);
        _floors.push_back(currentFloor + 1);
        endInsertRows();
        emit dataChanged(createIndex(0, 0), createIndex(static_cast<int>(_floors.size()), 0));

        return index;
    }

    if ((_floors[static_cast<size_t>(index) + 1]) == (currentFloor + 1))
    {
        return (index + 1);
    } else
    {
        beginInsertRows(QModelIndex(), index + 1, index + 1);
        auto currentFloorIt = _floors.begin();
        for(int i = 0; i <= index; ++i)
        {
            currentFloorIt++;
        }
        _floors.insert(currentFloorIt, (currentFloor + 1));
        endInsertRows();

        return (index + 1);
    }

}

int MapFloorsModel::downFloor(int index)
{
    if (_floors.size() == 0)
    {
        beginInsertRows(QModelIndex(), 0, 0);
        _floors.push_back(1);
        endInsertRows();
        emit dataChanged(createIndex(0, 0), createIndex(0, 0));
        return 0;
    }

    auto currentFloor = _floors[static_cast<size_t>(index)];
    if (currentFloor < -99)
    {
        return currentFloor;
    }

    if (index == 0)
    {
        beginInsertRows(QModelIndex(), index + 1, index + 1);
        _floors.insert(_floors.begin(), currentFloor - 1);
        endInsertRows();
        emit dataChanged(createIndex(0, 0), createIndex(static_cast<int>(_floors.size()), 0));

        return index + 1;
    }

    if ((_floors[static_cast<size_t>(index) - 1]) == (currentFloor - 1))
    {
        return (index - 1);
    } else
    {
        beginInsertRows(QModelIndex(), index, index);
        auto currentFloorIt = _floors.begin();
        for(int i = 0; i < index; ++i)
        {
            currentFloorIt++;
        }
        _floors.insert(currentFloorIt, (currentFloor - 1));
        endInsertRows();

        return index;
    }

}

int MapFloorsModel::getFloor(int index)
{
    if (index < 0){
        return _floors[0];
    } else if (index >= static_cast<int>(_floors.size()))
    {
        return _floors[_floors.size()];
    }

    return _floors[static_cast<size_t>(index)];
}
