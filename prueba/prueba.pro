QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        fdisk.cpp \
        lexico.cpp \
        main.cpp \
        mkdisk.cpp \
        rmdisk.cpp \
        sintactico.cpp \
    exec.cpp \
    ejecutar.cpp \
    mount.cpp \
    rep.cpp \
    listamount.cpp \
    nodolist.cpp \
    graficas.cpp \
    mkfs.cpp \
    login.cpp \
    mkgrp.cpp \
    mkusr.cpp \
    mkdir.cpp \
    mkfile.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Structs.h \
    fdisk.h \
    lexico.h \
    main.h \
    mkdisk.h \
    rmdisk.h \
    sintactico.h \
    exec.h \
    ejecutar.h \
    mount.h \
    rep.h \
    listamount.h \
    nodolist.h \
    graficas.h \
    mkfs.h \
    login.h \
    mkgrp.h \
    mkusr.h \
    mkdir.h \
    mkfile.h
