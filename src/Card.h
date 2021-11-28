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

#define CARD_CLASS(name, chr, a, b, c, d)                      \
    class name : public Card                                   \
    {                                                          \
    public:                                                    \
        name() {}                                              \
        ~name() override = default;                            \
        void print(std::ostream &out) override { out << chr; } \
        std::string getName() override { return #name; }       \
        int getCardsPerCoin(int coins) override                \
        {                                                      \
            switch (coins)                                     \
            {                                                  \
            case 1:                                            \
                return a;                                      \
            case 2:                                            \
                return b;                                      \
            case 3:                                            \
                return c;                                      \
            case 4:                                            \
                return d;                                      \
            default:                                           \
                return 0;                                      \
            }                                                  \
        }                                                      \
        char cardType = chr;                                   \
    }

// endregion

// region Card classes

class Card {
public:
    virtual ~Card() = default;;

    virtual int getCardsPerCoin(int coins) = 0;

    virtual std::string getName() = 0;

    virtual void print(std::ostream &out) = 0;
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
        if (cardType == 'B')
            card = new Blue();
        else if (cardType == 'C')
            card = new Chili();
        else if (cardType == 'S')
            card = new Stink();
        else if (cardType == 'G')
            card = new Green();
        else if (cardType == 's')
            card = new Soy();
        else if (cardType == 'b')
            card = new Black();
        else if (cardType == 'R')
            card = new Red();
        else if (cardType == 'g')
            card = new Garden();
        else
            throw IllegalType();
        cards.push_back(card);
        return card;
    }

private:
    CardFactory() = default;
    std::vector<Card *> cards;
};


// endregion

#endif //BEANS_CARD_H
