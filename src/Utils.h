#ifndef BEANS_UTILS_H
#define BEANS_UTILS_H

#include "Table.h"
#include <cstdlib>
#include <string>

namespace Utils
{

std::string getLine()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// Get line of at least minChars characters
std::string getLine(int minChars)
{
    while (true)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line.length() >= (size_t)minChars)
            return line;
        else
            std::cout << "Please enter at least " << minChars << " characters" << std::endl;
    }
}

// Get yes or no
bool getYesNo(const std::string &prompt, bool defaultValue)
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
    // If debug mode clear 10 lines, if release mode, clear 100 lines
#ifdef DEBUG
    for (int i = 0; i < 10; i++)
#else
    for (int i = 0; i < 100; i++) // we could use "clear" or "cls" but it's not portable
#endif
        std::cout << std::endl;
}

// Get a number from the user between min and max (inclusive)
int getRangedValue(const std::string &prompt, int min, int max)
{
    std::cout << prompt << "[" << min << ", " << max << "]" << std::endl;
    std::flush(std::cout);

    int choice;
    while (true)
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
