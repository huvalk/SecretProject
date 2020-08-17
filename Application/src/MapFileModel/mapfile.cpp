#include "MapFileModel/mapfile.h"

MapFile::MapFile(int id, QString tag, QString name)
    : _id(id),
      _tag {std::move(tag)},
      _name {std::move(name)}
{

}

int MapFile::id() const
{
    return _id;
}

QString MapFile::tag() const
{
    return _tag;
}

QString MapFile::name() const
{
    return _name;
}
