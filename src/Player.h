#ifndef BEANS_PLAYER_H
#define BEANS_PLAYER_H

#include "CardContainers.h"
#include <string>

class Player
{
  public:
    Player() = default;

    explicit Player(std::string name) : name(std::move(name))
    {
    }

    /*
     * Serialize the player to the given stream
     *  The player is serialized as follows:
     *  - length of the name (one byte)
     *  - name
     *  - number of coins
     *  - number of chains
     *  - for each chain:
     *  - character representing each card in the chain
     *  - chain length
     */
    friend std::ostream &operator<<(std::ostream &os, const Player &player)
    {
        // Write the player name
        unsigned char playerNameSize = player.name.size();
        os.write(reinterpret_cast<const char *>(&playerNameSize), sizeof(playerNameSize));

        for (unsigned int i = 0; i < playerNameSize; i++)
        {
            char c = player.name[i];
            os.write(reinterpret_cast<const char *>(&c), sizeof(c));
        }

        os.write(reinterpret_cast<const char *>(&player.numCoins), sizeof(player.numCoins));
        os.write(reinterpret_cast<const char *>(&player.maxNumChains), sizeof(player.maxNumChains));

        int validChains = player.countValidChains();
        os.write(reinterpret_cast<const char *>(&validChains), sizeof(validChains));

        for (auto chain : player.chains)
        {
            if (chain != nullptr)
            {
                os << *chain;
            }
        }

        // Add hand
        os << player.hand;

        return os;
    }

    /*
     * Deserialize the player to the given stream
     *  The player is serialized as follows:
     *  - length of the name (one byte)
     *  - name
     *  - number of coins
     *  - number of chains
     *  - for each chain:
     *  - character representing each card in the chain
     *  - chain length
     *  - hand
     */
    Player(std::istream &is, const CardFactory *factory)
    {
        // Load the name
        unsigned char nameLength;
        is.read((char *)&nameLength, sizeof(nameLength));
        char *nameBuffer = new char[nameLength];
        for (unsigned int i = 0; i < nameLength; i++)
        {
            char c;
            is.read((char *)&c, sizeof(c));
            nameBuffer[i] = c;
        }

        name = std::string(nameBuffer, nameLength);
        delete[] nameBuffer;

        // Load the coins
        is.read((char *)&numCoins, sizeof(numCoins));
        // Load the max num of chains
        is.read((char *)&maxNumChains, sizeof(maxNumChains));
        // Load the chains
        unsigned int numChains;
        is.read((char *)&numChains, sizeof(numChains));
        for (unsigned int i = 0; i < numChains; i++)
        {
            // Load each chain
            char chainType;
            is.read((char *)&chainType, sizeof(chainType));

            chains[i] = ChainFactory::getFactory()->createChain(chainType);

            unsigned char numCards;
            is.read((char *)&numCards, sizeof(numCards));

            chains[i]->chainSize = numCards;
        }

        // Load the hand
        hand = Hand(is, factory);
    }

    std::string getName() const
    {
        return name;
    }

    int getNumCoins() const
    {
        return numCoins;
    }

    Player &operator+=(int c)
    {
        numCoins += c;
        return *this;
    }

    int getMaxNumChains() const
    {
        return maxNumChains;
    }

    //  returns the number of non-zero chains
    int getNumChains() const
    {
        int num = 0;
        for (int i = 0; i < maxNumChains; i++)
        {
            if (chains[i] != nullptr && chains[i]->chainSize > 0)
            {
                num++;
            }
        }
        return num;
    }

    // pays 3 coins to buy third chain
    bool buyThirdChain()
    {
        if (getNumChains() < 3)
        {
            if (numCoins >= 3)
            {
                numCoins -= 3;
                maxNumChains++;
                std::cout << "You've bought a third chain!" << std::endl;
                return true;
            }
            else
            {
                std::cout << "You can't buy a third chain! You need at least 3 coins" << std::endl;
            }
        }
        else
        {
            std::cout << "You've already bought 3 chains!" << std::endl;
            return false;
        }

        if (numCoins >= 3 && getNumChains() < 3)
        {
            numCoins -= 3;
            maxNumChains++;
            return true;
        }
        return false;
    }

    void printHand(std::ostream &out, bool all)
    {
        if (all)
        {
            for (auto card : hand.cards)
            {
                out << card->getName() << " ";
            }
        }
        else
        {
            out << hand.top()->getName();
        }
        out << std::endl;
    }

    int countValidChains() const
    {
        int validChains = 0;
        for (int i = 0; i < maxNumChains; i++)
        {
            if (chains[i] != nullptr)
                validChains++;
        }

        return validChains;
    }

    void printChains(std::ostream &os)
    {
        int i = 0;
        for (auto chain : chains)
        {

            if (chain == nullptr)
            {
                i++;
                continue;
            }

            os << "\t(" << i << ") " << getBeanNameFromChar(chain->chainType()) << "(";
            os << chain->sellValue() << " coins) "
               << ": ";
            for (int s = 0; s < chain->chainSize; ++s)
            {
                os << chain->chainType() << " ";
            }
            os << std::endl;
            i++;
        }
        if (i == 0)
        {
            os << "\tYou don't have any crops :(" << std::endl;
        }
    }

    int chooseChain() const
    {
        return Utils::getRangedValue("In what crop do you want to place this card?", 0, getMaxNumChains() - 1);
    }

    bool playCard(Card *card)
    {
        int choice = chooseChain();

        if (chains[choice] == nullptr) // Create a new crop
        {
            chains[choice] = ChainFactory::getFactory()->createChain(card->getShortName());
            std::cout << "You've planted a " << card->getName() << " crop!" << std::endl;
            return true;
        }
        if (chains[choice]->chainType() == card->getShortName() &&
            chains[choice]->chainSize + 1 <
                getCardsPerCoinMap(card->getShortName(),
                                   4)) // Increment the current crop while not allowing overflow
        {
            *chains[choice] += card;
            std::cout << "You've placed a " << card->getName() << " in the " << chains[choice]->chainType() << " crop."
                      << std::endl;
            return true;
        }
        else
        {
            sellAndReplaceChain(card, choice);
        }

        return false;
    }

    void sellAndReplaceChain(Card *card, int choice)
    {
        int profit = chains[choice]->sellValue();
        std::cout << "You've sold the " << chains[choice]->chainType() << " crop for " << profit << " coins."
                  << std::endl;
        numCoins += profit;
        chains[choice] = ChainFactory::getFactory()->createChain(card->getShortName());
    }

    void plantTop()
    {
        Card *card = hand.play();
        std::string cardName = card->getName();
        std::cout << "You've drawn a... " << cardName << " from the top of your hand" << std::endl;
        playCard(card);
    }

    Card *discardAny()
    {
        // Ask user which card in hard to discard
        std::cout << "Hand: ";
        int i = 0;
        for (auto card : hand.cards)
        {
            std::cout << "(" << i++ << ") " << card->getName() << " | ";
        }
        std::cout << "Which card do you want to discard? [0-" << i - 1 << "]" << std::endl;
        int choice = Utils::getRangedValue("", 0, i - 1);
        return hand[choice];
    }

    Hand hand;
    std::vector<ChainBase *> chains = std::vector<ChainBase *>(3);

  private:
    std::string name;
    int numCoins = 0;
    int maxNumChains = 2;
};

#endif // BEANS_PLAYER_H
