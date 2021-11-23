//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_DISCARDPILE_H
#define BEANS_DISCARDPILE_H

#include "Card.h"
#include "CardFactory.h"

class DiscardPile : public std::vector<Card *> {
public:
    DiscardPile();

    DiscardPile(const DiscardPile &);

    DiscardPile &operator+=(Card *);

    // TODO: Deck already has an equivalent function to remove a card from the deck. This sounds redundant.
    Card *pickUp() {
        Card *card = this->back();
        this->pop_back();
        return card;
    }

    Card *top() {
        return this->back();
    }

    void print(std::ostream &) const {
        for (auto card: *this) {
            card->print(std::cout);
        }
    }

    friend std::ostream &operator<<(std::ostream &, const DiscardPile &);

    DiscardPile(std::istream &, const CardFactory *);

    ~DiscardPile();
};


#endif //BEANS_DISCARDPILE_H
