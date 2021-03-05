#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include "cardwidget.h"

class CardSlotWidget : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout *card_layout;

public:
    explicit CardSlotWidget(const std::pair<Card, CardStatus>& card_with_status, bool is_other_gamer_view, QWidget *parent = nullptr);

public slots:
    void slotCardClicked();

signals:
    void signalCardClicked(CARD_COLOR, uint8_t);
};
