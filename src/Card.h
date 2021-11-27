//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_CARD_H
#define BEANS_CARD_H

// Class declaration macros
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

#include <ostream>
#include <map>

class Card
{
public:
    virtual ~Card() = default;
    ;

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

#endif //BEANS_CARD_H
