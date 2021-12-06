//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_TABLE_H
#define BEANS_TABLE_H

#include <string>
#include "Card.h"
#include "CardContainers.h"
#include "Player.h"

void makeCards(Deck *deck, CardFactory *cf, char cardType, int amount)
{
    for (int i = 0; i < amount; i++)
        deck->push_back(cf->makeCard(cardType));
}

class Table
{
public:
    Table();
    // Initialize the table from zero
    Table(std::string p1, std::string p2) : player1(p1),
                                            player2(p2),
                                            deck(),
                                            discardPile()
    {
        InitializeCards();
        deck.shuffle();
        // Give 5 cards to each player
        for (int i = 0; i < 5; i++)
        {
            player1.hand.push_back(deck.draw());
            player2.hand.push_back(deck.draw());
        }
    }

    // Todo : Add a constructor that takes a file name and loads the table from it
    Table(std::istream &, const CardFactory *);

    Table(const TradeArea &tradeArea);

    bool win(std::string &winner) const
    {
        bool won = false;
        if (deck.empty())
        {
            if (player1.getNumCoins() > player2.getNumCoins())
            {
                winner = player1.getName();
                won = true;
            }
            else if (player1.getNumCoins() < player2.getNumCoins())
            {
                winner = player2.getName();
                won = true;
            }
            else
            {
                winner = "It's a tie! Everyone wins! (or loses if you think about it)";
                won = true;
            }
        }

        return won;
    }
    void nextTurn()
    {
        p1Turn = !p1Turn;
    }

    void printHand(bool) const;

    friend std::ostream &operator<<(std::ostream &os, const Table &table);

    Player *getCurrentPlayer()
    {
        if (p1Turn)
            return &player1;
        else
            return &player2;
    }

    bool p1Turn = false;
    Player player2;
    Player player1;
    Deck deck;
    DiscardPile discardPile;
    TradeArea tradeArea;

private:
    void InitializeCards()
    {
        CardFactory *factory = CardFactory::getFactory();
        makeCards(&deck, factory, 'B', 20);
        makeCards(&deck, factory, 'C', 18);
        makeCards(&deck, factory, 'S', 16);
        makeCards(&deck, factory, 'G', 14);
        makeCards(&deck, factory, 's', 12);
        makeCards(&deck, factory, 'b', 10);
        makeCards(&deck, factory, 'R', 8);
        makeCards(&deck, factory, 'g', 6);
    }
};

#endif //BEANS_TABLE_H
