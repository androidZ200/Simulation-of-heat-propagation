QT       += core gui opengl

win32 {
    QT +=  openglwidgets
    LIBS += -lopengl32 -lglu32
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Schemes/cranknicholsonscheme.cpp \
    Schemes/explictschema.cpp \
    GlPlot/glcomplexplot.cpp \
    GlPlot/glplot.cpp \
    GlPlot/glrealplot.cpp \
    Collectors/horizontalcollector.cpp \
    Schemes/implicitschema.cpp \
    Schemes/isolver.cpp \
    loadwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    showwindow.cpp \
    Collectors/verticalcollector.cpp

HEADERS += \
    Schemes/cranknicholsonscheme.h \
    Schemes/explictschema.h \
    GlPlot/glcomplexplot.h \
    GlPlot/glplot.h \
    GlPlot/glrealplot.h \
    Collectors/horizontalcollector.h \
    Schemes/implicitschema.h \
    Schemes/isolver.h \
    loadwindow.h \
    mainwindow.h \
    showwindow.h \
    Collectors/verticalcollector.h

FORMS += \
    loadwindow.ui \
    mainwindow.ui \
    showwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
