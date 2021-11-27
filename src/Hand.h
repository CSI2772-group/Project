//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_HAND_H
#define BEANS_HAND_H

#include <queue>
#include "Card.h"
#include "CardFactory.h"

class Hand
{
public:
    friend Hand &operator+=(Hand h, Card *);

    Card *play();

    Card *top();

    Card *operator[](int);

    friend std::ostream &operator<<(std::ostream &, const Hand &);

    Hand(std::istream &, const CardFactory *);

private:
    std::deque<Card *> cards;
};

#endif //BEANS_HAND_H
