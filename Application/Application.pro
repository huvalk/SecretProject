QT += quick sql
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
        src/GraphicScene/graphicitem.cpp \
        src/GraphicScene/graphicline.cpp \
        src/GraphicScene/graphicpoint.cpp \
        src/GraphicScene/graphicscene.cpp \
        src/MapFileModel/mapfile.cpp \
        src/MapFileModel/mapfilemodel.cpp \
        src/MapFileModel/mapfilereader.cpp \
        main.cpp

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
    include/GraphicScene/graphicitem.h \
    include/GraphicScene/graphicline.h \
    include/GraphicScene/graphicpoint.h \
    include/GraphicScene/graphicscene.h \
    include/GraphicScene/graphictypes.h \
    include/MapFileModel/mapfile.h \
    include/MapFileModel/mapfilemodel.h \
    include/MapFileModel/mapfilereader.h

DISTFILES += \
    dependencies.pri
