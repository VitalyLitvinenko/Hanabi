#include <QApplication>

#include "core/board.h"
#include <iostream>

#include "widgets/cardwidget.h"
#include "widgets/deckwidget.h"
#include "widgets/dumpwidget.h"
#include "widgets/gamerwidget.h"
#include "widgets/boardwidget.h"
#include <QtWidgets>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Board board;
    board.AddGemer("Stas");
    board.AddGemer("Olga");
    board.AddGemer("Kirill");
    board.TryToStartGame();

    BoardWidget game(board);

    game.show();
    return app.exec();
}
