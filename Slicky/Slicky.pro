QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customizestatuswidget.cpp \
    lightdevice.cpp \
    main.cpp \
    mainwindow.cpp \
    simplecrypt.cpp \
    slackdialog.cpp \
    status.cpp

HEADERS += \
    customizestatuswidget.h \
    hidapi.h \
    lightdevice.h \
    mainwindow.h \
    simplecrypt.h \
    slackdialog.h \
    status.h

FORMS += \
    customizestatuswidget.ui \
    mainwindow.ui \
    slackdialog.ui

ICON = Slicky.icns
#RC_ICONS = Slicky.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

macx: SOURCES += mac/hid.c
unix: !macx: SOURCES += linux/hid-libusb.c
win32: SOURCES += windows/hid.c

macx: LIBS += -framework CoreFoundation -framework IOkit
unix: !macx: LIBS += -lusb-1.0
win32: LIBS += -lSetupAPI

RESOURCES += \
    resource.qrc

unix: CONFIG += link_pkgconfig
