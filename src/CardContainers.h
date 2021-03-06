#ifndef BEANS_CARD_CONTAINERS_H
#define BEANS_CARD_CONTAINERS_H

#include "Card.h"
#include "Utils.h"
#include <list>

// region Deck
class Deck : public std::vector<Card *>
{
  public:
    Deck() = default;

    Card *draw()
    {
        Card *card = back();
        pop_back();
        return card;
    }

    friend std::ostream &operator<<(std::ostream &os, const Deck &deck)
    {
        unsigned char numCards = deck.size();
        os.write(reinterpret_cast<const char *>(&numCards), sizeof(numCards));
        for (auto card : deck)
            os << card->getShortName();
        return os;
    };

    Deck(std::istream &is, const CardFactory *cf)
    {
        unsigned char numCards;
        is.read((char *)&numCards, sizeof(numCards));

        for (unsigned int i = 0; i < numCards; i++)
        {
            char cardType;
            is.read((char *)&cardType, sizeof(cardType));
            push_back(cf->getFactory()->makeCard(cardType));
        }
    };

    void shuffle()
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(begin(), end(), rng);
    }
};
// endregion

// region Hand

class Hand
{
    friend class Player;

  public:
    Hand() = default;

    // Loads the hand from a stream
    Hand(std::istream &is, const CardFactory *cf)
    {
        unsigned char numCards;
        is.read((char *)&numCards, sizeof(numCards));

        for (unsigned int i = 0; i < numCards; i++)
        {
            char cardType;
            is.read((char *)&cardType, sizeof(cardType));
            cards.push_back(cf->getFactory()->makeCard(cardType));
        }
    };

    Hand &operator+=(Card *card)
    {
        cards.push_back(card);
        return *this;
    }

    Card *play()
    {
        Card *c = cards.front();
        cards.pop_front();
        return c;
    }

    Card *top()
    {
        return cards.front();
    }

    Card *operator[](int i)
    {
        // Return i-th card from the hand and remove it from the hand
        auto it = cards.begin();
        std::advance(it, i);
        Card *c = *it;
        cards.erase(it);
        return c;
    }

    friend std::ostream &operator<<(std::ostream &os, const Hand &hand)
    {
        unsigned char numCards = hand.cards.size();
        os.write(reinterpret_cast<const char *>(&numCards), sizeof(numCards));
        for (auto card : hand.cards)
            os << card->getShortName();

        return os;
    };

    std::list<Card *> cards;
};

// endregion

// region Chain
class ChainBase
{
  public:
    virtual int sellValue() = 0;

    virtual ChainBase &operator+=(Card *c) = 0;

    virtual char chainType() const
    {
        return '#';
    }

    friend std::ostream &operator<<(std::ostream &os, const ChainBase &chain)
    {
        char chainType = chain.chainType();
        os.write(reinterpret_cast<const char *>(&chainType), sizeof(chainType));

        unsigned char chainS = chain.chainSize;
        os.write(reinterpret_cast<const char *>(&chainS), sizeof(chainS));

        return os;
    }

    int chainSize = 1; // Start with 1 card

    ChainBase() = default;
    virtual ~ChainBase() = default;
};

// template class that extends "Card"
template <class T> class Chain : public virtual ChainBase
{
    static_assert(std::is_base_of<Card, T>::value, "T must derive from Card");

  public:
    Chain() = default;

    Chain<T> &operator+=(Card *card) override
    {
        // try to cast to card to T
        T *t = dynamic_cast<T *>(card);
        if (t != nullptr)
        {
            // cast successful
            chainSize++;
        }
        else
        {
            // cast failed
            throw IllegalType();
        }
        return *this;
    }

    int sellValue() override
    {
        if (chainSize >= getCardsPerCoinMap(chainType(), 4))
        {
            return 4;
        }
        else if (chainSize >= getCardsPerCoinMap(chainType(), 3))
        {
            return 3;
        }
        else if (chainSize >= getCardsPerCoinMap(chainType(), 2))
        {
            return 2;
        }
        else if (chainSize >= getCardsPerCoinMap(chainType(), 1))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    char chainType() const override
    {
        return T::cardType;
    }
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
        ChainBase *chain;
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
        chains.push_back(chain);
        return chain;
    }

    void DeleteFactory()
    {
        for (auto chain : chains)
        {
            delete chain;
        }
    }

  private:
    ChainFactory() = default;

    std::vector<ChainBase *> chains;
};

// endregion

// region Discard Pile

class DiscardPile : public std::vector<Card *>
{
  public:
    DiscardPile() = default;
    DiscardPile &operator=(const DiscardPile &) = default;

    DiscardPile(const DiscardPile &dp) = default;

    DiscardPile &operator+=(Card *card)
    {
        push_back(card);
        return *this;
    }

    Card *pickUp()
    {
        Card *card = this->back();
        this->pop_back();
        return card;
    }

    Card *top()
    {
        return this->back();
    }

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
        unsigned char numCards = dp.size();
        os.write(reinterpret_cast<const char *>(&numCards), sizeof(numCards));
        for (auto card : dp)
            os << card->getShortName();

        return os;
    }

    DiscardPile(std::istream &is, const CardFactory *)
    {
        unsigned char numCards;
        is.read(reinterpret_cast<char *>(&numCards), sizeof(numCards));
        for (unsigned char i = 0; i < numCards; i++)
        {
            char cardName;
            is.read(&cardName, sizeof(cardName));
            Card *card = CardFactory::getFactory()->makeCard(cardName);
            push_back(card);
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
        for (auto &c : cards)
        {
            if (c->getName() == card->getName())
            {
                return true;
            }
        }
        return false;
    }


    friend std::ostream &operator<<(std::ostream &os, const TradeArea &tradeArea)
    {
        unsigned char numCards = tradeArea.cards.size();
        os.write(reinterpret_cast<const char *>(&numCards), sizeof(numCards));
        for (auto card : tradeArea.cards)
            os << card->getShortName();

        return os;
    };

    TradeArea(std::istream &is, const CardFactory *cf)
    {
        unsigned char numCards;
        is.read((char *)&numCards, sizeof(numCards));

        for (unsigned int i = 0; i < numCards; i++)
        {
            char cardType;
            is.read((char *)&cardType, sizeof(cardType));
            cards.push_back(cf->getFactory()->makeCard(cardType));
        }
    };

    // Print s the trade area
    void pprint(std::ostream &os) const
    {
        os << "Trade Area: ";
        for (auto card : cards)
        {
            std::cout << card->getName() << " ";
        }
        std::cout << std::endl;
    }

    void pprintOptions(std::ostream &os) const
    {
        unsigned int index = 0;
        for (auto card : cards)
        {
            os << "(" << index << ") " << card->getName();
            if (index != cards.size() - 1)
            {
                os << " | ";
            }
            index++;
        }
        os << std::endl;
    }

    Card *chooseCard()
    {
        // Ask user which card in hard to discard
        std::cout << std::endl;
        pprintOptions(std::cout);
        int chosenIndex =
            Utils::getRangedValue("What card from the trade area would you like to pick", 0, (int)(cards.size() - 1));

        // Return card, and remove it from the trade area
        Card *card = cards.at(chosenIndex);
        cards.erase(cards.begin() + chosenIndex);
        return card;
    }

    std::vector<Card *> cards;
};

// endregion

#endif // BEANS_CARD_CONTAINERS_H
