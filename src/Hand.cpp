#include "Hand.h"

Hand::Hand() {}

Hand::Hand(const Hand &) {}

Hand::Hand &operator+=(Hand h, Card *c)
{
    h.cards.push(c);
}

Card *Hand::play() {}

Card *Hand::top() {}

Card *Hand::operator[](int i)
{
    std::queue<Card *> q;

    for (int j = 0; j < i; j++)
    {
        q.push(cards.pop());
    }

    Card *c;

    c = cards.
}

std::ostream &operator<<(std::ostream &os, const Hand &h)
{
    os << h;
}

Hand::Hand(std::istream &, const CardFactory *) {}