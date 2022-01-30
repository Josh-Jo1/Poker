#ifndef ACT_H
#define ACT_H

// Types of moves
enum class Move { Check, Call, Raise, Fold };

class Act {
    Move move;
    double bet;

    public:
    Act(Move move, double bet);
    Move getMove() const;
    double getBet() const;
};

#endif
