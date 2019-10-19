TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../muduo_study
LIBS += -pthread

SOURCES += main.cpp \
    epollpoller.cpp \
    socket.cpp \
    tcpserver.cpp \
    eventloop.cpp

HEADERS += \
    epollpoller.h \
    socket.h \
    tcpserver.h \
    eventloop.h
