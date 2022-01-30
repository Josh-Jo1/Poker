#ifndef ROUND_H
#define ROUND_H
#include "deckOfCards.h"
#include "player.h"
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using sharPlayer = std::shared_ptr<Player>;
using uniqCard = std::unique_ptr<Card>;

// Types of hands
enum class HandT { RoyalFlush, StraightFlush, FourOfAKind, FullHouse, Flush, Straight, ThreeOfAKind, TwoPair, OnePair, Highest };

class Round {
    std::shared_ptr<DeckOfCards> deck;
    std::vector<sharPlayer> players, activePlayers;
    double smallBlind, bigBlind, pot;
    std::map<sharPlayer, int> roundBets;
    std::vector<uniqCard> table;

    std::vector<const Card *> getTable() const;
    void blindBet(std::vector<sharPlayer>::const_iterator playerIt, std::string blind, double bet);
    void printVector(std::string name, std::vector<const Card *> cards) const;
    std::string HandTtoString(HandT handT) const;
    std::pair<HandT, std::vector<const Card *>> determineHand(std::vector<const Card *> allCards) const;
    int isHandBetter(std::pair<HandT, std::vector<const Card *>> h1, std::pair<HandT, std::vector<const Card *>> h2) const;

    public:
    Round(std::shared_ptr<DeckOfCards> deck, std::vector<sharPlayer> players, double blind);
    void assignRoles();
    void deal();
    bool collectBets(bool blind);
    void flop();         // first three cards
    void turn();         // fourth card
    void river();        // fifth card card
    void endRound();
};

#endif
