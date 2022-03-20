QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    Train.cpp \
    inmovement.cpp \
    main.cpp \
    mainwindow.cpp \
    outmovement.cpp \
    station.cpp \
    timeclock.cpp \
    traingenerator.cpp

HEADERS += \
    Train.h \
    inmovement.h \
    mainwindow.h \
    outmovement.h \
    station.h \
    timeclock.h \
    traingenerator.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icon.qrc \
    Images.qrc
