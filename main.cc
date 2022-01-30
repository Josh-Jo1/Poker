#include "match.h"
#include <iostream>
#include <string>
#include <sstream>

int main() {
    double buyIn, bigBlind;
    std::cout << "Buy-in: $";
    std::cin >> buyIn;
    std::cout << "Big blind: $";
    std::cin >> bigBlind;

    std::string names;
    std::cout << "Players: ";
    std::cin.ignore();
    std::getline(std::cin, names);

    Match match{buyIn, bigBlind, names};

    std::string seedLine;
    std::cout << "Seed: ";
    std::getline(std::cin, seedLine);
    std::istringstream ss{seedLine};
    unsigned int seed;
    if (ss >> seed) {
        match.setSeed(seed);
    }

    // DEV TESTING ONLY
    // double buyIn = 50;
    // double bigBlind = 2;
    // std::string names = "JJ * V * Kush 25 Vans *";
    // Match match{buyIn, bigBlind, names};
    // match.setSeed(10);


    std::string cmd;
    std::cout << std::endl << "Enter command: ";
    while (std::cin >> cmd) {
        if (cmd == "nr") {
            match.newRound();
        } else if (cmd == "ap") {
            std::getline(std::cin, names);
            match.addPlayers(names);
        } else if (cmd == "rp") {
            std::getline(std::cin, names);
            match.removePlayers(names);
        } else if (cmd == "aa") {
            std::getline(std::cin, names);
            match.addAmount(names);
        } else if (cmd == "eg") {
            match.endGame();
            break;
        } else {
            std::cout << "Invalid command" << std::endl;
        }
        // Check for input termination
        if (!std::cin.eof()) {
            std::cout << std::endl << "Enter command: ";
        }
    }
}
