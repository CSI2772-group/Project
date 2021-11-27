//
// Created by fer on 2021-11-22.
//

#ifndef BEANS_CARDFACTORY_H
#define BEANS_CARDFACTORY_H

#include <iostream>
#include "Card.h"
#include <vector>
// TODO : Implement the CardFactory class

// Circular dependency??

class CardFactory;

// RFE: When we initialize the deck, how can we know how many cards are in the deck?
//      Is there a better way than hard-coding it in the constructor?
class Deck
{
public:
    Deck()
    {
    }

    Card *draw();

    Deck(std::istream &, const CardFactory *);

private:
    std::vector<Card *> cards;
    int currentCard = 0;
};

class CardFactory
{
public:
    void operator=(CardFactory const &) = delete;

    CardFactory(CardFactory const &) = delete;

    static CardFactory &getFactory()
    {
        static CardFactory factory; // singleton
        return factory;
    }

    Deck getDeck()
    {
        //        std::shuffle(deck.begin(), deck.end(), std::default_random_engine(std::random_device()()));
        return deck;
    }

    virtual Card *makeCard(char); //This isnt from the requirements
private:
    CardFactory() {}

    Deck deck;
};

#endif //BEANS_CARDFACTORY_H
