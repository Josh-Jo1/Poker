#ifndef MATCH_H
#define MATCH_H
#include "deckOfCards.h"
#include "player.h"
#include <string>
#include <vector>

using sharPlayer = std::shared_ptr<Player>;

class Match {
    std::shared_ptr<DeckOfCards> deck;
    std::vector<sharPlayer> players;
    double matchBuyIn, bigBlind;
    
    public:
    Match(double buyIn, double bigBlind, std::string names);
    void setSeed(unsigned int seed);
    void newRound();
    void addPlayers(std::string names);
    void removePlayers(std::string names);
    void addAmount(std::string names);
    void endGame();
};

#endif
