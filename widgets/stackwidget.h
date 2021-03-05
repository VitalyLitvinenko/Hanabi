#pragma once

#include "../core/card.h"

#include <QWidget>
#include "deckwidget.h"

class StackWidget : public DeckWidget {
    Q_OBJECT
public:
    explicit StackWidget(size_t size, CARD_COLOR color, QWidget *parent = nullptr);
};
