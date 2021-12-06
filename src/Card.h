#ifndef BEANS_CARD_H
#define BEANS_CARD_H

#include <ostream>
#include <map>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

// region Class declaration macros
// I really don't want to type extra 160 loc
// because of the inheritance requirements

#define CARD_CLASS(name, chr, a, b, c, d)                               \
    class name : public Card                                            \
    {                                                                   \
    public:                                                             \
        name() {}                                                       \
        ~name() override = default;                                     \
        static void const print(std::ostream &out)  { out << chr; }     \
        static std::string const getName()  { return #name; }           \
        static void const pprint(std::ostream &out)  {                  \
          out << getName() << ": " << getCardsPerCoin(1) <<  " | "      \
          << getCardsPerCoin(2) << " | " << getCardsPerCoin(3) << " | " \
          << getCardsPerCoin(4) << " | "  ; }                           \
        static char const getShortName()  { return chr; }               \
        static int const getCardsPerCoin(int coins)                     \
        {                                                               \
            switch (coins)                                              \
            {                                                           \
            case 1:                                                     \
                return a;                                               \
            case 2:                                                     \
                return b;                                               \
            case 3:                                                     \
                return c;                                               \
            case 4:                                                     \
                return d;                                               \
            default:                                                    \
                return 0;                                               \
            }                                                           \
        }                                                               \
        static int const getCoinsPerCard(int cards)                     \
        {                                                               \
            if(cards >= d)                                              \
                return 4 ;                                              \
            else if (cards >= c)                                        \
                return 3;                                               \
            else if (cards >= b)                                        \
                return 2;                                               \
            else if (cards >= a)                                        \
                return 1;                                               \
            else                                                        \
                return 0;                                               \
        }                                                               \
        char cardType = chr;                                            \
    }

// endregion

// region Card classes

class Card {
public:
    virtual ~Card() = default;

    static const int getCardsPerCoin(int coins) { return 0; }
    static const int getCoinsPerCard(int cards) { return 0; }

    static const char getShortName() { return '\0'; }

    static const std::string getName() { return ""; }

    static const void pprint(std::ostream &out) {};

    static const void print(std::ostream &out) {};
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
class IllegalType : public std::exception {
};

class CardFactory {
    friend class Table;

public:
    void operator=(CardFactory const &) = delete;

    CardFactory(CardFactory const &) = delete;

    static CardFactory *getFactory() {
        static CardFactory factory; // singleton
        return &factory;
    }

    Card *makeCard(char cardType) {
        Card *card;
        switch (cardType) {
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

#endif //BEANS_CARD_H
