#include "card.h"

//
// Private
//

std::string Card::getRankString() const {
    switch (rank) {
        case Rank::Jack:
            return "J";
        case Rank::Queen:
            return "Q";
        case Rank::King:
            return "K";
        case Rank::Ace:
            return "A";
        default:
            return std::to_string((int)rank);
    }
}

std::string Card::getSuitString() const {
    switch (suit) {
        case Suit::Clubs:
            return "♣";
        case Suit::Diamonds:
            return "♦";
        case Suit::Hearts:
            return "♥";
        default:
            return "♠";
    }
}

//
// Public
//

Card::Card(Rank r, Suit s) : rank{r}, suit{s} {}

Rank Card::getRank() const {
    return rank;
}

Suit Card::getSuit() const {
    return suit;
}

int Card::getValue() const {
    return (int)rank;
}

std::string Card::getCardString() const {
    return getRankString() + getSuitString();
}
