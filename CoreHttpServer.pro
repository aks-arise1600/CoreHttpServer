QT += core network
QT -= gui

CONFIG += c++17

TARGET = CoreHttpServer
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    HTTP_Handler.cpp


HEADERS += \
    httplib.h \
    HTTP_Handler.h \
    htmls.h
    
DEFINES += CPPHTTPLIB_OPENSSL_SUPPORT

RESOURCES += \
    resources.qrc
    
LIBS += -lssl -lcrypto
