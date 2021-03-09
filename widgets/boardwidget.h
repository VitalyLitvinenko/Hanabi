#pragma once

#include "../core/board.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include "gamerwidget.h"
#include "deckwidget.h"
#include "dumpwidget.h"
#include "stackwidget.h"

class BoardWidget : public QWidget
{
    Q_OBJECT

private:
    Board _board;
    size_t _current_gamer_no;

    int _card_no;
    CARD_COLOR _color;
    uint8_t _rank;
    int _gamer_no;

    QPushButton *play_button;
    QPushButton *dump_button;
    QPushButton *hint_color_button;
    QPushButton *hint_rank_button;

    QVBoxLayout *players_layout;
    QHBoxLayout *main_layout;

public:
    explicit BoardWidget(QWidget *parent = nullptr);

    void Refresh(const Board& board, size_t current_gamer_no = -1);

public slots:
    void slotCardClicked(int card_no, CARD_COLOR color, uint8_t rank);
    void slotPlay();
    void slotDump();
    void slotHintColor();
    void slotHintRank();

signals:
    void signalPlayCard(int card_no);
    void signalDumpCard(int card_no);
    void signalHintColor(int gamer_no, CARD_COLOR color);
    void signalHintRank(int gamer_no, uint8_t rank);
};
