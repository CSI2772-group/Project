#include "CardFactory.h"
#include "IllegalType.h"

Card *CardFactory::makeCard(char cardType)
{

    if (cardType == 'B')
        return new Blue();
    else if (cardType == 'C')
        return new Chili();
    else if (cardType == 'S')
        return new Stink();
    else if (cardType == 'G')
        return new Green();
    else if (cardType == 's')
        return new Soy();
    else if (cardType == 'b')
        return new Black();
    else if (cardType == 'R')
        return new Red();
    else if (cardType == 'g')
        return new Garden();
    else
        throw IllegalType();
}

/*
CARD_CLASS(Blue, 'B', 4, 6, 8, 10);
CARD_CLASS(Chili, 'C', 3, 6, 8, 9);
CARD_CLASS(Stink, 'S', 3, 5, 7, 8);
CARD_CLASS(Green, 'G', 3, 5, 6, 7);
CARD_CLASS(Soy, 's', 2, 4, 6, 7);
CARD_CLASS(Black, 'b', 2, 4, 5, 6);
CARD_CLASS(Red, 'R', 2, 3, 4, 5);
CARD_CLASS(Garden, 'g', 0, 2, 3, 0);
*/