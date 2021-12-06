//
// Created by fer on 2021-12-03.
//

#ifndef BEANS_UTILS_H
#define BEANS_UTILS_H

#include <string>
#include <cstdlib>

namespace Utils
{

    // Get line of at least three characters
    std::string getLine()
    {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    std::string getLine(int minChars)
    {
        while (true)
        {
            std::string line;
            std::getline(std::cin, line);
            if (line.length() >= minChars)
                return line;
            else
                std::cout << "Please enter at least " << minChars << " characters" << std::endl;
        }
    }

    // Get yes or no
    bool getYesNo(std::string prompt, bool defaultValue)
    {
        std::cout << prompt;
        if (defaultValue)
        {
            std::cout << " [Y/n] :";
        }
        else
        {
            std::cout << " [y/N] :";
        }
        std::string line = getLine();
        if (line.empty())
        {
            return defaultValue;
        }
        switch (line[0])
        {
        case 'y':
        case 'Y':
            return true;
        case 'n':
        case 'N':
            return false;
        default:
            return defaultValue;
        }
    }

    void clearScreen()
    {
        // we could use "clear" or "cls" but it's not portable
        for (int i = 0; i < 50; i++)
            std::cout << std::endl;
    }

    void printTable(Table *table)
    {
        Player *currentPlayer;
        if (table->p1Turn)
        {
            currentPlayer = &table->player1;
        }
        else
        {
            currentPlayer = &table->player2;
        }

        std::cout << "Current turn: " << currentPlayer->getName() << std::endl;
        std::cout << "Deck size: " << table->deck.size() << std::endl;
        std::cout << "Discard pile size: " << table->discardPile.size() << std::endl;
        std::cout << "Crops: ";

        for (auto *chain : currentPlayer->chains)
        {
            std::cout << chain->chainType() << ", "; //TODO: Fix comma
        }
    }

    Table *loadGame(std::ifstream &save)
    {
        // TODO: Load game from file path
        std::cout << "Loading game..." << std::endl;
        return nullptr;
    }

}

#endif // BEANS_UTILS_H
