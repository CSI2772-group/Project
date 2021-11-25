#include "DiscardPile.h"
#include <algorithm>

DiscardPile &operator+=(DiscardPile d, Card *card)
{
    d.push_back(card);
}

DiscardPile::DiscardPile()
{
}

DiscardPile::DiscardPile(const DiscardPile &dp)
{
    for (int i = 0; i < size(); i++)
    {
        push_back(dp.at(i)); //unclear if this is supposed to be a clone or a copy
    }
}

Card *DiscardPile::pickUp()
{
    Card *c = at(size() - 1);
    pop_back();

    return c;
}

Card *DiscardPile::top()
{
    return back();
}

std::ostream &operator<<(std::ostream &os, const DiscardPile &dp)
{
    os << dp.back();
}

DiscardPile::DiscardPile(std::istream &is, const CardFactory *factory)
{
    //TODO: implement this
}

DiscardPile::~DiscardPile()
{
    //TODO: figure out what they want of this
}