#ifndef ODDSCALC_H
#define ODDSCALC_H
#include "card.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

using sharCard = std::shared_ptr<Card>;

class OddsCalc {
    int deckSize, numRemainingCards;
    std::map<Rank, int> ranksLeft { {Rank::Two, 4}, {Rank::Three, 4}, {Rank::Four, 4}, {Rank::Five, 4}, {Rank::Six, 4},
                                    {Rank::Seven, 4}, {Rank::Eight, 4}, {Rank::Nine, 4}, {Rank::Ten, 4}, {Rank::Jack, 4},
                                    {Rank::Queen, 4}, {Rank::King, 4}, {Rank::Ace, 4} };
    std::map<Suit, int> suitsLeft { {Suit::Clubs, 13}, {Suit::Diamonds, 13}, {Suit::Hearts, 13}, {Suit::Spades, 13} };
    std::vector<sharCard> visibleCards;
    std::map<int, double> factorialMemo;
    
    sharCard createCard(std::string rank, std::string suit) const;
    void getCards(std::string stage);

    double factorial(int n);
    double nCr(int n, int r);
    double exactlyProb(int startPossibilities, int success);
    void determineOdds(bool handOnly);
    void calcStage();

    public:
    OddsCalc();
    void setHand();
    void calcFlop();
    void calcTurn();
};

#endif
