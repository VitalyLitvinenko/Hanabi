#include "gamer.h"

Gamer::Gamer(const std::string& name) : _name(name) {
}

void Gamer::AddCard(Card card) {
    _hand.emplace_back(card, CardStatus{false, false});
}

size_t Gamer::CardCount() const {
    return _hand.size();
}

void Gamer::HintColor(CARD_COLOR color) {
    for (auto& card_status : _hand) {
        if (card_status.first.GetColor() == color) {
            card_status.second.is_known_color = true;
        }
    }
}

void Gamer::HintRank(uint8_t rank) {
    for (auto& card_status : _hand) {
        if (card_status.first.GetRank() == rank) {
            card_status.second.is_known_rank = true;
        }
    }
}

std::string Gamer::GetName() const {
    return _name;
}

const HAND& Gamer::GetCards() const {
    return _hand;
}

Card Gamer::PlayCard(size_t card_no) {
    if (card_no >= _hand.size()) {
        // TODO: add exception
        return {CARD_COLOR::WHITE, 0};
    }
    auto card_it = std::next(_hand.begin(), card_no);
    Card result = card_it->first;
    _hand.erase(card_it);
    return result;
}
