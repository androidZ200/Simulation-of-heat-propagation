QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cranknicholsonscheme.cpp \
    explictschema.cpp \
    horizontalcollector.cpp \
    implicitschema.cpp \
    isolver.cpp \
    loadwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    plot.cpp \
    showwindow.cpp \
    verticalcollector.cpp

HEADERS += \
    cranknicholsonscheme.h \
    explictschema.h \
    horizontalcollector.h \
    implicitschema.h \
    isolver.h \
    loadwindow.h \
    mainwindow.h \
    plot.h \
    showwindow.h \
    verticalcollector.h

FORMS += \
    loadwindow.ui \
    mainwindow.ui \
    showwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
