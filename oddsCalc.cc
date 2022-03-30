#include "oddsCalc.h"
#include <iostream>
#include <sstream>
#include <algorithm>

//
// Private
//

sharCard OddsCalc::createCard(std::string rank, std::string suit) const {
    // Cannot use switch statements because of string and cannot change to char because of 10

    Rank convertedRank;
    if (rank == "J") {
        convertedRank = Rank::Jack;
    } else if (rank == "Q") {
        convertedRank = Rank::Queen;
    } else if (rank == "K") {
        convertedRank = Rank::King;
    } else if (rank == "A") {
        convertedRank = Rank::Ace;
    } else {
        convertedRank = (Rank) std::stoi(rank);
    }

    Suit convertedSuit;
    if (suit == "C") {
        convertedSuit = Suit::Clubs;
    } else if (suit == "D") {
        convertedSuit = Suit::Diamonds;
    } else if (suit == "H") {
        convertedSuit = Suit::Hearts;
    } else {
        convertedSuit = Suit::Spades;
    }
    
    return std::make_shared<Card>(convertedRank, convertedSuit);
}

void OddsCalc::getCards(std::string stage) {
    std::cout << stage << ": ";
    // Create and store new cards
    std::string line, inRank, inSuit;
    std::getline(std::cin, line);
    std::istringstream ss{line};
    while (ss >> inRank) {
        ss >> inSuit;
        sharCard newCard = createCard(inRank, inSuit);
        --deckSize;
        --numRemainingCards;
        --ranksLeft[newCard->getRank()];
        --suitsLeft[newCard->getSuit()];
        visibleCards.push_back(newCard);
    }
    // Display all visible cards
    for (auto &card : visibleCards) {
        std::cout << card->getCardString() << " ";
    }
    std::cout << std::endl;
}

// Requires n >= 0
double OddsCalc::factorial(int n) {
    auto it = factorialMemo.find(n);
    if (it != factorialMemo.end()) {
        return it->second;
    }
    double result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    factorialMemo[n] = result;
    return result;
}

// Requires n >= r >= 0
double OddsCalc::nCr(int n, int r) {
    if (r == 0 || n == r) return 1;
    if (r == 1) return n;
    return factorial(n) / (factorial(r) * factorial(n - r));
}

// Hypergeometric distribution
double OddsCalc::exactlyProb(int successPossibilities, int successes) {
    if (successes > successPossibilities || successes > numRemainingCards) return 0;
    return (nCr(successPossibilities, successes) * nCr(deckSize - successPossibilities, numRemainingCards - successes))
            / nCr(deckSize, numRemainingCards);
}

// This function is called after flop and turn
void OddsCalc::determineOdds(bool focusHand) {
    std::vector<sharCard> focusCards;
    if (focusHand) {
        std::cout << "Focus: Hand" << std::endl;
        focusCards = { visibleCards.begin(), visibleCards.begin() + 2 };
    } else {
        std::cout << "Focus: Table" << std::endl;
        focusCards = { visibleCards.begin() + 2, visibleCards.end() };
    }

    // Cards sorted by rank and suit
    std::map<Rank, int> cardsByRank;
    std::map<Suit, int> cardsBySuit;
    for (auto &card : focusCards) {
        if (focusHand) {
            cardsByRank[card->getRank()] = 4 - ranksLeft[card->getRank()];
            cardsBySuit[card->getSuit()] = 13 - suitsLeft[card->getSuit()];
        } else {
            ++cardsByRank[card->getRank()];
            ++cardsBySuit[card->getSuit()];
        }
    }

    // Unique ranks, sorted
    std::vector<Rank> uniqRanks;
    for (auto &card : cardsByRank) {
        uniqRanks.push_back(card.first);
    }
    // All visible unique ranks, sorted
    std::vector<Rank> allVisibleUniqRanks;
    for (auto &rank : RANKS) {
        if (ranksLeft[rank] != 4) {
            allVisibleUniqRanks.push_back(rank);
        }
    }
    const std::vector<Rank> availableRanks = (focusHand ? allVisibleUniqRanks : uniqRanks);
    
    // Flush odds
    double flushOdds = 0;
    for (auto &suit : cardsBySuit) {
        if (suit.second < 5) {
            flushOdds += exactlyProb(suitsLeft[suit.first], 5 - suit.second);
        } else {
            flushOdds += 1;
        }
    }

    // Straight odds
    double straightOdds = 0;
    bool wheelConsidered = false;
    const int numAvailableRanks = availableRanks.size();
    if (numAvailableRanks + numRemainingCards >= 5) {
        // Generate all possible straights
        std::vector<std::vector<Rank>> possibleStraights;
        for (auto &rank : uniqRanks) {
            for (int i = 0; i < 5; ++i) {
                if ((int)rank - i >= 2 && (int)rank + 4 - i <= 14) {
                    std::vector<Rank> possibility = {
                        (Rank)((int)rank - i), (Rank)((int)rank + 1 - i), (Rank)((int)rank + 2 - i),
                        (Rank)((int)rank + 3 - i), (Rank)((int)rank + 4 - i)
                    };
                    auto it = std::find(possibleStraights.begin(), possibleStraights.end(), possibility);
                    if (it == possibleStraights.end()) {
                        possibleStraights.push_back(possibility);
                    }
                }
            }
            // Possibilty for wheel straight - A 2 3 4 5
            if (!wheelConsidered && (int)rank <= 5) {
                possibleStraights.push_back({ Rank::Ace, Rank::Two, Rank::Three, Rank::Four, Rank::Five });
                wheelConsidered = true;
            }
        }
        // Generate all possible remaining cards needed for straight
        std::vector<std::vector<Rank>> ranksForStraight;
        for (auto &possibility : possibleStraights) {
            std::vector<Rank> ranksNeeded;
            for (auto &rank : possibility) {
                if (std::find(availableRanks.begin(), availableRanks.end(), rank) == availableRanks.end()) {
                    ranksNeeded.push_back(rank);
                }
            }
            if (std::find(ranksForStraight.begin(), ranksForStraight.end(), ranksNeeded) == ranksForStraight.end()) {
                ranksForStraight.push_back(ranksNeeded);
            }
        }
        // Calculate probabilities
        int oneRankLeftSum = 0;
        for (auto &ranks : ranksForStraight) {
            const int numRanksNeeded = ranks.size();
            if (numRanksNeeded == 0) {
                straightOdds += 1;
            } else if (numRanksNeeded == 1) {
                const int thisRankLeft = ranksLeft[ranks[0]];
                straightOdds += exactlyProb(thisRankLeft, 1);
                if (numRemainingCards == 2) {
                    straightOdds -= (oneRankLeftSum/47.0) * (thisRankLeft/46.0);   // P(A ∩ B)
                    oneRankLeftSum += thisRankLeft;
                }
            } else if (numRanksNeeded == 2 && numRemainingCards == 2) {
                auto it = std::find_if(ranksForStraight.begin(), ranksForStraight.end(), [&ranks](std::vector<Rank> oneRank) {
                    return oneRank.size() == 1 && (ranks[0] == oneRank[0] || ranks[1] == oneRank[0]);
                });
                if (it == ranksForStraight.end()) {
                    straightOdds += (ranksLeft[ranks[0]]/47.0) * (ranksLeft[ranks[1]]/46.0);
                }
            }
        }
    }

    // Quad, triple, and pair odds
    double quadOdds = 0;
    double tripleOdds = 0;
    double pairOdds = 0;
    int doublesRanksLeftSum = 0;
    int singlesRanksLeftSum = 0;
    for (auto &rank : cardsByRank) {
        const int thisRankLeft = ranksLeft[rank.first];
        // Only 1 quad of any rank is possible in 7 cards, so P(A ∩ B) is always 0
        if (rank.second < 4) {
            quadOdds += exactlyProb(thisRankLeft, 4 - rank.second);
        } else {
            quadOdds += 1;
            continue;
        }
        // Up to 2 triples of any rank are possible in 7 cards
        if (rank.second < 3) {
            tripleOdds += exactlyProb(thisRankLeft, 3 - rank.second);
            if (numRemainingCards == 2 && rank.second == 2) {
                tripleOdds -= (doublesRanksLeftSum/47.0) * (thisRankLeft/46.0);     // P(A ∩ B)
                doublesRanksLeftSum += thisRankLeft;
            }
        } else {
            tripleOdds += 1;
            continue;
        }
        // Up to 3 pairs of any rank are possible in 7 cards
        if (rank.second < 2) {
            pairOdds += exactlyProb(thisRankLeft, 2 - rank.second);
            if (numRemainingCards == 2) {
                pairOdds -= (singlesRanksLeftSum/47.0) * (thisRankLeft/46.0);     // P(A ∩ B)
                singlesRanksLeftSum += thisRankLeft;
            }
        } else {
            pairOdds += 1;
        }
    }
    // Calculate possibility of pair in remaining cards
    // Since method first called after flop, no other hand possible strictly in remaining cards
    if (!focusHand && numRemainingCards == 2) {
        for (auto &rank : RANKS) {
            if (std::find(uniqRanks.begin(), uniqRanks.end(), rank) == uniqRanks.end()) {
                pairOdds += exactlyProb(ranksLeft[rank], 2);
            }
        }
    }

    std::cout << "  Quad: " << quadOdds << std::endl;
    std::cout << "  Flush: " << flushOdds << std::endl;
    std::cout << "  Straight: " << straightOdds << std::endl;
    std::cout << "  Triple: " << tripleOdds << std::endl;
    std::cout << "  Pair: " << pairOdds << std::endl;
}

void OddsCalc::calcStage() {
    if (numRemainingCards <= 2) {
        determineOdds(true);
        determineOdds(false);
    }
}

//
// Public
//

OddsCalc::OddsCalc() : deckSize{52}, numRemainingCards{7} {}

void OddsCalc::setHand() {
    getCards("Hand");
}

void OddsCalc::calcFlop() {
    getCards("Flop");
    calcStage();
}

void OddsCalc::calcTurn() {
    getCards("Turn");
    calcStage();
}
