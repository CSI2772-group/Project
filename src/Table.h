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

    friend std::ostream &operator<<(std::ostream &os, const Table &table)
    {
        os << table.player1;
        os << table.player2;
        os << table.deck;
        os << table.discardPile;
        os << table.tradeArea;
    };

    // Todo : Add a constructor that takes a file name and loads the table from it
    Table(std::ifstream &save, const CardFactory *cf)
    {
        // must be the same order as ostream operation
        player1 = Player(save, cf);
        player2 = Player(save, cf);
        deck = Deck(save, cf);
        discardPile = DiscardPile(save, cf);
        tradeArea = TradeArea(save, cf);
    }

    Table(const TradeArea &tradeArea) = delete; // Disable copy constructor

    // builder method to load table from file
    static Table *loadFromFile()
    {
        std::ifstream save("save.bean");

        if (save.is_open())
        {
            std::cout << "Save file found! Do you want to load it? (y/N)" << std::endl;
            std::string answerStr = Utils::getLine(1);
            char answer = answerStr[0];
            if (answer == 'y')
                return new Table(save, CardFactory::getFactory());
        }

        return nullptr;
    }

    // builder method to make a new table
    static Table *makeTable()
    {
        std::string p2Name;
        std::cout << "Starting a new game!" << std::endl;

        // Ask p1 and p2 for names
        std::cout << "Player 1, what is your name?" << std::endl;
        std::string p1Name = Utils::getLine(3);
        std::cout << "Player 2, what is your name?" << std::endl;
        while (true)
        {
            p2Name = Utils::getLine(3);
            if (p2Name != p1Name)
            {
                break;
            }
            else
            {
                std::cout << "Player 2, please choose a different name!" << std::endl;
            }
        }

        // Create new game
        return new Table(p1Name, p2Name);
    }

    // builder method to load or make table
    static Table *loadOrMakeTable()
    {
        // region Save or Load
        Table *loadedTable;
        loadedTable = loadFromFile();
        if (loadedTable)
            return loadedTable;

        else
            return makeTable();
    }

    char printTurnPrompt()
    {
        Utils::clearScreen();
        pprint(std::cout);
        std::cout << "[e]nd turn\t";
        std::cout << "[s]ave and quit\t"; // TODO: Implement the saving stuff
        std::cout << "[t]rade\t";
        if (!playedTwice)
            std::cout << "[p]lay again\t";
        if (!discarded)
            std::cout << "[d]iscard\t";
        if (getCurrentPlayer()->getNumChains() < 3)
        {
            std::cout << "[b]uy third chain\t";
        }

        std::string inputStr = Utils::getLine(1);
        return inputStr[0];
    }

    void handleEndTurn()
    {
        doneTurn = true;
        std::cout << "Ending turn..." << std::endl;
    }

    void handlePlantAgain()
    {
        if (playedTwice)
        {
            std::cout << "You can only play twice per turn!" << std::endl;
        }
        else
        {
            playedTwice = true;
            getCurrentPlayer()->plantTop();
        }
    }

    void handleDiscard()
    {
        if (discarded)
        {
            std::cout << "You can only discard once per turn!" << std::endl;
        }
        else
        {
            discarded = true;
            discardPile.push_back(getCurrentPlayer()->discardAny());
            ;
        }
    }

    void handleTrade()
    {
        if (!tradeArea.cards.empty())
        {
            Card *card = tradeArea.chooseCard();
            getCurrentPlayer()->playCard(card);
        }
        else
        {
            std::cout << "The trade area is empty!" << std::endl;
        }
    }

    void handleSave()
    {
        std::ofstream save("save.bean");
        if (save.is_open())
        {
            save << *this;
            std::cout << "Game saved!" << std::endl;
        }
        else
        {
            std::cout << "Could not save game!" << std::endl;
        }
    }

    void handleDecision(char input)
    {
        switch (input)
        {
        case 'e':
            handleEndTurn();
            break;
        case 'p':
            handlePlantAgain();
            break;
        case 'd':
            handleDiscard();
            break;
        case 'b':
            getCurrentPlayer()->buyThirdChain();
            break;
        case 's':
            handleSave();
            break;
        case 't':
            handleTrade();
            break;
        default:
            std::cout << "Invalid input!" << std::endl;
            break;
        }
    }

    void playNextTurn()
    {
        drawToTradeArea(3);

        updateTradeArea();

        Utils::clearScreen();
        pprint(std::cout);

        getCurrentPlayer()->plantTop();

        while (!doneTurn)
        {
            char input = printTurnPrompt();
            handleDecision(input);
        }

        // Put two cards from deck into player
        drawPlayerCards(2);

        // Flip player's turn
        changeTurn();
    }

    void playGame()
    {
        while (!deck.empty())
            playNextTurn();
    }

    void changeTurn()
    {
        doneTurn = false;
        playedTwice = false;
        p1Turn = !p1Turn;
    }

    void updateTradeArea()
    {
        while (!discardPile.empty())
        {
            if (tradeArea.legal(discardPile.top()))
            {
                tradeArea += discardPile.pickUp();
            }
            else
            {
                return;
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

    void handleGameEnd() const
    {
        std::cout << std::endl << "No more cards in the deck, Game over!" << std::endl;

        // short names
        Player p1 = player1;
        Player p2 = player2;
        int c1 = player1.getNumCoins();
        int c2 = player2.getNumCoins();
        std::string n1 = player1.getName();
        std::string n2 = player2.getName();

        if (c1 > c2)
            std::cout << n1 << " wins with " << c1 << " coins against " << c2 << "!" << std::endl;
        else if (c1 < c2)
            std::cout << n2 << " wins with " << c2 << " coins against " << c1 << "!" << std::endl;
        else
            std::cout << "It's a tie! Everyone wins! (or loses if you think about it)" << std::endl;
    }

    void drawPlayerCards(int num)
    {
        Player *currentPlayer = getCurrentPlayer();

        for (int i = 0; i < num; i++)
        {
            if (!deck.empty())
                currentPlayer->hand += deck.draw();
        }
    }

    void pprint(std::ostream &out)
    {
        Player *currentPlayer = getCurrentPlayer();

        out << "Current turn: " << currentPlayer->getName() << std::endl;
        out << "Deck size: " << deck.size() << std::endl;
        out << "Discard pile size: " << discardPile.size() << std::endl;
        tradeArea.pprint(out);
        out << std::endl;
        out << "Your Crops: " << std::endl;
        // Print current player crops
        currentPlayer->printChains(out);
        out << std::endl;
        out << "Hand: ";
        // Print current player hand
        currentPlayer->printHand(out, true);
        out << "Your Coins: " << currentPlayer->getNumCoins() << std::endl;
        out << std::endl;
    }

    // state variables to run current turn
    bool wantsToSave = false; // TODO when player wants to save, at the end of his turn, flip p1Turn and save game
    bool discarded = false;
    bool doneTurn = false;
    bool playedTwice = false;

    bool p1Turn = true;
    Player player1;
    Player player2;
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
