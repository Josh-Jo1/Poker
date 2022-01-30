#include "deckOfCards.h"
#include <algorithm>

//
// Private
//

void DeckOfCards::initDeck() {
    // Initialize deck
    for (auto &rank : RANKS) {
        for (auto &suit : SUITS) {
            deck.push_back(std::make_unique<Card>(rank, suit));
        }
    }
}

//
// Public
//

DeckOfCards::DeckOfCards() {
    initDeck();
    // Initialize rgen (True Random Number Generator)
    std::random_device rd;
    rgen = std::mt19937{rd()};
}

DeckOfCards::DeckOfCards(unsigned int seed) {
    initDeck();
    // Initialize rgen (Pseudo-Random Number Generator)
    rgen = std::mt19937{seed};
}

void DeckOfCards::shuffle() {
    std::shuffle(deck.begin(), deck.end(), rgen);
}

uniqCard DeckOfCards::getTopCard() {
    uniqCard card = std::move(deck.back());
    deck.pop_back();
    return card;
}

void DeckOfCards::returnCard(uniqCard card) {
    deck.push_back(std::move(card));
}
