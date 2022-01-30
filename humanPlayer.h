#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include "player.h"

class HumanPlayer: public Player {
    public:
    HumanPlayer(std::string name, double bank);
    Act getAct(double roundBet, double currBet) override;
};

#endif
