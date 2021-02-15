#include "GraphicScene/graphicparser.h"
#include "QJsonDocument"
#include "QJsonObject"

GraphicParser::GraphicParser()
{

}

QString GraphicParser::generateJSONScene(const GraphicTypes::building<GraphicLine> &scene, const GraphicTypes::building<GraphicPolygon> &polygons)
{
    int lowScene = 0;
    auto sceneIt = scene.begin();
    auto polyIt = polygons.begin();
    auto sceneEnd = scene.end();
    auto polyEnd = polygons.end();
    QString _jsonScene = "{";

    for(; sceneIt != sceneEnd && polyIt != polyEnd ;)
    {
        if (sceneIt->first <= polyIt->first && sceneIt != sceneEnd)
        {
            lowScene = sceneIt->first;
            _jsonScene.append(QString::number(sceneIt->first));

            _jsonScene.append(": { ");
            generateJSONLines(sceneIt->second, _jsonScene);

            if (polyIt->first == lowScene)
            {
                _jsonScene.append(", ");
                generateJSONPolygons(polyIt->second, _jsonScene);
                polyIt++;
            }
            sceneIt++;

            _jsonScene.append("}, ");
        } else if (polyIt != polyEnd)
        {
            _jsonScene.append(QString::number(polyIt->first));

            _jsonScene.append(": { ");
            generateJSONPolygons(polyIt->second, _jsonScene);
            polyIt++;
            _jsonScene.append("}, ");
        }
    }


    if (scene.size() != 0)
    {
        _jsonScene.remove(_jsonScene.size() - 2, 2);
    }
    _jsonScene.append("}");

    return _jsonScene;
}

void GraphicParser::generateJSONLines(const GraphicTypes::floor<GraphicLine>& scene, QString &jsonScene)
{
    jsonScene.append("lines: [");

    for(auto& item: scene)
    {
        jsonScene.append("{");
        auto point = item->getFirstPoint();
        jsonScene.append(QString::number(point.x()));
        jsonScene.append(", ");
        jsonScene.append(QString::number(point.y()));
        jsonScene.append(", ");
        point = item->getSecondPoint();
        jsonScene.append(QString::number(point.x()));
        jsonScene.append(", ");
        jsonScene.append(QString::number(point.y()));
        jsonScene.append("}, ");
    }
    if (scene.size() != 0)
    {
        jsonScene.remove(jsonScene.size() - 2, 2);
    }
    jsonScene.append("]");
}

void GraphicParser::generateJSONPolygons(const GraphicTypes::floor<GraphicPolygon>& polygons, QString &jsonScene)
{
    jsonScene.append("polys: [");

    for(auto& item: polygons)
    {
        jsonScene.append("{");
        auto polyPoints = item->getPolygonPoints();
        for (const auto &point: polyPoints)
        {
            jsonScene.append(QString::number(point.x()));
            jsonScene.append(", ");
            jsonScene.append(QString::number(point.y()));
            jsonScene.append(", ");
        }

        if (polyPoints.size() != 0)
        {
            jsonScene.remove(jsonScene.size() - 2, 2);
        }
        jsonScene.append("}, ");
    }

    if (polygons.size() != 0)
    {
        jsonScene.remove(jsonScene.size() - 2, 2);
    }
    jsonScene.append("]");
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

std::tuple<bool, GraphicTypes::building<GraphicLine>, GraphicTypes::building<GraphicPolygon>> GraphicParser::parseJSONScene(QString json)
{
    QStringRef subStr(&json);
    int state = 0;
    int currentFloor = 0;
    bool noErr = true;
    std::set<std::shared_ptr<GraphicLine>> resLines;
    std::set<std::shared_ptr<GraphicPolygon>> resPolys;
    GraphicTypes::building<GraphicLine> lines;
    GraphicTypes::building<GraphicPolygon> polygons;

    subStr = eraseSpaces(subStr);
    while (subStr.size() != 0 && state != 9 && noErr)
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
                state = 9;
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
                state = 9;
            } else if (std::tie(noErr, subStr, resLines)  =  parseLines(subStr); noErr)
            {
                lines[currentFloor] = std::move(resLines);
                state = 5;
            } else
            {
                noErr = false;
            }
            break;

        case 5:
            if (subStr[0] == '}')
            {
                subStr = eraseSymbol(1, subStr);
                state = 8;
            } else if (subStr[0] == ',')
            {
                subStr = eraseSymbol(1, subStr);
                state = 6;
            } else
            {
                noErr = false;
            }
            break;

        case 6:
             if (std::tie(noErr, subStr, resPolys)  =  parsePolygons(subStr); noErr)
            {
                polygons[currentFloor] = std::move(resPolys);
                state = 7;
            } else
            {
                noErr = false;
            }
            break;

        case 7:
            if (subStr[0] == '}')
            {
                subStr = eraseSymbol(1, subStr);
                state = 8;
            } else
            {
                noErr = false;
            }
            break;

        case 8:
            if (subStr[0] == ',')
            {
                subStr = eraseSymbol(1, subStr);
                state = 1;
            } else if (subStr[0] == '}')
            {
                subStr = eraseSymbol(1, subStr);
                state = 9;
            } else
            {
                noErr = false;
            }
            break;
        }

        subStr = eraseSpaces(subStr);
    }

    return std::make_tuple((noErr && state == 9), std::move(lines), std::move(polygons));
}

std::tuple<bool, QStringRef, std::set<std::shared_ptr<GraphicLine>>> GraphicParser::parseLines(QStringRef &json)
{
    int state = 0;
    bool noErr = true;
    std::shared_ptr<GraphicLine> resLine;
    std::set<std::shared_ptr<GraphicLine>> result;
    json = eraseSpaces(json);
    while (json.size() != 0 && state != 5 && noErr)
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
        }

        json = eraseSpaces(json);
    }

    return std::make_tuple((noErr && state == 5), json, std::move(result));
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

std::tuple<bool, QStringRef, std::set<std::shared_ptr<GraphicPolygon>>> GraphicParser::parsePolygons(QStringRef &json)
{
    int state = 0;
    bool noErr = true;
    std::shared_ptr<GraphicPolygon> resPoly;
    std::set<std::shared_ptr<GraphicPolygon>> result;
    json = eraseSpaces(json);
    while (json.size() != 0 && state != 5 && noErr)
    {
        switch (state) {
        case 0:
            if (json.left(5) != "polys")
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
            std::tie(noErr, json, resPoly) = parsePolygon(json);
            if (noErr)
            {
                result.insert(std::move(resPoly));
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
        }

        json = eraseSpaces(json);
    }

    return std::make_tuple((noErr && state == 5), json, std::move(result));
}

std::tuple<bool, QStringRef, std::shared_ptr<GraphicPolygon>> GraphicParser::parsePolygon(QStringRef &json)
{
    int state = 0;
    int it = 0;
    bool noErr = true;
    double tempDouble = 0;
    double tempX = 0;
    QPolygonF resPoly;

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
            if (std::tie(noErr, json, tempDouble) = parseDouble(json); noErr)
            {
                if (it % 2 == 0)
                {
                    tempX = tempDouble;
                } else
                {
                    resPoly << QPointF(tempX, tempDouble);
                }

                it++;
                state = 2;
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
            } else
            {
                state = 3;
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

    return std::make_tuple((noErr && state == 4), json, std::make_shared<GraphicPolygon>(resPoly));
}
