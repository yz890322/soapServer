TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    recvnetrequest.cpp \
    mainclass.cpp \
    facecompservice.cpp \
    stdsoap2.cpp \
    soapServer.cpp \
    soapC.cpp

HEADERS += \
    recvnetrequest.h \
    mainclass.h \
    facecompservice.h \
    stdsoap2.h \
    soapStub.h \
    soapH.h \
    packdata.h

OTHER_FILES += \
    myWebServer.pro.user

