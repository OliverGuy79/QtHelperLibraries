QT -= gui
QT += network quick

TEMPLATE = lib
DEFINES += CUTESIMPLERESTCLIENTLIB_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    restapimanager.cpp

HEADERS += \
    CuteSimpleRestClientLib_global.h \
    restapimanager.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
