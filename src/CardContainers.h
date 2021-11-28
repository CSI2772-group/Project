#ifndef BEANS_CARDCONTAINERS_H
#define BEANS_CARDCONTAINERS_H

#include "Card.h"

// region Deck
class Deck : public std::vector<Card*>
{
    friend class CardFactory;
public:
    Deck();
    Card *draw() {
        Card *card = back();
        pop_back();
        return card;
    }

    Deck(std::istream &, const CardFactory *);

    void shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(begin(), end(), g);
    }
};
// endregion

// region Hand

class Hand : public std::vector<Card*>
{
    friend class Player;
public:
    Hand() = default;
    Hand &operator+=(Card * card) {
        push_back(card);
        return *this;
    }

    Card *play() {
        Card *c = back();
        pop_back();
        return c;
    }

    Card *top() {
        return back();
    }

    Card *operator[](int i) {
        Card *c = this->at(i);
        erase(begin() + i);
        return c;
    }

    // TODO: Implement serilizing and deserializing
    friend std::ostream &operator<<(std::ostream &, const Hand &);
    Hand(std::istream &, const CardFactory *);
};

// endregion

// region Chain
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
    Chain() = default;
    // TODO: Implement chain deserialization (might need to refactor a bit)
    Chain(std::istream & is, CardFactory * factory) {
        std::vector<char> v = reinterpret_cast<const std::vector<char> &>(is);

        char t = v.at(0); //assuming there is at least one character in is

        for (char c : v)
        {
            if (c != t)
            {
                IllegalType l; //make sure it is of same card type
                throw(l);
            }
            else
                chain.push_back(factory->makeCard(c));
        }
    }

    Chain<T> &operator+=(Card * card) {
        if (chain.empty() || card->getName() == chain[0]->getName())
            chain.push_back(card);
        else
            throw IllegalType();
    }

    int sell() override {
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

    unsigned long size() override
    {
        return chain.size();
    }

private:
    std::vector<T *> chain;
};
// endregion

// region Discard Pile

class DiscardPile : public std::vector<Card *> {
public:
    DiscardPile() = default;

    DiscardPile(const DiscardPile & dp) {
        for (int i = 0; i < size(); i++)
        {
            push_back(dp.at(i)); //unclear if this is supposed to be a clone or a copy
        }
    }

    DiscardPile &operator+=(Card * card) {
        push_back(card);
        return *this;
    }

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

    //TODO: Implement serialization and deserialization of discard pile
    friend std::ostream &operator<<(std::ostream &, const DiscardPile &);

    DiscardPile(std::istream &, const CardFactory *) {

    }

};

// endregion

// region Trade Area

class TradeArea {
public:
    TradeArea();

    // Just adds a card to the trade area
    TradeArea &operator+=(Card * card){
        cards.push_back(card);
        return *this;
    }

    // Returns true if the card is legal to add to the trade area
    bool legal(Card * card) const{
        // TODO: Check if this is the right way to do this
        for (auto &c : cards) {
            if (c->getName() == card->getName()) {
                return false;
            }
        }
        return true;
    }

    // Removes a card of the corresponding bean name from the trade area
    Card *trade(std::string name){
        for (auto it = cards.begin(); it != cards.end(); ++it) {
            if ((*it)->getName() == name) {
                Card *card = *it;
                cards.erase(it);
                return card;
            }
        }
        return nullptr;
    }

    int numCards() const{
        return cards.size();
    }

    // TODO: Implement deserialization and serialization of trade area
    friend std::ostream &operator<<(std::ostream &, const TradeArea &);

    TradeArea(std::istream &, const CardFactory *);

private:
    std::vector<Card *> cards;
};

// endregion

#endif //BEANS_CARDCONTAINERS_H
