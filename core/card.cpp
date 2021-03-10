#include "card.h"
#include <tuple>

std::string ColorToString(CARD_COLOR color) {
    switch (color) {
    case CARD_COLOR::BLUE:   return "blue";
    case CARD_COLOR::GREEN:  return "green";
    case CARD_COLOR::RED:    return "red";
    case CARD_COLOR::WHITE:  return "white";
    case CARD_COLOR::YELLOW: return "yellow";
    }
    return "";
}

Card::Card(CARD_COLOR color, uint8_t rank) :
    _color(color), _rank(rank) {}

CARD_COLOR Card::GetColor() const {
    return _color;
}

uint8_t Card::GetRank() const {
    return _rank;
}

bool Card::operator<(const Card& rhs) const {
    auto rhs_color = rhs.GetColor();
    auto rhs_rank = rhs.GetRank();
    return std::tie(_color, _rank) < std::tie(rhs_color, rhs_rank);
}

bool Card::operator==(const Card& rhs) const {
    return rhs.GetColor() == _color && rhs.GetRank() == _rank;
}
