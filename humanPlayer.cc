#include "humanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(std::string name, double bank) : Player{name, bank} {}

Act HumanPlayer::getAct(double roundBet, double currBet) {
    // Display player info
    std::cout << getName() << "'s turn. Round bet: $" << roundBet << ". Current bet: $" << currBet << std::endl;
    const std::pair<const Card *, const Card*> hand = getHand();
    std::cout << "Hand: " << hand.first->getCardString() << " " << hand.second->getCardString() << std::endl;
    // Get input
    std::string move;
    std::cout << "Act: ";
    while (std::cin >> move) {
        if (move == "check") {
            if (currBet == roundBet) {
                return Act{Move::Check, 0};
            } else {
                std::cout << "Total bet must be at least " << currBet << std::endl;
            }
        } else if (move == "call") {
            const double totalBet = currBet - roundBet;
            if (totalBet == 0) {
                std::cout << "Bet is already highest bet" << std::endl;
            } else {
                if (takeFromBank(totalBet)) {
                    return Act{Move::Call, totalBet};
                } else {
                    std::cout << "Insufficient funds" << std::endl;
                }
            }
        } else if (move == "raise") {
            double bet;
            std::cin >> bet;
            const double totalBet = currBet - roundBet + bet;
            if (takeFromBank(totalBet)) {
                return Act{Move::Raise, totalBet};
            } else {
                std::cout << "Insufficient funds" << std::endl;
            }
        } else if (move == "fold") {
            return Act{Move::Fold, 0};
        } else {
            std::cout << "Invalid act. Please try again" << std::endl;
        }
        // Check for input termination
        if (!std::cin.eof()) {
            std::cout << "Act: ";
        }
    }
    // Only reached when eof
    return Act{Move::Check, 0};
}
