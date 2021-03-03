#include "dump.h"

void Dump::AddCard(Card card) {
    _cards.insert(card);
}

const std::multiset<Card>& Dump::GetCards() const {
    return _cards;
}
