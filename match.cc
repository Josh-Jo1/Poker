#include "match.h"
#include "humanPlayer.h"
#include "round.h"
#include <iostream>
#include <sstream>
#include <algorithm>


Match::Match(double buyIn, double bigBlind, std::string names)
    : deck{std::make_shared<DeckOfCards>()}, matchBuyIn{buyIn}, bigBlind{bigBlind}
{
    addPlayers(names);
}

void Match::setSeed(unsigned int seed) {
    deck = std::make_shared<DeckOfCards>(seed);
}

void Match::newRound() {
    deck->shuffle();
    Round round{deck, players, bigBlind};
    round.assignRoles();
    round.deal();

    // Perform intervals of betting
    const int betttingIntervals = 4;
    bool roundOver;
    for (int i = 0; i < betttingIntervals; ++i) {
        if (i == 1) round.flop();
        else if (i == 2) round.turn();
        else if (i == 3) round.river();
        roundOver = round.collectBets(i == 0);
        if (roundOver) {
            break;
        }
        std::cout << std::endl;
    }

    if (!roundOver) {
        round.endRound();
    }

    std::cout << "The round is over" << std::endl;

    // Rotate players left by 1, so that player at front (new small blind) will start bets
    std::rotate(players.begin(), players.begin() + 1, players.end());

    // Check for end game
    for (auto it = players.begin(); it != players.end(); ++it) {
        if ((*it)->getBank() == 0) {
            std::cout << (*it)->getName() << " has no more money and has lost" << std::endl;
            players.erase(it);
            --it;
        }
    }
    if (players.size() == 1) {
        std::cout << players.front()->getName() << " has won" << std::endl;
    }
}

void Match::addPlayers(std::string names) {
    std::istringstream ss{names};
    std::string name, buyIn;
    while (ss >> name) {
        ss >> buyIn;
        const double buyInAmount = (buyIn == "*" ? matchBuyIn : std::stod(buyIn));
        if (name != "cpu") {
            players.push_back(std::make_shared<HumanPlayer>(name, buyInAmount));
            std::cout << "  Player " << name << " added with $" << buyInAmount << std::endl;
        }
        // else cpu TODO
    }
}

void Match::removePlayers(std::string names) {
    std::istringstream ss{names};
    std::string name;
    while (ss >> name) {
        const auto found = std::find_if(players.begin(), players.end(),
                        [&name](const std::shared_ptr<Player> player) { return player->getName() == name; });
        if (found != players.end()) {
            std::cout << "  Player " << (*found)->getName() << " removed" << std::endl;
            players.erase(found);
        } else {
            std::cout << "  Player " << name << " not found" << std::endl;
        }
    }
}

void Match::addAmount(std::string names) {
    std::istringstream ss{names};
    std::string name, amount;
    while (ss >> name) {
        const auto found = std::find_if(players.begin(), players.end(),
                        [&name](const std::shared_ptr<Player> player) { return player->getName() == name; });
        if (found != players.end()) {
            ss >> amount;
            (*found)->addToBank(std::stod(amount));
            std::cout << "  Player " << (*found)->getName() << " received $" << amount << std::endl;
        } else {
            std::cout << "  Player " << name << " not found" << std::endl;
        }
    }
}

void Match::endGame() {
    for (auto &player : players) {
        std::cout << "  " << player->getName() << " ended with $" << player->getBank() << std::endl;
    }
}
