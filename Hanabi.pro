QT += core gui

CONFIG += c++17 console debug
CONFIG -= app_bundle

TEMPLATE = app
TARGET = Hanabi

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        widgets/boardwidget.cpp \
        widgets/cardslotwidget.cpp \
        widgets/cardwidget.cpp \
        core/board.cpp \
        core/card.cpp \
        core/deck.cpp \
        core/dump.cpp \
        core/gamer.cpp \
        main.cpp \
        widgets/deckwidget.cpp \
        widgets/dumpwidget.cpp \
        widgets/gamerwidget.cpp \
        widgets/stackwidget.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    widgets/boardwidget.h \
    widgets/cardslotwidget.h \
    widgets/cardwidget.h \
    core/board.h \
    core/card.h \
    core/deck.h \
    core/dump.h \
    core/gamer.h \
    widgets/deckwidget.h \
    widgets/dumpwidget.h \
    widgets/gamerwidget.h \
    widgets/stackwidget.h

QT += widgets
