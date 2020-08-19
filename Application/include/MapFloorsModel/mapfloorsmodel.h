#pragma once

#include <QAbstractListModel>
#include <QVector>

class MapFloorsModel: public QAbstractListModel
{
    Q_OBJECT
public:
    MapFloorsModel();

    static void registerMe(const std::string& moduleName);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool updateMapFloors(QVector<int> mapFloors);
    Q_INVOKABLE int upFloor(int index);
    Q_INVOKABLE int downFloor(int index);
    Q_INVOKABLE int getFloor(int index);

private:
    std::vector<int> _floors;
    enum MapFloorsRole {
        Floor = Qt::UserRole + 1
    };
};
