#include "cardwidget.h"

CardWidget::CardWidget(std::pair<Card, CardStatus> card_with_status, bool is_big_card, QWidget *parent)
    : QPushButton(parent), _card_with_status(card_with_status) {
    RefreshStatus();
    //setAlignment(Qt::AlignCenter);

    if (is_big_card) {
        setFont(QFont("Arial",50));
        setFixedSize(70,100);
    } else {
        setFont(QFont("Arial",10));
        setFixedSize(14,20);
    }
}

void CardWidget::SetStatus(CardStatus new_card_status) {
    _card_with_status.second = new_card_status;
    RefreshStatus();
}

void CardWidget::RefreshStatus() {
    QString background_color = "grey";
    if (_card_with_status.second.is_known_color) {
        switch (_card_with_status.first.GetColor()) {
        case CARD_COLOR::WHITE:
            background_color = "white";
            break;
        case CARD_COLOR::YELLOW:
            background_color = "yellow";
            break;
        case CARD_COLOR::GREEN:
            background_color = "green";
            break;
        case CARD_COLOR::BLUE:
            background_color = "blue";
            break;
        case CARD_COLOR::RED:
            background_color = "red";
            break;
        default:
            break;
        }
    }
    setStyleSheet("background-color:" + background_color + ";");

    if (_card_with_status.second.is_known_rank) {
        setText(QString::number(_card_with_status.first.GetRank()));
    } else {
        setText("");
    }
}

Card CardWidget::GetCard() const {
    return _card_with_status.first;
}
