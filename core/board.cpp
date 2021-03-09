#include "board.h"

Board::Board() : _count_of_lives(MAX_NUMBER_OF_LIVES),
                 _count_of_hints(MAX_NUMBER_OF_HINTS),
                 _current_gamer_no(0),
                 _is_game_running(false) {
}

Board::Board(const std::vector<uint8_t>& data) {
    size_t it = 0;
    _count_of_stacked_cards[CARD_COLOR::BLUE] = data[it++];
    _count_of_stacked_cards[CARD_COLOR::GREEN] = data[it++];
    _count_of_stacked_cards[CARD_COLOR::RED] = data[it++];
    _count_of_stacked_cards[CARD_COLOR::WHITE] = data[it++];
    _count_of_stacked_cards[CARD_COLOR::YELLOW] = data[it++];

    _count_of_lives = data[it++];
    _count_of_hints = data[it++];

    _is_game_running = false;

    auto gamers_count = data[it++];
    for (size_t gamers_no = 0; gamers_no < gamers_count; ++gamers_no) {
        auto name_size = data[it++];
        AddGamer(std::string(data.begin()+it, data.begin()+it+name_size));
        it += name_size;
        auto card_count = data[it++];
        for (size_t card_no = 0; card_no < card_count; ++card_no) {
            CARD_COLOR color = static_cast<CARD_COLOR>(data[it++]);
            auto rank = data[it++];
            _gamers.back().AddCard({color, rank});
            if (data[it++]) {
                _gamers.back().HintColor(color);
            }
            if (data[it++]) {
                _gamers.back().HintRank(rank);
            }
        }
    }

    _current_gamer_no = data[it++];
    _deck = Deck(data[it++]);

    auto dump_size = data[it++];
    for (size_t card_no = 0; card_no < dump_size; ++card_no) {
        CARD_COLOR color = static_cast<CARD_COLOR>(data[it++]);
        auto rank = data[it++];
        _dump.AddCard({color, rank});
    }

    _is_game_running = data[it++];
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

size_t  Board::GetDeckSize() const {
    return _deck.GetSize();
}

const Dump&  Board::GetDump() const {
    return _dump;
}

const std::map<CARD_COLOR, uint8_t>& Board::GetCountOfStackedCards() const {
    return _count_of_stacked_cards;
}

bool Board::AddGamer(const std::string& name) {
    if (_is_game_running) {
        return false;
    }
    if (_gamers.size() == MAX_NUMBER_OF_GAMERS) {
        return false;
    }
    for (const auto& gamer : _gamers) {
        if (name == gamer.GetName()) {
            return false;
        }
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
    if (_count_of_lives < 2) {
        _count_of_lives = 0;
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

bool Board::IsAllStacked() const {
    return GetScore() == 25;
}


bool Board::PlayCard(size_t card_no) {
    if (!_is_game_running) {
        return false;
    }
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
        if (IsAllStacked()) {
            _is_game_running = false;
        }
        return true;
    } else {
        _dump.AddCard(card);
        TryToDecrementLives();
        return false;
    }
}

void Board::DumpCard(size_t card_no) {
    if (!_is_game_running) {
        return;
    }
    Gamer& current_gamer = _gamers[_current_gamer_no];
    NextGamer();
    _dump.AddCard(current_gamer.PlayCard(card_no));
    if (!_deck.IsEmpty()) {
        current_gamer.AddCard(_deck.GetNextCard());
    }
    TryToIncrementHints();
}

void Board::PromptColor(size_t gamer_no, CARD_COLOR color) {
    if (!_is_game_running) {
        return;
    }
    if (gamer_no < _gamers.size() && gamer_no != _current_gamer_no) {
        if (TryToDecrementHints()) {
            _gamers.at(gamer_no).HintColor(color);
        }
    }
    NextGamer();
}

void Board::PromptRank(size_t gamer_no, uint8_t rank) {
    if (!_is_game_running) {
        return;
    }
    if (gamer_no < _gamers.size() && gamer_no != _current_gamer_no) {
        if (TryToDecrementHints()) {
            _gamers.at(gamer_no).HintRank(rank);
        }
    }
    NextGamer();
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

std::vector<uint8_t> Board::ToByteArray() {
    std::vector<uint8_t> res;
    res.push_back(_count_of_stacked_cards[CARD_COLOR::BLUE]);
    res.push_back(_count_of_stacked_cards[CARD_COLOR::GREEN]);
    res.push_back(_count_of_stacked_cards[CARD_COLOR::RED]);
    res.push_back(_count_of_stacked_cards[CARD_COLOR::WHITE]);
    res.push_back(_count_of_stacked_cards[CARD_COLOR::YELLOW]);

    res.push_back(_count_of_lives);
    res.push_back(_count_of_hints);

    res.push_back(_gamers.size());
    for (auto gamer : _gamers) {
        auto name = gamer.GetName();
        res.push_back(name.size());
        res.insert(res.end(), name.begin(), name.end());
        auto cards = gamer.GetCards();
        res.push_back(cards.size());
        for (auto card : cards) {
            res.push_back(static_cast<uint8_t>(card.first.GetColor()));
            res.push_back(card.first.GetRank());
            res.push_back(card.second.is_known_color);
            res.push_back(card.second.is_known_rank);
        }
    }

    res.push_back(_current_gamer_no);
    res.push_back(_deck.GetSize());

    auto dump_cards = _dump.GetCards();
    res.push_back(dump_cards.size());
    for (auto card : dump_cards) {
        res.push_back(static_cast<uint8_t>(card.GetColor()));
        res.push_back(card.GetRank());
    }

    res.push_back(_is_game_running);

    return res;
}
