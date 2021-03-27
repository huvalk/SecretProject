QT += quick sql network
CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/DataBase/commondatabase.cpp \
        src/DataBase/connectionmanager.cpp \
        src/GraphicScene/PathFinder/customqueue.cpp \
        src/GraphicScene/PathFinder/graphmanager.cpp \
        src/GraphicScene/PathFinder/listgraph.cpp \
        src/GraphicScene/PathFinder/logicline.cpp \
        src/GraphicScene/PathFinder/logicpoint.cpp \
        src/GraphicScene/PathFinder/pathfinder.cpp \
        src/GraphicScene/graphiccontainer.cpp \
        src/GraphicScene/Items/graphicimage.cpp \
        src/GraphicScene/Items/graphicitem.cpp \
        src/GraphicScene/Items/graphicline.cpp \
        src/GraphicScene/graphiccontructor.cpp \
        src/GraphicScene/graphicpainter.cpp \
        src/GraphicScene/Items/graphicpoint.cpp \
        src/GraphicScene/Items/graphicpolygon.cpp \
        src/GraphicScene/graphicscene.cpp \
        src/MapFileModel/mapfile.cpp \
        src/MapFileModel/mapfilemodel.cpp \
        src/MapFileModel/mapfilereader.cpp \
        main.cpp \
        src/MapFloorsModel/mapfloorsmodel.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/qml
QML2_IMPORT_PATH += $$PWD/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../shared/include

LIBS += -L$$PWD/../shared/lib/ -lDataBase

HEADERS += \
    include/DataBase/commondatabase.h \
    include/DataBase/connectionmanager.h \
    include/DataBase/dbtypes.h \
    include/GraphicScene/PathFinder/customqueue.h \
    include/GraphicScene/PathFinder/graphmanager.h \
    include/GraphicScene/PathFinder/graphtypes.h \
    include/GraphicScene/PathFinder/listgraph.h \
    include/GraphicScene/PathFinder/logicline.h \
    include/GraphicScene/PathFinder/logicpoint.h \
    include/GraphicScene/PathFinder/pathfinder.h \
    include/GraphicScene/graphicconstructor.h \
    include/GraphicScene/graphiccontainer.h \
    include/GraphicScene/Items/graphicimage.h \
    include/GraphicScene/Items/graphicitem.h \
    include/GraphicScene/Items/graphicline.h \
    include/GraphicScene/graphicpainter.h \
    include/GraphicScene/Items/graphicpoint.h \
    include/GraphicScene/Items/graphicpolygon.h \
    include/GraphicScene/graphicscene.h \
    include/GraphicScene/graphictypes.h \
    include/MapFileModel/mapfile.h \
    include/MapFileModel/mapfilemodel.h \
    include/MapFileModel/mapfilereader.h \
    include/MapFloorsModel/mapfloorsmodel.h \
    src/GraphicScene/PathFinder/logicline.h

DISTFILES += \
    dependencies.pri
