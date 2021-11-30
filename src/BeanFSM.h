//
// Created by fer on 2021-11-29.
//

#ifndef BEANS_BEANFSM_H
#define BEANS_BEANFSM_H

#include "Card.h"
#include "Table.h"
#include "CardContainers.h"
#include "Table.h"

#include <iostream>
#include <fstream>
#include <sstream>

class BeanFSM {
public:


    void start() {
        state = &BeanFSM::loadNewGameState;
        while (state != nullptr) {
            (*this.*state)();
        }
        std::cout << "Goodbye!" << std::endl;
    }

private:

    // region States

    void loadNewGameState() {
        // Ask if user wants to load a game or start a new one
        std::ifstream save("save.bean");
        if (save.is_open()) {
            std::cout << "Save file found! Do you want to load it? (y/N)" << std::endl;
            char answer;
            std::cin >> answer;
            if (answer == 'y'){
                loadGame(save);
                state = &BeanFSM::playGameState;
                return;
            }
        }

        std::cout << "Starting a new game!" << std::endl;


        std::string p1Name;
        std::string p2Name;
        // Ask p1 and p2 for names
        while (true) {
            std::cout << "Player 1, what is your name?" << std::endl;

            std::cin >> p1Name;
            if (p1Name.length() > 0) {
                break;
            } else {
                std::cout << "Invalid name" << std::endl;
            }
        }

        while (true) {
            std::cout << "Player 2, what is your name?" << std::endl;
            std::cin >> p2Name;
            if (p2Name.length() > 0) {
                break;
            } else {
                std::cout << "Invalid name" << std::endl;
            }
        }

        // Create new game
        table = new Table(p1Name, p2Name);
        state = &BeanFSM::playGameState;

    }

    void playGameState() {
        // std::cout << table->player1;
        std::cout << "After setting up the game, we play it on this state" << std::endl;

        // Ask if player wants to quit
        std::cout << "Do you want to quit? (y/N)" << std::endl;
        char answer;
        std::cin >> answer;
        if (answer == 'y') {
            state = nullptr;
            return;
        }
    }

    // endregion

    void loadGame(std::ifstream &save) {
        // TODO: Load game from file path
        std::cout << "Loading game..." << std::endl;
        state = nullptr;
    }




//saveFile.open(entry.path().string());
    void (BeanFSM::* state)(void);
    Table *table;
};


#endif //BEANS_BEANFSM_H
