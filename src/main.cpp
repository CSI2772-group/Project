
#include <iostream>
#include "Card.h"
#include "Table.h"

const std::string beanAsciiArt =
        "      ████████\n"
        "    ██▒▒▒▒▒▒▒▒▒▒██\n"
        "  ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "██▒▒▒▒▒▒▒▒    ▒▒▒▒▒▒██\n"
        "██▒▒▒▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒██\n"
        "██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████\n"
        "██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "  ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "      ████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "          ████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "              ████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██\n"
        "                  ████████████████\n";


int main() {
    // Print ascii art
    std::cout << beanAsciiArt;
    std::cout << "Welcome to the Bean Game!\nLiterally Fall Guys™ except its like an obscure card game :/\n";

    // Ask if new game or load game
    // if(new game)
    // {
    //     Create new game with table and stuff
    //     Give 5 cards to each player
    // } else
    // {
    //     // Load game (deserialize)
    // }

    // while(table.deck.size() > 0)
    // {
    //     for(int i = 0; i < table.players.size(); i++)
    //     {
    //         Print current player's hand
    //         Ask player input
    //         if(input == "exit")
    //             serialize(); // Save game
    //             break;
    //         else if(input == "draw")
    //             drawCard();
    //         if(tradeArea.size() != 0)
    //              Add bean cards from the TradeArea to chains or discard them.
    //         Play topmost card in hand
    //        Check chains
    //        Put stuff on discard pile
    //        AHHH SO MUCH STUFF
    //     }
    // }

    // Print win screen
    return 0;
}