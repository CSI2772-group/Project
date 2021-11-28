//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_TABLE_H
#define BEANS_TABLE_H

#include <string>
#include "Card.h"
#include "CardContainers.h"
#include "Player.h"

#define MAKE_CARDS(deck, factory, cardType, amount) \
    for (int i = 0; i < amount; i++)                \
        deck.push_back(factory->makeCard(cardType));\

class Table {
public:
    // Initialize the table from zero
    Table(std::string p1, std::string p2): player1(p1), player2(p2), deck(){
        CardFactory* factory = CardFactory::getFactory();
        MAKE_CARDS(deck, factory, 'B', 20);
        MAKE_CARDS(deck, factory, 'C', 18);
        MAKE_CARDS(deck, factory, 'S', 16);
        MAKE_CARDS(deck, factory, 'G', 14);
        MAKE_CARDS(deck, factory, 's', 12);
        MAKE_CARDS(deck, factory, 'b', 10);
        MAKE_CARDS(deck, factory, 'R', 8);
        MAKE_CARDS(deck, factory, 'g', 6);
        deck.shuffle();
    }

    // Todo : Add a constructor that takes a file name and loads the table from it
    Table(std::istream &, const CardFactory *);

    Table(const TradeArea &tradeArea);

    bool win(std::string & winner) const{
        bool won = false;
        if (player1.hand.size() == 0) {
            won = true;
            winner = player1.getName();
        }
        if (player2.hand.size() == 0) {
            won = true;
            winner = player2.getName();
        }
        return won;
    }

    void printHand(bool) const;

    friend std::ostream &operator<<(std::ostream &, const Table &);

private:
    Player player1;
    Player player2;
    Deck deck;
    DiscardPile discardPile;
    TradeArea tradeArea;
};

#endif //BEANS_TABLE_H
