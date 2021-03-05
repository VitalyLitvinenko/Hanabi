#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "../core/card.h"

class CardWidget : public QPushButton {
    Q_OBJECT

private:
    std::pair<Card, CardStatus> _card_with_status;

public:
    explicit CardWidget(std::pair<Card, CardStatus> card_with_status, bool is_big_card = true, QWidget *parent = nullptr);

    void SetStatus(CardStatus new_card_status);
    Card GetCard() const;

private slots:
    void RefreshStatus();
};
