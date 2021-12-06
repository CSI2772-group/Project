#ifndef BEANS_CARDCONTAINERS_H
#define BEANS_CARDCONTAINERS_H

#include "Card.h"
#include <list>

// region Deck
class Deck : public std::vector<Card *>
{
    friend class CardFactory;

public:
    Deck() = default;

    Card *draw()
    {
        Card *card = back();
        pop_back();
        return card;
    }

    Deck(std::istream &, const CardFactory *);

    void shuffle()
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(begin(), end(), rng);
    }

    void shuffle(int seed)
    {
        std::mt19937 rng(seed);
        std::shuffle(begin(), end(), rng);
    }
};
// endregion

// region Hand

class Hand : public std::list<Card *>
{
    friend class Player;

public:
    Hand() = default;

    Hand &operator+=(Card *card)
    {
        push_back(card);
        return *this;
    }

    Card *play()
    {
        Card *c = front();
        pop_front();
        return c;
    }

    Card *top() { return front(); }

    Card *operator[](int i)
    {
        // Return i-th card from the hand and remove it from the hand
        auto it = begin();
        std::advance(it, i);
        Card *c = *it;
        erase(it);
        return c;
    }

    // TODO: make sure this works
    friend std::ostream &operator<<(std::ostream &os, const Hand &h)
    {
        for (auto card : h)
        {
            os.put(card->getShortName());
        }

        return os;
    };

    Hand(std::istream &is, const CardFactory *cf)
    {
        while (is)
        {
            push_back(cf->getFactory()->makeCard(is.get()));
        }
    };

    std::vector<Card *> getUniqueCards()
    {
        std::vector<Card *> unique;

        for (auto *c : *this)
        {
            //for each card in hand

            //check to see if its already in unique
            bool anyEqual = false;
            for (auto *u : unique)
                anyEqual = anyEqual || c->getName() == u->getName();

            //if it's not, add it to unique
            if (!anyEqual)
                unique.push_back(c);
        }

        return unique;
    }
};

// endregion

// region Chain
class ChainBase
{
public:
    virtual int sell() = 0;

    virtual ChainBase &operator+=(Card *c) = 0;

    virtual std::string chainType() = 0;

    virtual char chainTypeShort() = 0;

    int chainSize = 0;
};

// template class that extends "Card"
template <class T>
class Chain : public ChainBase
{
    static_assert(std::is_base_of<Card, T>::value, "T must derive from Card");

public:
    Chain() = default;

    // TODO: Implement chain deserialization (it takes a char and a byte from
    // istream)
    // not sure what to do with the byte
    Chain(std::istream &is, CardFactory *cf)
    {
        while (is)
        {
            cards.push_back(cf->getFactory()->makeCard(is.get()));
        }
    }

    Chain<T> &operator+=(Card *card) override
    {
        // try to cast to card to T
        T *t = dynamic_cast<T *>(card);
        if (t != nullptr)
        {
            // cast successful
            cards.push_back(t);
        }
        else
        {
            // cast failed
            throw IllegalType();
        }
    }

    int sell() override
    {

        if (cards.size() >= T::getCardsPerCoin(4))
            return 4;
        else if (cards.size() >= T::getCardsPerCoin(3))
            return 3;
        else if (cards.size() >= T::getCardsPerCoin(2))
            return 2;
        else if (cards.size() >= T::getCardsPerCoin(1))
            return 1;
        else
            return 0;
    }

    std::string chainType() override { return T::getName(); }

    char chainTypeShort() override { return T::getShortName(); }

private:
    std::vector<T *> cards;
};

// Chain Factory

class ChainFactory
{
public:
    void operator=(ChainFactory const &) = delete;

    ChainFactory(ChainFactory const &) = delete;

    static ChainFactory *getFactory()
    {
        static ChainFactory factory; // singleton
        return &factory;
    }

    ChainBase *createChain(char chainType)
    {
        ChainBase *chain = nullptr;
        switch (chainType)
        {
        case 'B':
            chain = new Chain<Blue>();
            break;
        case 'C':
            chain = new Chain<Chili>();
            break;
        case 'S':
            chain = new Chain<Stink>();
            break;
        case 'G':
            chain = new Chain<Green>();
            break;
        case 's':
            chain = new Chain<Soy>();
            break;
        case 'b':
            chain = new Chain<Black>();
            break;
        case 'R':
            chain = new Chain<Red>();
            break;
        case 'g':
            chain = new Chain<Garden>();
            break;
        default:
            throw IllegalType();
        }
        return chain;
    }

private:
    ChainFactory() = default;

    std::vector<Card *> cards;
};

// endregion

// endregion

// region Discard Pile

class DiscardPile : public std::vector<Card *>
{
public:
    using std::vector<Card *>::vector; //inherit vector constructors

    DiscardPile(const DiscardPile &dp)
    {
        for (int i = 0; i < size(); i++)
        {
            push_back(dp.at(i)); // unclear if this is supposed to be a clone or copy
        }
    }

    DiscardPile &operator+=(Card *card)
    {
        push_back(card);
        return *this;
    }

    Card *pickUp()
    {
        Card *card = back();
        pop_back();
        return card;
    }

    Card *top() const
    {
        if (size() == 0)
            return nullptr;
        else
            return back();
    }

    // I think this is serialization
    void print(std::ostream &) const
    {
        auto it = begin();
        while (it != end())
        {
            std::cout << *it << " ";
            it++;
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const DiscardPile &dp)
    {
        if (dp.size() == 0)
            os << "Discard pile is empty";
        else
            os << dp.top();

        return os;
    };

    // TODO: Implement serialization and deserialization of discard pile
    DiscardPile(std::istream &is, const CardFactory *cf)
    {
        while (is)
        {
            push_back(cf->getFactory()->makeCard(is.get()));
        }
    }
};

// endregion

// region Trade Area

class TradeArea
{
public:
    TradeArea() = default;

    // Just adds a card to the trade area
    TradeArea &operator+=(Card *card)
    {
        cards.push_back(card);
        return *this;
    }

    // Returns true if the card is legal to add to the trade area
    bool legal(Card *card) const
    {
        // TODO: Check if this is the right way to do this

        for (auto &c : cards)
        {
            if (c->getName() == card->getName())
            {
                return false;
            }
        }
        return true;
    }

    // Removes a card of the corresponding bean name from the trade area
    Card *trade(std::string name)
    {
        for (auto it = cards.begin(); it != cards.end(); ++it)
        {
            if ((*it)->getName() == name)
            {
                Card *card = *it;
                cards.erase(it);
                return card;
            }
        }
        return nullptr;
    }

    // Returns the number of cards in the trade area
    int numCards() const { return cards.size(); }

    // TODO: make sure this works
    friend std::ostream &operator<<(std::ostream &os, const TradeArea &tradeArea)
    {
        for (auto card : tradeArea.cards)
        {
            os.put(card->getShortName());
        }

        return os;
    };

    TradeArea(std::istream &is, const CardFactory *cf)
    {
        while (is)
        {
            cards.push_back(cf->getFactory()->makeCard(is.get()));
        }
    };

    // Print s the trade area
    void pprint(std::ostream &os) const
    {
        os << "Trade Area: " << std::endl;
        for (auto card : cards)
        {
            card->pprint(std::cout);
        }
    }

    void removeCard(Card *c)
    {
        cards.erase(std::remove(cards.begin(), cards.end(), c), cards.end());
    }

    std::vector<Card *> cards;
};

// endregion

#endif // BEANS_CARDCONTAINERS_H
