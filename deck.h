#pragma once

#include <vector>
#include "card.h"


class Deck {
public:
    Deck();

    bool IsEmpty() const;
    size_t GetSize() const;
    Card GetNextCard();

private:
    void Shuffle();

private:
    std::vector<Card> _deck;
};
