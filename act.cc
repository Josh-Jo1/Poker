#include "act.h"

Act::Act(Move move, double bet) : move{move}, bet{bet} {}

Move Act::getMove() const {
    return move;
}

double Act::getBet() const {
    return bet;
}
