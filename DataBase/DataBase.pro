QT -= gui
QT += core sql

CONFIG += c++17
TEMPLATE = lib

DESTDIR = $$PWD/../shared/lib
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/connectionmanager.cpp \
    src/executor.cpp \
    src/processor.cpp \
    src/selector.cpp

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/connectionmanager.h \
    include/dbmapper.h \
    include/dbtypes.h \
    include/executor.h \
    include/processor.h \
    include/selector.h

DISTFILES += \
    dependencies.pri

headers.path = $$PWD/../shared/include
headers.files = include/dbtypes.h \
                include/processor.h

INSTALLS += headers
