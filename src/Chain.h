//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_CHAIN_H
#define BEANS_CHAIN_H

#include <vector>
#include "Card.h"
#include "CardFactory.h"

// Chain abstract interface
class ChainBase
{
public:
    virtual int sell() = 0;

    virtual ChainBase &operator+=(Card *) = 0;

    virtual unsigned long size() = 0;
};

template <class T>
class Chain : public ChainBase
{
public:
    Chain(std::istream &, const CardFactory *);

    Chain<T> &operator+=(Card *);

    int sell();

    unsigned long size() override
    {
        return chain.size();
    }

private:
    std::vector<T *> chain;
};

#endif //BEANS_CHAIN_H
