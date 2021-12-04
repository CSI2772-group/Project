//
// Created by fer on 2021-11-29.
//

#ifndef BEANS_BEANFSM_H
#define BEANS_BEANFSM_H

#include "Card.h"
#include "CardContainers.h"
#include "Table.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <sstream>

class RoundState {
public:
  int drawnCards = 0;

};

class BeanFSM {
public:
  void start() {
    state = &BeanFSM::loadNewGameState;
    currentPlayer = nullptr;
    loadNewGameState();
    while (state != nullptr) {
      Utils::clearScreen();
      std::cout << "Current turn: " << currentPlayer->getName() << std::endl;
      std::cout << "Deck size: " << table->deck.size() << std::endl;
      std::cout << "Discard pile size: " << table->discardPile.size() << std::endl;
      std::cout << "Crops: " << std::endl;
      table->player1.printHand()
  //      table->discardPile.print(std::cout);
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
      std::cout << "Save file found! Do you want to load it? (y/N)"
                << std::endl;
      char answer;
      std::cin >> answer;
      if (answer == 'y') {
        loadGame(save);
        state = &BeanFSM::playGameState;
        return;
      }
    }

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
    state = &BeanFSM::playGameState;
    currentPlayer = &table->player1;
  }

  void playGameState() {
    if (table->p1Turn) {
      currentPlayer = &table->player1;
    } else {
      currentPlayer = &table->player2;
    }
    table->p1Turn = !table->p1Turn;

    std::cout << std::endl;

    // Ask if player wants to quit
    bool quit = Utils::getYesNo("Do you want to save and quit?", false);
    if (quit) {
      state = nullptr; // TODO: save game
      return;
    }
  }

  void drawCardState(){
    // Card drawn
    Card* c = currentPlayer->hand.play();
    std::cout << "You drew a " << c->getName() << "!" << std::endl;
    // What to do next?

  }

  void tradeAskState() {
    if (table->tradeArea.numCards() > 0) {
      // Print trade area
      table->tradeArea.pprint(std::cout);

      // Player may now add one or more cards to the trade area to his crops.
      // Automatically add all crops matching chains from the trade area to the
      // player's crops.
      auto tradeCardIter = table->tradeArea.cards.begin();
      while (tradeCardIter != table->tradeArea.cards.end()) {
        auto chainIter = table->player1.chains.begin();
        while (chainIter != table->player1.chains.end()) {
          ChainBase*chainVal = *chainIter;
          Card *tradeCardVal = *tradeCardIter;
          if (chainVal->chainType() == tradeCardVal->getShortName()) {
            // Remove card from trade area and add to player's crops
            table->tradeArea.cards.erase(tradeCardIter);
            long iterCnt = std::distance(table->player1.chains.begin(),
                                        chainIter);
            ChainBase* chain = table->player1.chains.at(iterCnt);
            *chain += tradeCardVal;
            // Print what was added
            std::cout << "Automatically added " << tradeCardVal->getName() <<
                " from the trade area to your chains" << std::endl;
            break;
          }
          chainIter++;
        }
      }

      // TODO: Ask player if he wants to trade cards manually


    }

    state = &BeanFSM::drawCardState;
  }

  // endregion

  void loadGame(std::ifstream &save) {
    // TODO: Load game from file path
    std::cout << "Loading game..." << std::endl;
    state = nullptr;
  }

  // saveFile.open(entry.path().string());
  void (BeanFSM::*state)(void);
  Table *table;
  Player *currentPlayer;
};

#endif // BEANS_BEANFSM_H
