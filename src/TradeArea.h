//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_TRADEAREA_H
#define BEANS_TRADEAREA_H

#include <list>
#include "Card.h"
#include "CardFactory.h"

class TradeArea {
public:
    TradeArea();

    TradeArea &operator+=(Card *);

    bool legal(Card *) const;

    Card *trade(std::string);

    int numCards() const;

    friend std::ostream &operator<<(std::ostream &, const TradeArea &);

    TradeArea(std::istream &, const CardFactory *);

private:
    std::list<Card *> cards;

};


#endif //BEANS_TRADEAREA_H
