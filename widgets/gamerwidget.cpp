#include "gamerwidget.h"

GamerWidget::GamerWidget(const Gamer& gamer, bool is_other_gamer_view, bool is_other_gamer_do, QWidget *parent)
    : QFrame(parent) {
    auto vLayout = new QVBoxLayout;
    setLayout(vLayout);

    setFrameStyle(is_other_gamer_do ? 0 : 3);

    auto name_w = new QLabel("<H1>" + QString::fromUtf8(gamer.GetName().c_str()) + "</H1>");
    name_w->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    vLayout->addWidget(name_w);

    auto cards_with_statuses = gamer.GetCards();
    if (!cards_with_statuses.empty()) {
        hLayout = new QHBoxLayout;
        for (auto card_with_status : cards_with_statuses) {
            auto cardslot = new CardSlotWidget(card_with_status, is_other_gamer_view);
            hLayout->addWidget(cardslot);
            connect(cardslot, SIGNAL(signalCardClicked(CARD_COLOR, uint8_t)), this, SLOT(slotCardClicked(CARD_COLOR, uint8_t)));
        }
        vLayout->addLayout(hLayout);
    }
}

void GamerWidget::slotCardClicked(CARD_COLOR color, uint8_t rank) {
    auto card_slot = (CardWidget*)sender();
    int card_no = hLayout->indexOf(card_slot);
    emit signalActiveCard(card_no, color, rank);
}
