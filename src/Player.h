//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_PLAYER_H
#define BEANS_PLAYER_H

#include <string>
#include "Chain.h"
#include "CardFactory.h"

class Player {
public:
    Player(std::string &name) : name(name) {}

    Player(std::istream &, const CardFactory *);

    std::string getName() const {
        return name;
    }

    int getNumCoins() const {
        return numCoins;
    }

    Player &operator+=(int c) {
        numCoins += c;
        return *this;
    }

    int getMaxNumChains() const {
        return maxNumChains;
    }

    //  returns the number of non-zero chains
    int getNumChains() const {
        int num = 0;
        for (int i = 0; i < maxNumChains; i++) {
            if (chains[i]->size() > 0) {
                num++;
            }
        }
        return num;
    }


    ChainBase *&operator[](int);

    void buyThirdChain();

    void printHand(std::ostream &, bool);

    friend std::ostream &operator<<(std::ostream &, const Player &);

private:
    std::string name;
    int numCoins = 0;
    int maxNumChains = 2;
    ChainBase *chains[3]; // HACK:FIXME solve this template problem
};

#endif //BEANS_PLAYER_H
