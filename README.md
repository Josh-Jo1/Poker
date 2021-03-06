# Poker

This is a C++ project for the popular Texas Hold'em variant of poker.

Currently, its purpose is to manage a game of poker, with a heavy focus on implementing OOP. The program allows for any number of players to play using a command-line interface, but there are no computer players and each player's hand is visible to everyone. In the future, features may be added to address these issues and improve the game.

Additionally, there is an odds calculator that a player may use to determine the probability of getting a quad, flush, straight, triple, and pair. These probabilities are calculated using the cards in their hand and separately using only the cards on the table. In the future, the ability to determine opponents' odds can be included, which could then lead to the first computer player.

## Usage

Open a terminal in the project directory.

Make the project with: `make`

Run the program with: `./poker`

Make the odds calculator with: `make oddsCalc`

Run the odds calculator with: `./oddsCalc`

## Game Flow

### Initial Setup

| Input      | Description              | Example      |
| ---------- | ------------------------ | ------------ |
| Buy-in     | Positive double value    | `50`         |
| Big blind  | Positive double value    | `2`          |
| Players    | Pairs of name and amount | `P1 30 P2 *` |
| Seed       | Optional integer value   | `10`         |

The players' name is any string. The players' amount is either "*" to indicate the buy-in or a positive double value.

### Commands

| Command   | Description      | Example        |
| --------- | ---------------- | -------------  |
| `nr`      | New round        | `nr`           |
| `ap`      | Add players      | `ap P1 5 P2 *` |
| `rp`      | Remove players   | `rp P2`        |
| `aa`      | Add amount       | `aa P1 20`     |
| `eg`      | End game         | `eg`           |

The players' name is any string. The players' amount is either "*" to indicate the buy-in or a positive double value.

### Acts

The available acts are as per the rules of Texas Hold'em. Acts may be one of `check`, `call`, `raise`, or `fold`. A `raise` must be followed by a positive double value.

## Implementation

The Card class is used to contain the Rank and Suit for any playing card, with basic getter methods to access the information. Then, the DeckOfCards class creates unique pointers to Card objects to form a standard deck of playing cards. DeckOfCards can get the top card, return a card to the deck, and shuffle the deck (using an optional seed).

The Act class is used to contain a player's Move and bet amount, with basic getter methods to access the information. The abstract Player class holds data for any player, including their name, amount in the bank, and hand. The hand is a pair of unique pointers to Cards and the pure virtual getAct method will return a player's Act on their turn. Other methods are available to access and modify the bank and hand. The HumanPlayer class inherits from the Player class with its own implementation of the getAct method. This design will allow for computer players to be implemented easily in the future.

Before looking at the Round class, consider the Match class. This class creates a shared pointer to the DeckOfCards and shared pointers to Players for the duration of the game. The available methods are used to control a game of poker by starting a new round, adding or removing players, adding to their bank amounts, and ending the game. The most crucial is starting a new round, which creates a Round object and uses its public methods to control the flow of a round of poker. The Round class is responsible for maintaining the active players, pot amount, and Cards on the table. The available methods allow for assigning the blinds and dealer, collecting bets, making cards visible, and determining the winner of a round of poker.

Through the use of RAII principles, this program effectively handles memory management and removes the requirement from clients. The single-responsibility principle and appropriate modularization allow for easily understandable code and modifications to be made efficiently. High cohesion and low coupling also help to add new features to this project without impacting many dependencies.

## Statistics

Equation 1: P(A ??? B) = P(A) + P(B) - P(A ??? B)

Since the probabilities are calculated after the flop and river, Equation 1 will suffice to accurately calculate probabilities while ensuring the same event is not counted multiple times.

For quad, flush, triple, and pair, the probability is calculated by determining exactly how many more cards are needed, then considering its possibility within the remaining cards. Applying Equation 1, if we have one 7 and want a pair, then we need exactly one 7 and another card that is not 7. If we also have one 5 and want another pair, then we need one 5 and another card that is not 5, minus the probability of one 5 and one 7.

For straight, all the possible straights are determined from the available cards, then the ranks needed for those straights, and finally the possibility of getting those ranks in the remaining cards. Applying Equation 1, if we have 7 in hand and 8 9 10 on the table, a straight can be made with 5 and 6, just 6, just J, or J and Q. Calculating for one 6 will include 5 and 6, and similarly for J, so that reduces the possibilities to consider. After calculating the straight with 6 by considering one 6 and another card that is not 6, calculating the straight with J must minus the probability of one 6 and one J.

## Sample Game

```
Buy-in: $200
Big blind: $2
Players: JJ * Kush 150 Vans * Visali *
  Player JJ added with $200
  Player Kush added with $150
  Player Vans added with $200
  Player Visali added with $200
Seed: 40

Enter command: nr
Kush is the big blind
JJ is the small blind
Visali is the dealer
Pot: 3
Vans's turn. Round bet: $0. Current bet: $2
Hand: 7??? 10???
Act: call
Pot: 5
Visali's turn. Round bet: $0. Current bet: $2
Hand: 3??? A???
Act: call
Pot: 7
JJ's turn. Round bet: $1. Current bet: $2
Hand: K??? J???
Act: call
Pot: 8
Kush's turn. Round bet: $2. Current bet: $2
Hand: 3??? A???
Act: check

Table: 5??? 5??? 4???
Pot: 8
JJ's turn. Round bet: $2. Current bet: $2
Hand: K??? J???
Act: check
Pot: 8
Kush's turn. Round bet: $2. Current bet: $2
Hand: 3??? A???
Act: check
Pot: 8
Vans's turn. Round bet: $2. Current bet: $2
Hand: 7??? 10???
Act: check
Pot: 8
Visali's turn. Round bet: $2. Current bet: $2
Hand: 3??? A???
Act: check

Table: 5??? 5??? 4??? Q???
Pot: 8
JJ's turn. Round bet: $2. Current bet: $2
Hand: K??? J???
Act: raise 5
Pot: 13
Kush's turn. Round bet: $2. Current bet: $7
Hand: 3??? A???
Act: fold
Pot: 13
Vans's turn. Round bet: $2. Current bet: $7
Hand: 7??? 10???
Act: call
Pot: 18
Visali's turn. Round bet: $2. Current bet: $7
Hand: 3??? A???
Act: call

Table: 5??? 5??? 4??? Q??? A???
Pot: 23
JJ's turn. Round bet: $7. Current bet: $7
Hand: K??? J???
Act: check
Pot: 23
Vans's turn. Round bet: $7. Current bet: $7
Hand: 7??? 10???
Act: check
Pot: 23
Visali's turn. Round bet: $7. Current bet: $7
Hand: 3??? A???
Act: raise 3
Pot: 26
JJ's turn. Round bet: $7. Current bet: $10
Hand: K??? J???
Act: fold
Pot: 26
Vans's turn. Round bet: $7. Current bet: $10
Hand: 7??? 10???
Act: fold
Visali has won $26
The round is over

Enter command: nr
Vans is the big blind
Kush is the small blind
JJ is the dealer
Pot: 3
Visali's turn. Round bet: $0. Current bet: $2
Hand: Q??? 5???
Act: call
Pot: 5
JJ's turn. Round bet: $0. Current bet: $2
Hand: 9??? 8???
Act: call
Pot: 7
Kush's turn. Round bet: $1. Current bet: $2
Hand: J??? 9???
Act: call
Pot: 8
Vans's turn. Round bet: $2. Current bet: $2
Hand: 10??? 7???
Act: check

Table: 2??? J??? A???
Pot: 8
Kush's turn. Round bet: $2. Current bet: $2
Hand: J??? 9???
Act: raise 3
Pot: 11
Vans's turn. Round bet: $2. Current bet: $5
Hand: 10??? 7???
Act: fold
Pot: 11
Visali's turn. Round bet: $2. Current bet: $5
Hand: Q??? 5???
Act: call
Pot: 14
JJ's turn. Round bet: $2. Current bet: $5
Hand: 9??? 8???
Act: fold

Table: 2??? J??? A??? 4???
Pot: 14
Kush's turn. Round bet: $5. Current bet: $5
Hand: J??? 9???
Act: check
Pot: 14
Visali's turn. Round bet: $5. Current bet: $5
Hand: Q??? 5???
Act: check

Table: 2??? J??? A??? 4??? 8???
Pot: 14
Kush's turn. Round bet: $5. Current bet: $5
Hand: J??? 9???
Act: check
Pot: 14
Visali's turn. Round bet: $5. Current bet: $5
Hand: Q??? 5???
Act: check

Kush has a One Pair hand: J??? J??? A??? 9??? 8???
Visali has a Highest hand: A??? Q??? J??? 8??? 5???
Kush has won $14
The round is over

Enter command: nr
Visali is the big blind
Vans is the small blind
Kush is the dealer
Pot: 3
JJ's turn. Round bet: $0. Current bet: $2
Hand: 2??? 10???
Act: call
Pot: 5
Kush's turn. Round bet: $0. Current bet: $2
Hand: J??? Q???
Act: call
Pot: 7
Vans's turn. Round bet: $1. Current bet: $2
Hand: 2??? 5???
Act: call
Pot: 8
Visali's turn. Round bet: $2. Current bet: $2
Hand: 9??? 2???
Act: check

Table: 10??? Q??? J???
Pot: 8
Vans's turn. Round bet: $2. Current bet: $2
Hand: 2??? 5???
Act: check
Pot: 8
Visali's turn. Round bet: $2. Current bet: $2
Hand: 9??? 2???
Act: check
Pot: 8
JJ's turn. Round bet: $2. Current bet: $2
Hand: 2??? 10???
Act: check
Pot: 8
Kush's turn. Round bet: $2. Current bet: $2
Hand: J??? Q???
Act: check

Table: 10??? Q??? J??? K???
Pot: 8
Vans's turn. Round bet: $2. Current bet: $2
Hand: 2??? 5???
Act: check
Pot: 8
Visali's turn. Round bet: $2. Current bet: $2
Hand: 9??? 2???
Act: check
Pot: 8
JJ's turn. Round bet: $2. Current bet: $2
Hand: 2??? 10???
Act: check
Pot: 8
Kush's turn. Round bet: $2. Current bet: $2
Hand: J??? Q???
Act: check

Table: 10??? Q??? J??? K??? 8???
Pot: 8
Vans's turn. Round bet: $2. Current bet: $2
Hand: 2??? 5???
Act: check
Pot: 8
Visali's turn. Round bet: $2. Current bet: $2
Hand: 9??? 2???
Act: check
Pot: 8
JJ's turn. Round bet: $2. Current bet: $2
Hand: 2??? 10???
Act: check
Pot: 8
Kush's turn. Round bet: $2. Current bet: $2
Hand: J??? Q???
Act: check

Vans has a Highest hand: K??? Q??? J??? 10??? 8???
Visali has a Straight hand: K??? Q??? J??? 10??? 9???
JJ has a One Pair hand: 10??? 10??? K??? Q??? J???
Kush has a Two Pair hand: Q??? Q??? J??? J??? K???
Visali has won $8
The round is over

Enter command: eg
  Visali ended with $217
  JJ ended with $189
  Kush ended with $155
  Vans ended with $189
```

## Sample Odds
```
Hand: 5 C 5 D
5??? 5???
Flop: 7 C 7 D 6 C
5??? 5??? 7??? 7??? 6???
Focus: Hand
  Quad: 0.000925069
  Flush: 0.0416281
  Straight: 0.0222017
  Triple: 0.0832562
  Pair: 1
Focus: Table
  Quad: 0.000925069
  Flush: 0
  Straight: 0
  Triple: 0.0860315
  Pair: 1.17854
Turn: Q C
5??? 5??? 7??? 7??? 6??? Q???
Focus: Hand
  Quad: 0
  Flush: 0.195652
  Straight: 0
  Triple: 0.0434783
  Pair: 1
Focus: Table
  Quad: 0
  Flush: 0
  Straight: 0
  Triple: 0.0434783
  Pair: 1.13043
```
