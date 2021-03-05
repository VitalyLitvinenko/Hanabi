#include "stackwidget.h"

StackWidget::StackWidget(size_t size, CARD_COLOR color, QWidget *parent) : DeckWidget(size, parent) {
    QString background_color = "grey";
    switch (color) {
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
    setStyleSheet("background-color:" + background_color + ";");
}
