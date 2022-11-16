QT += quick networkauth
 QT += webview multimedia
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += \
    qml.qrc


CONFIG += qmltypes
QML_IMPORT_NAME = backend
QML_IMPORT_MAJOR_VERSION = 1

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/../../QtHelperLibraries/QmlComponentLibrary

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CuteSimpleRestClientLib/release/ -lCuteSimpleRestClientLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CuteSimpleRestClientLib/debug/ -lCuteSimpleRestClientLib
else:unix: LIBS += -L$$OUT_PWD/../CuteSimpleRestClientLib/ -lCuteSimpleRestClientLib

INCLUDEPATH += $$PWD/../CuteSimpleRestClientLib
DEPENDPATH += $$PWD/../CuteSimpleRestClientLib

HEADERS += \
    $$PWD/../CuteSimpleRestClientLib/restapimanager.h \
    ../FirebaseWrapper/firebaseauthwrapper.h \
    ../QDotEnv/qdotenv.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FirebaseWrapper/release/ -lFirebaseWrapper
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FirebaseWrapper/debug/ -lFirebaseWrapper
else:unix: LIBS += -L$$OUT_PWD/../FirebaseWrapper/ -lFirebaseWrapper

INCLUDEPATH += $$PWD/../FirebaseWrapper
DEPENDPATH += $$PWD/../FirebaseWrapper


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QDotEnv/release/ -lQDotEnv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QDotEnv/debug/ -lQDotEnv
else:unix: LIBS += -L$$OUT_PWD/../QDotEnv/ -lQDotEnv

INCLUDEPATH += $$PWD/../QDotEnv
DEPENDPATH += $$PWD/../QDotEnv

DISTFILES += \
    Qml/Delegates/RecipeCard.qml
