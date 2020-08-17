#pragma once
#include <QString>

// Класс представляет делегат для модели QML в поиске
class MapFile
{
public:
    MapFile() = default;
    MapFile(int id, QString tag, QString name);

    int id() const;
    QString tag() const;
    QString name() const;

private:
    int         _id;
    QString _tag;
    QString _name;
};
