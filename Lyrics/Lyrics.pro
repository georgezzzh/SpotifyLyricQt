QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#win32API支持
win32{
    LIBS += -luser32
}
# 中英文支持
TRANSLATIONS = Translation_EN.ts Translation_CN.ts
# 编译出的exe带有图标
RC_ICONS += exeIcon.ico
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
    client.cpp \
    desklyric.cpp \
    main.cpp \
    mainwindow.cpp \
    mylabel.cpp \
    mytextbrowser.cpp \
    neteasecloudmusicclient.cpp \
    qqmusicclient.cpp \
    settings.cpp \
    spotify.cpp

HEADERS += \
    client.h \
    desklyric.h \
    mainwindow.h \
    mylabel.h \
    mytextbrowser.h \
    neteasecloudmusicclient.h \
    qqmusicclient.h \
    settings.h \
    spotify.h

FORMS += \
    mainwindow.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/resource.qrc
