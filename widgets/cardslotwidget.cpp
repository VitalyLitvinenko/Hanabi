#include "cardslotwidget.h"

CardSlotWidget::CardSlotWidget(const std::pair<Card, CardStatus>& card_with_status, bool is_other_gamer_view, QWidget *parent) : QWidget(parent) {
    card_layout = new QVBoxLayout;
    setLayout(card_layout);
    CardWidget *card;
    if (is_other_gamer_view) {
        card = new CardWidget({card_with_status.first, CardStatus({true, true})});
        card_layout->addWidget(new CardWidget({card_with_status.first, card_with_status.second}, false));
    } else {
        card = new CardWidget({card_with_status.first, card_with_status.second});
    }
    card_layout->addWidget(card);
    connect(card, SIGNAL(clicked()), this, SLOT(slotCardClicked()));
}

void CardSlotWidget::slotCardClicked() {
    auto card = (CardWidget*)sender();
    emit signalCardClicked(card->GetCard().GetColor(), card->GetCard().GetRank());
}
