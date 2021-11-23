//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_TABLE_H
#define BEANS_TABLE_H

#include <string>
#include "CardFactory.h"
#include "DiscardPile.h"
#include "Player.h"
#include "TradeArea.h"


class Table {
public:
    Table(std::istream &, const CardFactory *);
    //{
    //  // Reconstruct the table from the input stream
    //  player1 = new Player(1, "P1");
    //  player2 = new Player(1, "P1");
    //}

    Table(const TradeArea &tradeArea);

    bool win(std::string &) const;

    void printHand(bool) const;

    friend std::ostream &operator<<(std::ostream &, const Table &);

private:
    Deck deck;
    DiscardPile discardPile;
    TradeArea tradeArea;
    Player player1;
    Player player2;
};


#endif //BEANS_TABLE_H
