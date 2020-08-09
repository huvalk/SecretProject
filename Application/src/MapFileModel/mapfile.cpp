#include "MapFileModel/mapfile.h"

MapFile::MapFile(QString tag, QString name)
    : _tag {std::move(tag)},
      _name {std::move(name)}
{

}

QString MapFile::tag() const
{
    return _tag;
}

QString MapFile::name() const
{
    return _name;
}
