
#include <fstream>
#include <iostream>

#include "Table.h"
#include "Utils.h"

const std::string beanAsciiArt = "      ████████\n"
                                 "    ██▒▒▒▒▒▒▒▒▒▒██\n"
                                 "  ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
                                 "██▒▒▒▒▒▒▒▒    ▒▒▒▒▒▒██\n"
                                 "██▒▒▒▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒██\n"
                                 "██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████\n"
                                 "██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
                                 "████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
                                 "    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
                                 "      ████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
                                 "          ████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
                                 "              ████████████████\n";

Table *loadFromFile()
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

Table *makeTable()
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

Table *loadOrMakeTable()
{
    // region Save or Load
    Table *loadedTable;
    loadedTable = loadFromFile();
    if (loadedTable)
        return loadedTable;

    else
        return makeTable();
}

// returns deck empty
void drawPlayerCards(Table *table, int num)
{
    Player *currentPlayer = table->getCurrentPlayer();

    for (int i = 0; i < num; i++)
    {
        if (!table->deck.empty())
            currentPlayer->hand += table->deck.draw();
    }
}

int main()
{
    // 🌽 Print corny ascii art 🌽
    std::cout << beanAsciiArt;
    std::cout << "Welcome to the Bean Game!\nLiterally Fall Guys™ except its "
                 "like an obscure card game :/\n";

    Table *table = loadOrMakeTable();

    // region Main Game Loop
    Player *currentPlayer;
    while (!table->deck.empty())
    {
        currentPlayer = table->getCurrentPlayer();
        //          Put 3 cards from top deck into trade area

        table->drawToTradeArea(3);

        // Update trade area from discard pile
        table->updateTradeArea();

        //  TODO: (Optional) Auto add trade cards to chains (later discard them)

        Utils::clearScreen();
        table->pprint(std::cout);

        //  player.play_top_card()
        currentPlayer->plantTop();

        // while (player don't want to end turn)
        //        ask for:
        //            - save and quit // once
        //             - end turn  // once
        //            - player.play_top_card() // once
        //            - player.discard_any()  // once
        //            - player.buy_third_chain() // once
        //            - trade from trade area // unlimited
        bool playedTwice = false;
        bool discarded = false;
        while (true)
        {
            Utils::clearScreen();
            table->pprint(std::cout);
            std::cout << "[e]nd turn\t";
            std::cout << "[s]ave and quit\t"; // TODO: Implement the saving stuff
            std::cout << "[t]rade\t";
            if (!playedTwice)
                std::cout << "[p]lay again\t";
            if (!discarded)
                std::cout << "[d]iscard\t";
            if (currentPlayer->getNumChains() < 3)
            {
                std::cout << "[b]uy third chain\t";
            }
            std::cout << std::endl;

            std::string inputStr = Utils::getLine(1);
            char input = inputStr[0];

            bool doneWithTurn = false;
            switch (input)
            {
            case 'e':
                doneWithTurn = true;
                std::cout << "Ending turn.." << std::endl;
                break;
            case 'p': // Plant again
                if (playedTwice)
                {
                    std::cout << "You can only play twice per turn!" << std::endl;
                    break;
                }
                playedTwice = true;
                currentPlayer->plantTop();
                break;
            case 'd':
                if (discarded)
                {
                    std::cout << "You can only discard once per turn!" << std::endl;
                    break;
                }
                discarded = true;
                table->discardPile.push_back(currentPlayer->discardAny());
                break;
            case 'b':
                currentPlayer->buyThirdChain();
                break;
            case 't':
                // What if trade area is empty
                if (!table->tradeArea.cards.empty())
                {
                    Card *card = table->tradeArea.chooseCard();
                    currentPlayer->playCard(card);
                }
                else
                {
                    std::cout << "The trade area is empty!" << std::endl;
                }
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                break;
            }

            if (doneWithTurn)
            {
                break;
            }
        }

        // Put two cards from deck into player
        drawPlayerCards(table, 2);

        // Flip player's turn
        table->changeTurn();
    }

    // endregion

    // region Check for winner

    // endregion

    return 0;
}
