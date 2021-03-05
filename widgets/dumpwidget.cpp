#include "dumpwidget.h"

DumpWidget::DumpWidget(const Dump& dump, QWidget *parent) : QWidget(parent) {
    auto vLayout = new QVBoxLayout;
    setLayout(vLayout);

    // vLayout->addWidget(new QLabel("DUMP"));

    auto cards = dump.GetCards();
    if (!cards.empty()) {
        CARD_COLOR prev_color = cards.begin()->GetColor();
        auto hLayout = new QHBoxLayout;
        hLayout->setAlignment(Qt::AlignLeft);

        for (auto card : cards) {
            if (card.GetColor() != prev_color) {
                vLayout->addLayout(hLayout);
                hLayout = new QHBoxLayout;
                hLayout->setAlignment(Qt::AlignLeft);
                prev_color = card.GetColor();
            }
            hLayout->addWidget(new CardWidget({card,{true,true}}, false));
        }
        vLayout->addLayout(hLayout);
    }
}
