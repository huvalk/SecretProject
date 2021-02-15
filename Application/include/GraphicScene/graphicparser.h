#pragma once

#include <GraphicScene/graphicpoint.h>
#include <GraphicScene/graphicpolygon.h>
#include <GraphicScene/graphicline.h>
#include <GraphicScene/graphictypes.h>
#include <map>
#include <memory>
#include <set>
#include <QString>

class GraphicParser
{
public:
    GraphicParser();

    QString generateJSONScene(const GraphicTypes::building<GraphicLine>& scene);
    std::pair<bool, GraphicTypes::building<GraphicLine>> parseJSONScene(QString json);

private:
    QStringRef eraseSpaces(QStringRef &json);
    QStringRef eraseSymbol(int len, QStringRef &json);
    std::tuple<bool, QStringRef, int> parseNumber(QStringRef &json);
    std::tuple<bool, QStringRef, double> parseDouble(QStringRef &json);
    std::tuple<bool, QStringRef, std::set<std::shared_ptr<GraphicLine>>> parseLines(QStringRef &json);
    std::tuple<bool, QStringRef, std::shared_ptr<GraphicLine>> parseLine(QStringRef &json);
    std::tuple<bool, QStringRef, std::set<std::shared_ptr<GraphicPolygon>>> parsePolygons(QStringRef &json);
    std::tuple<bool, QStringRef, std::shared_ptr<GraphicPolygon>> parsePolygon(QStringRef &json);
};
