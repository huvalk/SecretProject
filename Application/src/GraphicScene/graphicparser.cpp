#include "GraphicScene/graphicparser.h"

GraphicParser::GraphicParser()
{

}

QString GraphicParser::generateJSONScene(const GraphicTypes::floor<GraphicLine> &scene)
{
    QString _jsonScene = "{";
        for(auto i = scene.begin(); i != scene.end(); ++i)
        {
            _jsonScene.append(QString::number(i->first));
            _jsonScene.append(": { lines: [");

            for(auto& item: i->second)
            {
                _jsonScene.append("{");
                auto point = item->getFirstPoint();
                _jsonScene.append(QString::number(point.x()));
                _jsonScene.append(", ");
                _jsonScene.append(QString::number(point.y()));
                _jsonScene.append(", ");
                point = item->getSecondPoint();
                _jsonScene.append(QString::number(point.x()));
                _jsonScene.append(", ");
                _jsonScene.append(QString::number(point.y()));
                _jsonScene.append("}, ");
            }
            if (i->second.size() != 0)
            {
                _jsonScene.remove(_jsonScene.size() - 2, 2);
            }
            _jsonScene.append("]}, ");
        }
        if (scene.size() != 0)
        {
            _jsonScene.remove(_jsonScene.size() - 2, 2);
        }
        _jsonScene.append("}");

        return _jsonScene;
}

QStringRef GraphicParser::eraseSpaces(QStringRef &json)
{
    int i = 0;
    for (; i < json.size() && json[i] == ' '; ++i) {}

     return json.right(json.size() - i);
}

QStringRef GraphicParser::eraseSymbol(int len, QStringRef &json)
{
     return json.right(json.size() - len);
}

std::tuple<bool, QStringRef, int> GraphicParser::parseNumber(QStringRef &json)
{
    int i = 0;
    for (; i < json.size(); ++i)
    {
        if ( std::isdigit( json[i].toLatin1() ) == 0 )
        {
            break;
        }
    }

    if (i == 0)
    {
        return std::make_tuple(false, json, 0);
    }
     auto num =  json.left(i).toInt();
     json = eraseSymbol(i, json);
     return std::make_tuple(true, json, num);
}

std::tuple<bool, QStringRef, double> GraphicParser::parseDouble(QStringRef &json)
{
    int i = 0;
    for (; i < json.size(); ++i)
    {
        if ( !( (std::isdigit(json[i].toLatin1()) != 0) || (json[i] == '.') ) )
        {
            break;
        }
    }

    if (i == 0)
    {
        return std::make_tuple(false, json, 0);
    }
     auto num =  json.left(i).toDouble();
     json = eraseSymbol(i, json);
     return std::make_tuple(true, json, num);
}

std::pair<bool, GraphicTypes::floor<GraphicLine>> GraphicParser::parseJSONScene(QString json)
{
    QStringRef subStr(&json);
    int state = 0;
    int currentFloor = 0;
    bool noErr = true;
    std::set<std::shared_ptr<GraphicLine>> resLines;
    GraphicTypes::floor<GraphicLine> points;

    subStr = eraseSpaces(subStr);
    while (subStr.size() != 0 && state != 6 && noErr)
    {
        switch (state) {
        case 0:
            if (subStr[0] == '{')
            {
                subStr = eraseSymbol(1, subStr);
                state = 1;
            } else
            {
                noErr = false;
            }
            break;

        case 1:
            if (subStr[0] == '}')
            {
                subStr = eraseSymbol(1, subStr);
                state = 7;
            } else if (std::tie(noErr, subStr, currentFloor) = parseNumber(subStr); noErr)
            {
                state = 2;
            } else
            {
                noErr = false;
            }

            break;

        case 2:
            if (subStr[0] == ':')
            {
                subStr = eraseSymbol(1, subStr);
                state = 3;
            } else
            {
                noErr = false;
            }
            break;

        case 3:
            if (subStr[0] == '{')
            {
                subStr = eraseSymbol(1, subStr);
                state = 4;
            } else
            {
                noErr = false;
            }
            break;

        case 4:
            if (subStr[0] == '}')
            {
                subStr = eraseSymbol(1, subStr);
                state = 6;
            } else if (std::tie(noErr, subStr, resLines)  =  parseLines(subStr); noErr)
            {
                points[currentFloor] = std::move(resLines);
                state = 5;
            } else
            {
                noErr = false;
            }
            break;

        case 5:
            if (subStr[0] == ',')
            {
                subStr = eraseSymbol(1, subStr);
                state = 1;
            } else if (subStr[0] == '}')
            {
                subStr = eraseSymbol(1, subStr);
                state = 6;
            } else
            {
                noErr = false;
            }
            break;
        }

        subStr = eraseSpaces(subStr);
    }

    return std::make_pair((noErr && state == 6), std::move(points));
}

std::tuple<bool, QStringRef, std::set<std::shared_ptr<GraphicLine>>> GraphicParser::parseLines(QStringRef &json)
{
    int state = 0;
    bool noErr = true;
    std::shared_ptr<GraphicLine> resLine;
    std::set<std::shared_ptr<GraphicLine>> result;
    json = eraseSpaces(json);
    while (json.size() != 0 && state != 6 && noErr)
    {
        switch (state) {
        case 0:
            if (json.left(5) != "lines")
            {
                noErr = false;
            } else
            {
                json = eraseSymbol(5, json);
                state = 1;
            }
            break;

        case 1:
            if (json[0] == ':')
            {
                json = eraseSymbol(1, json);
                state = 2;
            } else
            {
                noErr = false;
            }
            break;

        case 2:
            if (json[0] == '[')
            {
                json = eraseSymbol(1, json);
                state = 3;
            } else
            {
                noErr = false;
            }
            break;

        case 3:
            std::tie(noErr, json, resLine) = parseLine(json);
            if (noErr)
            {
                result.insert(std::move(resLine));
                state = 4;
            } else
            {
                noErr = false;
            }
            break;

        case 4:
            if (json[0] == ']')
            {
                json = eraseSymbol(1, json);
                state = 5;
            } else if (json[0] == ',')
            {
                json = eraseSymbol(1, json);
                state = 3;
            } else
            {
                noErr = false;
            }
            break;

        case 5:
            if (json[0] == '}')
            {
                json = eraseSymbol(1, json);
                state = 6;
            } else
            {
                noErr = false;
            }
            break;
        }

        json = eraseSpaces(json);
    }

    return std::make_tuple((noErr && state == 6), json, std::move(result));
}

std::tuple<bool, QStringRef, std::shared_ptr<GraphicLine>> GraphicParser::parseLine(QStringRef &json)
{
    int state = 0;
    int it = 0;
    bool noErr = true;
    double resPos[4];

    json = eraseSpaces(json);
    while (json.size() != 0 && state != 4 && noErr)
    {
        switch (state) {
        case 0:
            if (json[0] == '{')
            {
                json = eraseSymbol(1, json);
                state = 1;
            } else
            {
                noErr = false;
            }
            break;

        case 1:
            if (std::tie(noErr, json, resPos[it]) = parseDouble(json); noErr)
            {
                if (it < 3)
                {
                    state = 2;
                } else
                {
                    state = 3;
                }
            } else
            {
                noErr = false;
            }
            break;

        case 2:
            if (json[0] == ',')
            {
                json = eraseSymbol(1, json);
                state = 1;
                ++it;
            } else
            {
                noErr = false;
            }
            break;

        case 3:
            if (json[0] == '}')
            {
                json = eraseSymbol(1, json);
                state = 4;
            } else
            {
                noErr = false;
            }
            break;
        }

        json = eraseSpaces(json);
    }

    return std::make_tuple((noErr && state == 4), json, std::make_shared<GraphicLine>(QPointF(resPos[0], resPos[1]), QPointF(resPos[2], resPos[3])));
}
