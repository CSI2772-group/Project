#include "Hand.h"
#include <list>

Hand::Hand &operator+=(Hand h, Card *c)
{
    h.cards.push_back(c);
}

Card *Hand::play()
{
    Card *c = cards.front();
    cards.pop_front();

    return c;
}

Card *Hand::top()
{
    return cards.front();
}

Card *Hand::operator[](int i)
{
    Card *c = cards.at(i);
    cards.erase(cards.begin() + i);

    return c;
}

std::ostream &operator<<(std::ostream &os, const Hand &h)
{
    os << h;
}

Hand::Hand(std::istream &, const CardFactory *)
{
}