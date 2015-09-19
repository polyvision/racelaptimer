#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T10:44:29
#
#-------------------------------------------------

QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RaceLapTimer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rltdatabase.cpp \
    currentrace.cpp \
    racepilot.cpp \
    currentpilotracelap.cpp \
    qextserialport/qextserialenumerator.cpp \
    qextserialport/qextserialport.cpp \
    sfx.cpp \
    settings.cpp \
    racetablewidget.cpp \
    modelrace.cpp \
    currentracewidget.cpp \
    pilotswidget.cpp \
    configurationwidget.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    rltdatabase.h \
    currentrace.h \
    racepilot.h \
    singleton.h \
    currentpilotracelap.h \
    qextserialport/qextserialenumerator.h \
    qextserialport/qextserialenumerator_p.h \
    qextserialport/qextserialport.h \
    qextserialport/qextserialport_global.h \
    qextserialport/qextserialport_p.h \
    sfx.h \
    settings.h \
    racetablewidget.h \
    modelrace.h \
    currentracewidget.h \
    pilotswidget.h \
    configurationwidget.h \
    aboutdialog.h

ICON = app_icon.png

macx {
  SOURCES += qextserialport/qextserialenumerator_osx.cpp \
            qextserialport/qextserialport_unix.cpp
}

win32 {
  RC_ICONS = appicon.ico

  SOURCES += qextserialport/qextserialenumerator_win.cpp \
      qextserialport/qextserialport_win.cpp \
}
FORMS    += mainwindow.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

win32:LIBS             += -lsetupapi -ladvapi32 -luser32
macx:LIBS              += -framework IOKit -framework CoreFoundation
