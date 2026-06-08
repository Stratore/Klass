QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    absence.cpp \
    data_store.cpp \
    main.cpp \
    mainwindow.cpp \
    student.cpp
HEADERS += \
    absence.h \
    data_store.h \
    mainwindow.h \
    student.h
FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Klass_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/default.jpg \
    images/image_1.jpg \
    images/image_1.png \
    images/image_10.jpg \
    images/image_10.png \
    images/image_11.jpg \
    images/image_11.png \
    images/image_12.jpg \
    images/image_12.png \
    images/image_13png \
    images/image_14.jpg \
    images/image_14.png \
    images/image_15.jpg \
    images/image_15.png \
    images/image_16.jpg \
    images/image_16.png \
    images/image_2.jpg \
    images/image_2.png \
    images/image_3.jpg \
    images/image_3.png \
    images/image_4.jpg \
    images/image_4.png \
    images/image_5.jpg \
    images/image_5.png \
    images/image_6.jpg \
    images/image_6.png \
    images/image_7.jpg \
    images/image_7.png \
    images/image_8.jpg \
    images/image_8.png \
    images/image_9.jpg \
    images/image_9.png
