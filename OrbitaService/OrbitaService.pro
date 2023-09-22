QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    cashdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    orderdialog.cpp \
    ordereditdialog.cpp \
    qsqldbhelper.cpp \
    searchdialog.cpp

HEADERS += \
    aboutdialog.h \
    cashdialog.h \
    mainwindow.h \
    orderdialog.h \
    ordereditdialog.h \
    qsqldbhelper.h \
    searchdialog.h

FORMS += \
    aboutdialog.ui \
    cashdialog.ui \
    mainwindow.ui \
    orderdialog.ui \
    ordereditdialog.ui \
    searchdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

RC_ICONS = icon.ico
