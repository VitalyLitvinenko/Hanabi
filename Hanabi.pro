QT -= gui

CONFIG += c++17 console debug
CONFIG -= app_bundle

TEMPLATE = app
TARGET = Hanabi

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        board.cpp \
        card.cpp \
        deck.cpp \
        dump.cpp \
        gamer.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    board.h \
    card.h \
    deck.h \
    dump.h \
    gamer.h
