#pragma once

#include <vector>
#include <map>
#include "deck.h"
#include "dump.h"
#include "gamer.h"

class Board {
private:
    static constexpr uint8_t MAX_NUMBER_OF_LIVES = 3;
    static constexpr uint8_t MAX_NUMBER_OF_HINTS = 8;
    static constexpr size_t MIN_NUMBER_OF_GAMERS = 2;
    static constexpr size_t MAX_NUMBER_OF_GAMERS = 5;
    static constexpr size_t CARDS_ON_HAND = 5;

public:
    Board();

    uint8_t GetCountOfStackedCards(CARD_COLOR color);
    uint8_t GetCountOfLives() const;
    uint8_t GetCountOfHints() const;

    bool AddGemer(const std::string& name);
    const std::vector<Gamer>& GetGamers() const;


    bool PlayCard(size_t card_no);
    void DumpCart(size_t card_no);
    void PromptColor(size_t gamer_no, CARD_COLOR color);
    void PromptRank(size_t gamer_no, uint8_t rank);


    bool TryToStartGame();
    bool IsRunning() const;
    size_t GetScore() const;
    size_t GetCurrentGamerNo() const;

private:
    bool TryToIncrementLives();
    bool TryToDecrementLives();
    bool TryToIncrementHints();
    bool TryToDecrementHints();

    void NextGamer();


private:
    std::map<CARD_COLOR, uint8_t> _count_of_stacked_cards;
    uint8_t _count_of_lives;
    uint8_t _count_of_hints;

    std::vector<Gamer> _gamers;
    size_t _current_gamer_no;

    Deck _deck;
    Dump _dump;

    bool _is_game_running;
};
