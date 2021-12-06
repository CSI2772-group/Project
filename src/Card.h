#ifndef BEANS_CARD_H
#define BEANS_CARD_H

#include <algorithm>
#include <iostream>
#include <map>
#include <ostream>
#include <random>
#include <vector>

// region Class declaration macros
// I really don't want to type extra 160 loc
// because of the inheritance requirements

#define CARD_CLASS(name, chr, a, b, c, d)                                                                              \
    class name : public Card                                                                                           \
    {                                                                                                                  \
      public:                                                                                                          \
        name()                                                                                                         \
        {                                                                                                              \
        }                                                                                                              \
        ~name() override = default;                                                                                    \
        void print(std::ostream &out) override                                                                         \
        {                                                                                                              \
            out << chr;                                                                                                \
        }                                                                                                              \
        std::string getName() override                                                                                 \
        {                                                                                                              \
            return #name;                                                                                              \
        }                                                                                                              \
        void pprint(std::ostream &out) override                                                                        \
        {                                                                                                              \
            out << getName() << ": " << getCardsPerCoin(1) << " | " << getCardsPerCoin(2) << " | "                     \
                << getCardsPerCoin(3) << " | " << getCardsPerCoin(4) << " | ";                                         \
        }                                                                                                              \
        char getShortName () override                                                                    \
        {                                                                                                              \
            return chr;                                                                                                \
        }                                                                                                              \
        int getCardsPerCoin(int coins) override                                                                        \
        {                                                                                                              \
            switch (coins)                                                                                             \
            {                                                                                                          \
            case 1:                                                                                                    \
                return a;                                                                                              \
            case 2:                                                                                                    \
                return b;                                                                                              \
            case 3:                                                                                                    \
                return c;                                                                                              \
            case 4:                                                                                                    \
                return d;                                                                                              \
            default:                                                                                                   \
                return 0;                                                                                              \
            }                                                                                                          \
        }                                                                                                              \
        int getCoinsPerCard(int cards) override                                                                        \
        {                                                                                                              \
            if (cards >= d)                                                                                            \
                return 4;                                                                                              \
            else if (cards >= c)                                                                                       \
                return 3;                                                                                              \
            else if (cards >= b)                                                                                       \
                return 2;                                                                                              \
            else if (cards >= a)                                                                                       \
                return 1;                                                                                              \
            else                                                                                                       \
                return 0;                                                                                              \
        }                                                                                                              \
        static const char cardType = chr;                                                                                           \
    }

// endregion

// region Card classes

class Card
{
  public:
    static const char cardType = '*';

    virtual ~Card() = default;

    virtual int getCardsPerCoin(int coins)
    {
        return 0;
    }
    virtual int getCoinsPerCard(int cards)
    {
        return 0;
    }

    virtual char getShortName()
    {
        return '*';
    }

    virtual std::string getName() = 0;

    virtual void pprint(std::ostream &out){};

    virtual void print(std::ostream &out){};
};

CARD_CLASS(Blue, 'B', 4, 6, 8, 10);

CARD_CLASS(Chili, 'C', 3, 6, 8, 9);

CARD_CLASS(Stink, 'S', 3, 5, 7, 8);

CARD_CLASS(Green, 'G', 3, 5, 6, 7);

CARD_CLASS(Soy, 's', 2, 4, 6, 7);

CARD_CLASS(Black, 'b', 2, 4, 5, 6);

CARD_CLASS(Red, 'R', 2, 3, 4, 5);

CARD_CLASS(Garden, 'g', 0, 2, 3, 0);

// endregion

// region Card factory
class IllegalType : public std::exception
{
};

class CardFactory
{
    friend class Table;

  public:
    void operator=(CardFactory const &) = delete;

    CardFactory(CardFactory const &) = delete;

    static CardFactory *getFactory()
    {
        static CardFactory factory; // singleton
        return &factory;
    }

    Card *makeCard(char cardType)
    {
        Card *card;
        switch (cardType)
        {
        case 'B':
            card = new Blue();
            break;
        case 'C':
            card = new Chili();
            break;
        case 'S':
            card = new Stink();
            break;
        case 'G':
            card = new Green();
            break;
        case 's':
            card = new Soy();
            break;
        case 'b':
            card = new Black();
            break;
        case 'R':
            card = new Red();
            break;
        case 'g':
            card = new Garden();
            break;
        default:
            throw IllegalType();
        }
        cards.push_back(card);
        return card;
    }

  private:
    CardFactory() = default;

    std::vector<Card *> cards;
};

// endregion

#endif // BEANS_CARD_H
