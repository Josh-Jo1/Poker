#include "oddsCalc.h"
#include <iostream>
#include <string>

int main () {
    OddsCalc{};
    std::string input = "Y";
    while (input == "Y" && !std::cin.eof()) {
        OddsCalc calc{};
        calc.setHand();
        calc.calcFlop();
        calc.calcTurn();

        std::cout << "Again? (Y/N): ";
        std::cin >> input;
        std::cin.ignore();
    }
}


// Verified tests:

// Hand: 5 C 7 D
// 5♣ 7♠
// Flop: 5 D 7 C 6 H
// 5♣ 7♠ 5♠ 7♣ 6♥
// Focus: Hand
//   Quad: 0.00185014
//   Flush: 0
//   Straight: 0.0222017
//   Triple: 0.164662
//   Pair: 2
// Focus: Table
//   Quad: 0
//   Flush: 0
//   Straight: 0.0222017
//   Triple: 0.00462535
//   Pair: 0.336725
// Turn: Q H
// 5♣ 7♦ 5♦ 7♣ 6♥ Q♥
// Focus: Hand
//   Quad: 0
//   Flush: 0
//   Straight: 0
//   Triple: 0.0869565
//   Pair: 2
// Focus: Table
//   Quad: 0
//   Flush: 0
//   Straight: 0
//   Triple: 0
//   Pair: 0.217391

// Hand: 5 C 5 D
// 5♣ 5♦
// Flop: 7 C 7 D 6 C
// 5♣ 5♦ 7♣ 7♦ 6♣
// Focus: Hand
//   Quad: 0.000925069
//   Flush: 0.0416281
//   Straight: 0.0222017
//   Triple: 0.0832562
//   Pair: 1
// Focus: Table
//   Quad: 0.000925069
//   Flush: 0
//   Straight: 0
//   Triple: 0.0860315
//   Pair: 1.17854
// Turn: Q C
// 5♣ 5♦ 7♣ 7♦ 6♣ Q♣
// Focus: Hand
//   Quad: 0
//   Flush: 0.195652
//   Straight: 0
//   Triple: 0.0434783
//   Pair: 1
// Focus: Table
//   Quad: 0
//   Flush: 0
//   Straight: 0
//   Triple: 0.0434783
//   Pair: 1.13043

// Hand: 4 D 6 C
// 4♦ 6♣
// Flop: 7 S 8 D 10 D
// 4♦ 6♣ 7♠ 8♦ 10♦
// Focus: Hand
//   Quad: 0
//   Flush: 0.0416281
//   Straight: 0.310823
//   Triple: 0.00555042
//   Pair: 0.240056
// Focus: Table
//   Quad: 0
//   Flush: 0
//   Straight: 0.012951
//   Triple: 0.00832562
//   Pair: 0.403793
// Turn: 9 C
// 4♦ 6♣ 7♠ 8♦ 10♦ 9♣
// Focus: Hand
//   Quad: 0
//   Flush: 0
//   Straight: 1.08696
//   Triple: 0
//   Pair: 0.130435
// Focus: Table
//   Quad: 0
//   Flush: 0
//   Straight: 0.152174
//   Triple: 0
//   Pair: 0.26087
