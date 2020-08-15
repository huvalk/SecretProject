#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DataBase/commondatabase.h"
#include "GraphicScene/graphicscene.h"
#include "MapFileModel/mapfilemodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    MapFileModel::registerMe("MapFiles");
    GraphicScene::registerMe("GraphicScene");
    engine.addImportPath(":/qml");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    CommonDataBase &database = CommonDataBase::instance();
    engine.rootContext()->setContextProperty("database", &database);
    engine.load(url);

    return app.exec();
}
