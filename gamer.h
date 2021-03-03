#pragma once

#include <list>
#include <map>
#include <string>
#include "card.h"

using HAND = std::list<std::pair<Card, CardStatus>>;

class Gamer {
public:
    explicit Gamer(const std::string& name);

    void AddCard(Card card);
    size_t CardCount() const;
    void HintColor(CARD_COLOR color);
    void HintRank(uint8_t rank);
    std::string GetName() const;

    Card PlayCard(size_t card_no);

private:
    HAND _hand;
    std::string _name;
};
