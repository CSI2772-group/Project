
#include <iostream>
#include <fstream>

#include "Table.h"
#include "Utils.h"

const std::string beanAsciiArt =
        "      ████████\n"
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


int main() {
    Table* table;

    // Print corny ascii art  🌽
    std::cout << beanAsciiArt;
    std::cout << "Welcome to the Bean Game!\nLiterally Fall Guys™ except its like an obscure card game :/\n";


    // region Save or Load

    // Ask if user wants to load a game or start a new one
    std::ifstream save("save.bean");
    bool loaded = false;
    if (save.is_open()) {
        std::cout << "Save file found! Do you want to load it? (y/N)"
                  << std::endl;
        char answer;
        std::cin >> answer;
        if (answer == 'y') {
            table = Utils::loadGame(save);
            loaded = true;
        }
    }
    if (!loaded){
        std::string p2Name;
        std::cout << "Starting a new game!" << std::endl;

        // Ask p1 and p2 for names
        std::cout << "Player 1, what is your name?" << std::endl;
        std::string p1Name = Utils::getLine(3);
        std::cout << "Player 2, what is your name?" << std::endl;
        while (true) {
            p2Name = Utils::getLine(3);
            if (p2Name != p1Name) {
                break;
            } else {
                std::cout << "Player 2, please choose a different name!" << std::endl;
            }
        }

        // Create new game
        table = new Table(p1Name, p2Name);
    }

    // endregion

    // region Main Game Loop
    Player* currentPlayer = table->getCurrentPlayer();
    while(!table->deck.empty()){
        //          Put 3 cards from top deck into trade area
        int drawnCount = 0;
        while(drawnCount < 3 && !table->deck.empty()){
            table->tradeArea.cards.push_back(table->deck.back());
            table->deck.pop_back();
            drawnCount++;
        }

        // Update trade area from discard pile
        //        while(discard_pile.top() in trade area)
        //            trade area.add(discard_pile.pop())
        bool inTradeArea = true;
        while(inTradeArea){
            inTradeArea = false;
            Card* topDiscard = table->discardPile.top();
            for(Card* card : table->tradeArea.cards){
                if(card->getName() == topDiscard->getName()){
                    inTradeArea = true;
                    table->tradeArea.cards.push_back(topDiscard);
                    table->discardPile.pop_back();
                    break;
                }
            }
        }

        //  TODO: Auto add trade cards to chains (later discard them)


        // TODO: Ask user input here
        Utils::clearScreen();
        Utils::printTable(table);

        //  player.play_top_card()
        Card* card = currentPlayer->hand.top();
        // TODO: Force user to use card


        // while (player don't want to end turn)
        //        ask for:
        //            - player.play_top_card() // once
        //            - player.discard_any()  // once
        //            - trade from trade area // unlimited
        bool playedTwice = false;
        bool discarded = false;
        while (true){
            // Ask for input

            // TODO: Implement the saving stuff

            std::cout << "[e]nd turn\t";
            if (!playedTwice)
                std::cout << "[p]lay again\t";
            if (!discarded)
                std::cout << "[d]iscard\t";
            std::cout << std::endl;

            char input;
            std::cin >> input;

            bool doneWithTurn = false;
            switch(input){
                case 'e':
                    doneWithTurn = true;
                    std::cout << "Ending turn" << std::endl;
                    break;
                case 'p':
                    if (playedTwice){
                        std::cout << "You can only play twice per turn!" << std::endl;
                        break;
                    }
                    playedTwice = true;
                    // TODO: Play again logic
                    break;
                case 'd':
                    if (discarded){
                        std::cout << "You can only discard once per turn!" << std::endl;
                        break;
                    }
                    discarded = true;
                    // TODO: Discard one card from hand logic
                    break;
                default:
                    std::cout << "Invalid input!" << std::endl;
                    break;
            }

            if (doneWithTurn){
                break;
            }
        }

        // TODO: Put two cards from deck into player

        //  end turn


        // Flip player's turn
        table->p1Turn = !table->p1Turn;
    }

    // endregion

    // region Check for winner

    // endregion

    return 0;
}

