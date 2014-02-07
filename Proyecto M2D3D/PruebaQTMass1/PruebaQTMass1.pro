#-------------------------------------------------
#
# Project created by QtCreator 2014-01-10T10:25:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = PruebaQTMass1
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    qcustomplot.cpp \
    table.cpp \
    thread.cpp \
    zoomgraph.cpp

HEADERS  += dialog.h \
    qcustomplot.h \
    table.h \
    structures.h \
    thread.h \
    main.h \
    zoomgraph.h

FORMS    += dialog.ui \
    table.ui \
    zoomgraph.ui

RESOURCES += \
    Resources.qrc

LIBS += "D:\Users\gcucho\Desktop\MASS PC\Proyecto M2D3D\Release\release\Mass2d3d.dll"

RC_FILE = myapp.rc
