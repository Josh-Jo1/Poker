#ifndef CARD_H
#define CARD_H
#include <string>
#include <vector>

// Types of ranks
enum class Rank { Two = 2, Three = 3, Four = 4, Five = 5, Six = 5, Seven = 7,
                    Eight = 8, Nine = 9, Ten = 10, Jack = 11, Queen = 12, King = 13, Ace = 14 };
// Vector of all ranks
const std::vector<Rank> RANKS { Rank::Two, Rank::Three, Rank::Four, Rank::Five, Rank::Six, Rank::Seven,
                                Rank::Eight, Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen, Rank::King, Rank::Ace };

// Types of suits
enum class Suit { Clubs, Diamonds, Hearts, Spades };
// Vector of all suits
const std::vector<Suit> SUITS { Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades };


class Card {
    Rank rank;
    Suit suit;

    std::string getRankString() const;
    std::string getSuitString() const;

    public:
    Card(Rank r, Suit s);
    Rank getRank() const;
    Suit getSuit() const;
    int getValue() const;
    std::string getCardString() const;
};

#endif
