#include "Chain.h"
#include "Card.h"
#include <stdexcept>
#include <vector>
#include "IllegalType.h"

int Chain<Card>::sell()
{
    if (chain.size() > chain[0]->getCardsPerCoin(4))
        return 4;
    else if (chain.size() > chain[0]->getCardsPerCoin(3))
        return 3;
    else if (chain.size() > chain[0]->getCardsPerCoin(2))
        return 2;
    else if (chain.size() > chain[0]->getCardsPerCoin(1))
        return 1;
    else
        return 0;
}

template <class T>
Chain<T>::Chain(std::istream &is, const CardFactory *factory)
{
    std::vector<char> v = is;

    char t = v.at(0); //assuming there is at least one character in is

    for (char c : is)
    {
        if (c != t)
        {
            IllegalType l; //make sure is is of same card type
            throw(l);
        }
        else
            chain.push_back(factory->makeCard(c));
    }
}

template <class T>
Chain<T> &operator+=(Chain<T> c, Card *card)
{
    if (c.chain.size() > 0 && card->getName() != c.chain[0]->getName())
    {
        IllegalType l;
        throw(l);
    }
    else
        c.chain.insert(card);
}
