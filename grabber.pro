TARGET = grabber
TEMPLATE = app
QT += core dbus gui

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    common/DataIndex.cpp \
    common/Dictionary.cpp \
    common/Elm.cpp \
    common/libdecompress.cpp \
    main.cpp \
    Grabber.cpp

DESTDIR = ./output/
OBJECTS_DIR = ./output/obj/
MOC_DIR = ./output/moc/
RCC_DIR = ./output/rcc/

HEADERS += \
    common/DataIndex.hpp \
    common/Dictionary.hpp \
    common/Elm.hpp \
    common/types.hpp \
    Grabber.hpp

INCLUDEPATH += \
    common/\

FORMS += \
    MainWindow.ui


