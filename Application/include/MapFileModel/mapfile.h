#pragma once
#include <QString>

// Класс представляет делегат для модели QML в поиске
class MapFile
{
public:
    MapFile() = default;
    MapFile(QString tag, QString name);

    QString tag() const;
    QString name() const;

private:
    QString _tag;
    QString _name;
};
