#include "round.h"
#include "player.h"
#include <algorithm>
#include <iostream>

//
// Private
//

std::vector<const Card *> Round::getTable() const {
    std::vector<const Card *> cards;
    for (auto &card : table) {
        cards.push_back(card.get());
    }
    return cards;
}

void Round::blindBet(std::vector<sharPlayer>::const_iterator playerIt, std::string blind, double bet) {
    std::cout << (*playerIt)->getName() << " is the " << blind << " blind" << std::endl;
    if (!(*playerIt)->takeFromBank(bet)) {
        std::cout << (*playerIt)->getName() << " cannot afford the " << blind << " blind and is out of this round" << std::endl;
        activePlayers.erase(playerIt);
    } else {
        pot += bet;
        roundBets[(*playerIt)] += bet;
    }
}

void Round::printVector(std::string name, std::vector<const Card *> cards) const {
    std::cout << name;
    for (auto &card : cards) {
        std::cout << card->getCardString() << " ";
    }
    std::cout << std::endl;
}

std::string Round::HandTtoString(HandT handT) const {
    switch (handT) {
        case (HandT::RoyalFlush):
            return "Royal Flush";
        case (HandT::StraightFlush):
            return "Straight Flush";
        case (HandT::FourOfAKind):
            return "Four Of A Kind";
        case (HandT::FullHouse):
            return "Full House";
        case (HandT::Flush):
            return "Flush";
        case (HandT::Straight):
            return "Straight";
        case (HandT::ThreeOfAKind):
            return "Three Of A Kind";
        case (HandT::TwoPair):
            return "Two Pair";
        case (HandT::OnePair):
            return "One Pair";
        default:
            return "Highest";
    }
}

std::pair<HandT, std::vector<const Card *>> Round::determineHand(std::vector<const Card *> allCards) const {
    // Sort all cards by decreasing value
    std::sort(allCards.begin(), allCards.end(), [](const Card * a, const Card * b) { return a->getValue() > b->getValue(); });

    // DEV TESTING ONLY
    // std::vector<std::unique_ptr<Card>> testCards;
    // testCards.push_back(std::make_unique<Card>(Rank::Ace, Suit::Hearts));
    // testCards.push_back(std::make_unique<Card>(Rank::Jack, Suit::Spades));
    // testCards.push_back(std::make_unique<Card>(Rank::Ten, Suit::Clubs));
    // testCards.push_back(std::make_unique<Card>(Rank::Ten, Suit::Hearts));
    // testCards.push_back(std::make_unique<Card>(Rank::Ten, Suit::Diamonds));
    // testCards.push_back(std::make_unique<Card>(Rank::Three, Suit::Spades));
    // testCards.push_back(std::make_unique<Card>(Rank::Two, Suit::Spades));

    // allCards.clear();
    // for (auto &c : testCards) {
    //     allCards.push_back(c.get());
    // }

    // std::cout << "All cards: ";
    // for (auto &card : allCards) {
    //     std::cout << card->getRankString() << " ";
    // }
    // std::cout << std::endl;

    // Cards sorted by rank and suit
    std::map<Rank, std::vector<const Card *>> cardsByRank;
    std::map<Suit, std::vector<const Card *>> cardsBySuit;
    for (auto &card : allCards) {
        cardsByRank[card->getRank()].push_back(card);
        cardsBySuit[card->getSuit()].push_back(card);
    }

    // Unique cards
    std::vector<const Card *> uniqCards;
    for (auto it = cardsByRank.rbegin(); it != cardsByRank.rend(); ++it) {
        uniqCards.push_back(it->second.back());
    }

    // Check flush
    std::vector<const Card *> flush;
    for (auto &suit : cardsBySuit) {
        if (suit.second.size() == 5) {
            flush = suit.second;
            break;
        }
    }

    // Check straight
    std::vector<const Card *> straight;
    const int uniqCardsSize = uniqCards.size();
    if (uniqCardsSize >= 5) {
        for (int i = 0; i < uniqCardsSize - 4; ++i) {
            if (uniqCards[i]->getValue() - uniqCards[i + 4]->getValue() == 4) {
                straight = {uniqCards.begin() + i, uniqCards.begin() + i + 5};
                break;
            }
        }
        // Check for wheel straight - A 2 3 4 5
        // Example uniqCards: A . . 5 4 3 2 (where . is any card or empty)
        if (straight.empty() && uniqCards.front()->getRank() == Rank::Ace && uniqCards.back()->getRank() == Rank::Two
                && uniqCards.rbegin()[3]->getValue() - uniqCards.back()->getValue() == 3) {
            straight = {uniqCards.end() - 4, uniqCards.end()};
            straight.push_back(uniqCards.front());
        }
    }

    // Check for Royal flush or Straight flush
    if (!flush.empty() && !straight.empty()) {
        // If every rank is the same in flush and straight, straight flush exists
        // Cannot compare specific card * because if there are multiple cards of same rank, cannot be
        //      sure which suit included in straight (due to use of uniqCards)
        if (std::equal(flush.begin(), flush.end(), straight.begin(),
                        [](const Card * a, const Card * b) { return a->getRank() == b->getRank(); })) {
            if (flush.front()->getRank() == Rank::Ace && flush.begin()[1]->getRank() == Rank::King) {
                return std::make_pair(HandT::RoyalFlush, flush);
            }
            return std::make_pair(HandT::StraightFlush, flush);
        }
    }

    // Check for quad, triple, and pairs
    std::vector<const Card *> quad, triple, pair1, pair2;
    for (auto it = cardsByRank.rbegin(); it != cardsByRank.rend(); ++it) {
        const int rankSize = it->second.size();
        if (rankSize == 4 && quad.empty()) {
            quad = it->second;
        } else if (rankSize == 3 && triple.empty()) {
            triple = it->second;
        } else if (rankSize == 2 && pair1.empty()) {
            pair1 = it->second;
        } else if (rankSize == 2 && pair2.empty()) {
            pair2 = it->second;
        }
    }

    HandT bestHandT;
    std::vector<const Card *> bestHandCards;
    if (!quad.empty()) {
        bestHandT = HandT::FourOfAKind;
        bestHandCards = quad;
    } else if (!triple.empty() && !pair1.empty()) {
        bestHandT = HandT::FullHouse;
        if (triple[0]->getValue() < pair1[0]->getValue()) {
            bestHandCards = pair1;
            bestHandCards.insert(bestHandCards.end(), triple.begin(), triple.end());
        } else {
            bestHandCards = triple;
            bestHandCards.insert(bestHandCards.end(), pair1.begin(), pair1.end());
        }
    } else if (!flush.empty()) {
        bestHandT = HandT::Flush;
        bestHandCards = flush;
    } else if (!straight.empty()) {
        bestHandT = HandT::Straight;
        bestHandCards = straight;
    } else if (!triple.empty()) {
        bestHandT = HandT::ThreeOfAKind;
        bestHandCards = triple;
    } else if (!pair1.empty() && !pair2.empty()) {
        bestHandT = HandT::TwoPair;
        bestHandCards = pair1;
        bestHandCards.insert(bestHandCards.end(), pair2.begin(), pair2.end());
    } else if (!pair1.empty()) {
        bestHandT = HandT::OnePair;
        bestHandCards = pair1;
    } else {
        bestHandT = HandT::Highest;
    }

    if (bestHandT == HandT::Highest) {
        bestHandCards = {allCards.begin(), allCards.begin() + 5};
    } else {
        // Add additional cards to best hand cards till 5 found
        for (auto it = allCards.begin(); it != allCards.end() && bestHandCards.size() < 5; ++it) {
            // If *it not in best hand cards, push to back
            if (std::find(bestHandCards.begin(), bestHandCards.end(), *it) == bestHandCards.end()) {
                bestHandCards.push_back(*it);
            }
        }
    }

    return std::make_pair(bestHandT, bestHandCards);
}

int Round::isHandBetter(std::pair<HandT, std::vector<const Card *>> hand1, std::pair<HandT, std::vector<const Card *>> hand2) const {
    if (hand1.first == hand2.first) {
        const int handSize = 5;
        for (std::size_t i = 0; i < handSize; ++i) {
            if (hand1.second[i]->getValue() < hand2.second[i]->getValue()) {
                return 1;
            } else if (hand1.second[i]->getValue() > hand2.second[i]->getValue()) {
                return -1;
            }
        }
        return 0;
    }
    // Based on order in enum class Hand, using underlying int to compare
    return hand1.first > hand2.first ? 1 : -1;
}

//
// Public
//

Round::Round(std::shared_ptr<DeckOfCards> deck, std::vector<sharPlayer> players, double blind)
    : deck{deck}, players{players}, activePlayers{players}, smallBlind{blind / 2}, bigBlind{blind}, pot{0} {}

void Round::assignRoles() {
    // activePlayers[0] is small blind, activePlayers[1] is big blind, activePlayers.back() is dealer
    // This specific order allows for easy collection of bets in correct order
    blindBet(activePlayers.begin() + 1, "big", bigBlind);
    blindBet(activePlayers.begin(), "small", smallBlind);
    std::cout << activePlayers.back()->getName() << " is the dealer" << std::endl;
}

void Round::deal() {
    for (auto &player : players) {
        uniqCard first = deck->getTopCard();
        uniqCard second = deck->getTopCard();
        player->setHand(std::move(first), std::move(second));
    }
}

bool Round::collectBets(bool blind) {
    double currBet = roundBets[activePlayers.front()];
    if (blind) {
        // Rotate players left by 2
        std::rotate(activePlayers.begin(), activePlayers.begin() + 2, activePlayers.end());
        currBet = bigBlind;
    }

    bool firstCycle = true;
    auto it = activePlayers.begin();
    while (firstCycle || roundBets[(*it)] != currBet) {
        std::cout << "Pot: " << pot << std::endl;
        // Handle player act
        const Act pAct = (*it)->getAct(roundBets[(*it)], currBet);
        if (pAct.getMove() == Move::Fold) {
            activePlayers.erase(it);
            if (activePlayers.size() == 1) {
                std::cout << activePlayers.front()->getName() << " has won $" << pot << std::endl;
                activePlayers.front()->addToBank(pot);
                return true;
            }
            --it;
        } else {
            roundBets[(*it)] += pAct.getBet();
            currBet = roundBets[(*it)];
            pot += pAct.getBet();
        }
        // Handle iteration
        if (it + 1 == activePlayers.end()) {
            firstCycle = false;
            it = activePlayers.begin();
        } else {
            ++it;
        }
        // Handle input termination
        if (std::cin.eof()) return true;
    }

    if (blind) {
        // Rotate players right by 2 - original order
        std::rotate(activePlayers.rbegin(), activePlayers.rbegin() + 2, activePlayers.rend());
    }
    return false;
}

void Round::flop() {
    for (int i = 0; i < 3; ++i) {
        table.push_back(deck->getTopCard());
    }
    printVector("Table: ", getTable());
}

void Round::turn() {
    table.push_back(deck->getTopCard());
    printVector("Table: ", getTable());
}

void Round::river() {
    table.push_back(deck->getTopCard());
    printVector("Table: ", getTable());
}

// This function is always called after river
void Round::endRound() {
    std::vector<sharPlayer> winners;
    std::pair<HandT, std::vector<const Card *>> bestHand;
    for (auto &player : activePlayers) {
        std::vector<const Card *> allCards = getTable();
        allCards.push_back(player->getHand().first);
        allCards.push_back(player->getHand().second);
        const std::pair<HandT, std::vector<const Card *>> playerHand = determineHand(allCards);
        std::cout << player->getName() << " has a " << HandTtoString(playerHand.first) << " hand: ";
        printVector("", playerHand.second);

        if (winners.size() == 0) {
            winners.push_back(player);
            bestHand = playerHand;
            continue;
        }

        int betterHand = isHandBetter(bestHand, playerHand);
        if (betterHand == 1) {
            winners.clear();
            winners.push_back(player);
            bestHand = playerHand;
        } else if (betterHand == 0) {
            winners.push_back(player);
        }
    }

    const double winAmount = pot / winners.size();
    for (auto &winner : winners) {
        std::cout << winner->getName() << " has won $" << winAmount << std::endl;
        winner->addToBank(winAmount);
    }

    // Return cards to deck
    for (auto &player : players) {
        std::pair<uniqCard, uniqCard> hand = player->returnHand();
        deck->returnCard(std::move(hand.first));
        deck->returnCard(std::move(hand.second));
    }
    while (table.size() != 0) {
        deck->returnCard(std::move(table.back()));
        table.pop_back();
    }
}
