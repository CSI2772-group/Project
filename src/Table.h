//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_TABLE_H
#define BEANS_TABLE_H

#include "Card.h"
#include "CardContainers.h"
#include "Player.h"
#include <string>

#define MAKE_CARDS(deck, factory, cardType, amount)                                                                    \
    for (int i = 0; i < amount; i++)                                                                                   \
        deck.push_back(factory->makeCard(cardType));

class Table
{
  public:
    // Initialize the table from zero
    Table(std::string p1, std::string p2) : player1(p1), player2(p2), deck(), discardPile()
    {
        InitializeCards();
        deck.shuffle();
        // Give 5 cards to each player
        for (int i = 0; i < 5; i++)
        {
            player1.hand.cards.push_back(deck.draw());
            player2.hand.cards.push_back(deck.draw());
        }
    }

    // Todo : Add a constructor that takes a file name and loads the table from it
    Table(std::istream &, const CardFactory *) : player1("a"), player2("b")
    {
    }

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

    void changeTurn()
    {
        p1Turn != p1Turn;
    }

    void updateTradeArea()
    {
        while (!discardPile.empty())
        {
            if (tradeArea.legal(discardPile.top()))
            {
                tradeArea += discardPile.pickUp();
            }
        }
    }

    void drawToTradeArea(int num)
    {
        for (int i = 0; i < num; i++)
        {
            if (!deck.empty())
                tradeArea += deck.draw();
        }
    }

    void printHand(bool) const;

    friend std::ostream &operator<<(std::ostream &, const Table &);

    Player *getCurrentPlayer()
    {
        if (p1Turn)
        {
            return &player1;
        }
        else
        {
            return &player2;
        }
    }

    bool p1Turn = true;
    Player player2;
    Player player1;
    Deck deck;
    DiscardPile discardPile;
    TradeArea tradeArea;

  private:
    void InitializeCards()
    {
        CardFactory *factory = CardFactory::getFactory();
        MAKE_CARDS(deck, factory, 'B', 20);
        MAKE_CARDS(deck, factory, 'C', 18);
        MAKE_CARDS(deck, factory, 'S', 16);
        MAKE_CARDS(deck, factory, 'G', 14);
        MAKE_CARDS(deck, factory, 's', 12);
        MAKE_CARDS(deck, factory, 'b', 10);
        MAKE_CARDS(deck, factory, 'R', 8);
        MAKE_CARDS(deck, factory, 'g', 6);
    }
};

#endif // BEANS_TABLE_H
