#pragma once

#include <set>
#include "card.h"

class Dump {
public:
    Dump() = default;

    void AddCard(Card card);
    const std::multiset<Card>& GetCards() const;

private:
    std::multiset<Card> _cards;
};
