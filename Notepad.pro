QT       += core gui core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutnotepaddialog.cpp \
    filehelper.cpp \
    findorreplacedialog.cpp \
    main.cpp \
    notepadwindow.cpp \
    notepadmanager.cpp \
    textfiledialog.cpp

HEADERS += \
    aboutnotepaddialog.h \
    constsdefine.h \
    filehelper.h \
    fileinfo.h \
    findorreplacedialog.h \
    notepadwindow.h \
    notepadmanager.h \
    textfiledialog.h

FORMS += \
    aboutnotepaddialog.ui \
    findorreplacedialog.ui \
    notepadwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
