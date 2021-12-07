//
// Created by fer on 2021-12-03.
//

#ifndef BEANS_UTILS_H
#define BEANS_UTILS_H

#include "Table.h"
#include <cstdlib>
#include <string>

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
    for (int i = 0; i < 10; i++)
        std::cout << std::endl;
}

int getRangedValue(std::string prompt, int min, int max)
{
    std::cout << prompt << "[" << min << ", " << max << "]" << std::endl;
    std::flush(std::cout);

    bool valid = false;
    int choice = -1;
    while (!valid)
    {
        std::string choiceStr = getLine();
        choice = std::stoi(choiceStr);

        if (choice >= min && choice <= max)
            break;
        else
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
            std::cout << "Input must be between " << min << "and " << max << std::endl;
        }
    }

    return choice;
}

} // namespace Utils

#endif // BEANS_UTILS_H
