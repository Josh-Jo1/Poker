#ifndef DECKOFCARDS_H
#define DECKOFCARDS_H
#include "card.h"
#include <memory>
#include <random>

using uniqCard = std::unique_ptr<Card>;

class DeckOfCards {
    std::mt19937 rgen;
    std::vector<uniqCard> deck;

    void initDeck();
    
    public:
    DeckOfCards();
    DeckOfCards(unsigned int seed);
    void shuffle();
    uniqCard getTopCard();
    void returnCard(uniqCard card);
};

#endif
