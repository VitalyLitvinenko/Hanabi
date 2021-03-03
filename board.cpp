#include "board.h"

Board::Board() : _count_of_lives(MAX_NUMBER_OF_LIVES),
                 _count_of_hints(MAX_NUMBER_OF_HINTS),
                 _current_gamer_no(0),
                 _is_game_running(false) {
}

uint8_t Board::GetCountOfStackedCards(CARD_COLOR color) {
    return _count_of_stacked_cards[color];
}

uint8_t Board::GetCountOfLives() const {
    return _count_of_lives;
}

uint8_t Board::GetCountOfHints() const {
    return _count_of_hints;
}

bool Board::AddGemer(const std::string& name) {
    if (_gamers.size() == MAX_NUMBER_OF_GAMERS) {
        return false;
    }
    _gamers.emplace_back(name);
    return true;
}

const std::vector<Gamer>& Board::GetGamers() const {
    return _gamers;
}


bool Board::TryToIncrementLives() {
    if (_count_of_lives == MAX_NUMBER_OF_LIVES) {
        return false;
    }
    ++_count_of_lives;
    return true;
}

bool Board::TryToDecrementLives() {
    if (_count_of_lives == 0) {
        _is_game_running = false;
        return false;
    }
    --_count_of_lives;
    return true;
}

bool Board::TryToIncrementHints() {
    if (_count_of_hints == MAX_NUMBER_OF_HINTS) {
        return false;
    }
    ++_count_of_hints;
    return true;
}

bool Board::TryToDecrementHints() {
    if (_count_of_hints == 0) {
        return false;
    }
    --_count_of_hints;
    return true;
}


bool Board::PlayCard(size_t card_no) {
    Gamer& current_gamer = _gamers[_current_gamer_no];
    NextGamer();
    Card card = current_gamer.PlayCard(card_no);
    if (!_deck.IsEmpty()) {
        current_gamer.AddCard(_deck.GetNextCard());
    }
    if (card.GetRank() == _count_of_stacked_cards[card.GetColor()] + 1) {
        ++_count_of_stacked_cards[card.GetColor()];
        if (card.GetRank() == 5) {
            TryToIncrementHints();
        }
        return true;
    } else {
        _dump.AddCard(card);
        TryToDecrementLives();
        return false;
    }
}

void Board::DumpCart(size_t card_no) {
    Gamer& current_gamer = _gamers[_current_gamer_no];
    NextGamer();
    _dump.AddCard(current_gamer.PlayCard(card_no));
    if (!_deck.IsEmpty()) {
        current_gamer.AddCard(_deck.GetNextCard());
    }
    TryToIncrementHints();
}

void Board::PromptColor(size_t gamer_no, CARD_COLOR color) {
    NextGamer();
    if (gamer_no < _gamers.size() && gamer_no != _current_gamer_no) {
        if (TryToDecrementHints()) {
            _gamers.at(gamer_no).HintColor(color);
        }
    }
}

void Board::PromptRank(size_t gamer_no, uint8_t rank) {
    NextGamer();
    if (gamer_no < _gamers.size() && gamer_no != _current_gamer_no) {
        if (TryToDecrementHints()) {
            _gamers.at(gamer_no).HintRank(rank);
        }
    }
}

bool Board::TryToStartGame() {
    if (_gamers.size() < MIN_NUMBER_OF_GAMERS) {
        return false;
    }
    if (_is_game_running) {
        return false;
    }
    for (auto& gamer : _gamers) {
        for (size_t i = 0; i < CARDS_ON_HAND; ++i) {
            gamer.AddCard(_deck.GetNextCard());
        }
    }
    _is_game_running = true;
    return true;
}

bool Board::IsRunning() const {
    return _is_game_running;
}

size_t Board::GetScore() const {
    size_t score = 0;
    for (auto [color, count] : _count_of_stacked_cards) {
        score += count;
    }
    return score;
}

size_t Board::GetCurrentGamerNo() const {
    return _current_gamer_no;
}

void Board::NextGamer() {
    ++_current_gamer_no;
    _current_gamer_no %= _gamers.size();
    if (_gamers[_current_gamer_no].CardCount() < CARDS_ON_HAND) {
        _is_game_running = false;
    }
}
