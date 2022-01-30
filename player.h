#ifndef PLAYER_H
#define PLAYER_H
#include "act.h"
#include "card.h"
#include <memory>
#include <string>
#include <utility>

using uniqCard = std::unique_ptr<Card>;

class Player {
    std::string name;
    double bank;
    std::pair<uniqCard, uniqCard> hand;

    public:
    Player(std::string name, double bank);
    std::string getName() const;
    double getBank() const;
    void addToBank(double amount);
    bool takeFromBank(double amount);
    std::pair<const Card *, const Card *> getHand() const;
    void setHand(uniqCard first, uniqCard second);
    std::pair<uniqCard, uniqCard> returnHand();
    virtual Act getAct(double roundBet, double currBet) = 0;
    virtual ~Player() = default;
};

#endif
