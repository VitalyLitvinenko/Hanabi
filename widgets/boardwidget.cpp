#include "boardwidget.h"

BoardWidget::BoardWidget(QWidget *parent) : QWidget(parent), main_layout(nullptr) {
}

void BoardWidget::slotCardClicked(int card_no, CARD_COLOR color, uint8_t rank) {
    auto gamer_no = players_layout->indexOf((GamerWidget*)sender());

    _card_no = card_no;
    _color = color;
    _rank = rank;
    _gamer_no = gamer_no;
    bool is_current_gamer = static_cast<int>(_board.GetCurrentGamerNo()) == gamer_no;
    bool is_active_gamer = _current_gamer_no == _board.GetCurrentGamerNo();
    play_button->setEnabled(is_current_gamer && is_active_gamer);
    dump_button->setEnabled(is_current_gamer && is_active_gamer);
    hint_color_button->setDisabled(is_current_gamer || _board.GetCountOfHints() == 0 || !is_active_gamer);
    hint_rank_button->setDisabled(is_current_gamer || _board.GetCountOfHints() == 0 || !is_active_gamer);
}

void BoardWidget::slotPlay() {
    emit signalPlayCard(_card_no);
}

void BoardWidget::slotDump() {
    emit signalDumpCard(_card_no);
}

void BoardWidget::slotHintColor() {
    emit signalHintColor(_gamer_no, _color);
}

void BoardWidget::slotHintRank() {
    emit signalHintRank(_gamer_no, _rank);
}

void BoardWidget::Refresh(const Board& board, size_t current_gamer_no) {
    _board = board;
    _current_gamer_no = current_gamer_no;
    if (main_layout != nullptr) {
        qDeleteAll(findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
        delete main_layout;
        main_layout = nullptr;
    }
    main_layout = new QHBoxLayout;

    players_layout = new QVBoxLayout;
    const auto& gamers = board.GetGamers();
    for (size_t gamer_no = 0; gamer_no < gamers.size(); ++gamer_no) {
        auto gamer_w = new GamerWidget(gamers[gamer_no],
                                       current_gamer_no != gamer_no,
                                       board.GetCurrentGamerNo() != gamer_no);
        players_layout->addWidget(gamer_w);
        connect(gamer_w, SIGNAL(signalActiveCard(int, CARD_COLOR, uint8_t)), this, SLOT(slotCardClicked(int, CARD_COLOR, uint8_t)));
    }
    main_layout->addLayout(players_layout);

    auto stacks_layout = new QHBoxLayout;
    for (const auto& [color, count] : board.GetCountOfStackedCards()) {
        if (count > 0) {
            stacks_layout->addWidget(new StackWidget(count, color));
        }
    }

    auto info_layout = new QVBoxLayout;
    info_layout->addWidget(new QLabel(QString("<H1>Lives: ") + QString::number(board.GetCountOfLives()) + "</H1>"));
    info_layout->addWidget(new QLabel(QString("<H1>Hints: ") + QString::number(board.GetCountOfHints()) + "</H1>"));
    info_layout->addWidget(new DeckWidget(board.GetDeckSize()));
    info_layout->addLayout(stacks_layout);
    info_layout->addWidget(new DumpWidget(board.GetDump()));
    info_layout->setStretch(2, 1);
    info_layout->setStretch(3, 1);
    info_layout->setStretch(4, 1);

    play_button = new QPushButton("play");
    play_button->setEnabled(false);
    dump_button = new QPushButton("fold");
    dump_button->setEnabled(false);
    hint_color_button = new QPushButton("hint color");
    hint_color_button->setEnabled(false);
    hint_rank_button = new QPushButton("hint rank");
    hint_rank_button->setEnabled(false);

    // action buttons only for gamers
    if (current_gamer_no < board.GetGamers().size()) {
        info_layout->addWidget(play_button);
        connect(play_button, SIGNAL(clicked()), this, SLOT(slotPlay()));
        info_layout->addWidget(dump_button);
        connect(dump_button, SIGNAL(clicked()), this, SLOT(slotDump()));
        info_layout->addWidget(hint_color_button);
        connect(hint_color_button, SIGNAL(clicked()), this, SLOT(slotHintColor()));
        info_layout->addWidget(hint_rank_button);
        connect(hint_rank_button, SIGNAL(clicked()), this, SLOT(slotHintRank()));
    }

    main_layout->addLayout(info_layout);

    setLayout(main_layout);

    if (!board.IsRunning()) {
        QMessageBox ms_box;
        ms_box.setText("Your score: " + QString::number(board.GetScore()));
        ms_box.exec();
        return;
    }

}
