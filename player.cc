#include "player.h"

Player::Player(std::string name, double bank) : name{name}, bank{bank} {}

std::string Player::getName() const {
    return name;
}

double Player::getBank() const {
    return bank;
}

// For proper use, requires amount > 0
void Player::addToBank(double amount) {
    bank += amount;
}

// For proper use, requires amount > 0
bool Player::takeFromBank(double amount) {
    if (bank >= amount) {
        bank -= amount;
        return true;
    }
    return false;
}

std::pair<const Card *, const Card *> Player::getHand() const {
    return std::make_pair(hand.first.get(), hand.second.get());
}

void Player::setHand(uniqCard first, uniqCard second) {
    hand = std::make_pair(std::move(first), std::move(second));
}

std::pair<uniqCard, uniqCard> Player::returnHand() {
    return std::move(hand);
}
