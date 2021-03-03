#pragma once

#include <cinttypes>

enum class CARD_COLOR : char {
    WHITE,
    YELLOW,
    GREEN,
    BLUE,
    RED
};

class Card {
public:
    Card(CARD_COLOR color, uint8_t _rank);

    CARD_COLOR GetColor() const;
    uint8_t GetRank() const;

    bool operator<(const Card&) const;
    bool operator==(const Card&) const;

private:
    CARD_COLOR _color;
    uint8_t _rank;
};

struct CardStatus {
    bool is_known_color = false;
    bool is_known_rank  = false;
};
