QT += gui
QT += network networkauth quick
TEMPLATE = lib
DEFINES += FIREBASEWRAPPER_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    firebaseauthwrapper.cpp \
    firestorewrapper.cpp \
    googleauth.cpp

HEADERS += \
    FirebaseWrapper_global.h \
    firebaseauthwrapper.h \
    firestorewrapper.h \
    googleauth.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CuteSimpleRestClientLib/release/ -lCuteSimpleRestClientLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CuteSimpleRestClientLib/debug/ -lCuteSimpleRestClientLib
else:unix: LIBS += -L$$OUT_PWD/../CuteSimpleRestClientLib/ -lCuteSimpleRestClientLib

INCLUDEPATH += $$PWD/../CuteSimpleRestClientLib
DEPENDPATH += $$PWD/../CuteSimpleRestClientLib
