#include "Chain.h"
#include "Card.h"
#include <stdexcept>
#include <vector>
#include "IllegapType.h"

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

Chain<Card>::Chain()
{
    std::vector<Card> chain;
}

template <class T>
Chain<T>::Chain(const Chain<T> &)
{
}

template <class T>
Chain<T>::Chain(std::istream &, const CardFactory *)
{
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