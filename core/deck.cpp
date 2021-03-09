#include "deck.h"

#include <random>
#include <chrono>
#include <algorithm>

Deck::Deck() {
    std::vector<CARD_COLOR> colors = {CARD_COLOR::WHITE,
                                      CARD_COLOR::YELLOW,
                                      CARD_COLOR::GREEN,
                                      CARD_COLOR::BLUE,
                                      CARD_COLOR::RED};
    std::vector<uint8_t> ranks = {1,1,1,2,2,3,3,4,4,5};
    for (auto color : colors) {
        for (auto rank : ranks) {
            _deck.emplace_back(color, rank);
        }
    }
    Shuffle();
}

Deck::Deck(uint8_t size) {
    _deck.resize(size, {CARD_COLOR::WHITE, 0});
}

bool Deck::IsEmpty() const {
    return _deck.empty();
}

size_t Deck::GetSize() const {
    return _deck.size();
}

Card Deck::GetNextCard() {
    auto card = _deck.back();
    _deck.pop_back();
    return card;
}

void Deck::Shuffle() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(_deck.begin(), _deck.end(), std::default_random_engine(seed));
}
