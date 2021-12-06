//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_PLAYER_H
#define BEANS_PLAYER_H

#include <string>

class Player
{
  public:
    Player(std::string name) : name(name)
    {
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
     */
    Player(std::istream &is, const CardFactory *factory)
    {
        // Load the name
        unsigned char nameLength;
        is.read((char *)&nameLength, sizeof(nameLength));
        char *nameBuffer = new char[nameLength];
        is.read(nameBuffer, nameLength);
        name = std::string(nameBuffer, nameLength);
        delete[] nameBuffer;

        // Load the coins
        is.read((char *)&numCoins, sizeof(numCoins));
        // Load the max num of chains
        is.read((char *)&maxNumChains, sizeof(maxNumChains));
        // Load the chains
        unsigned int numChains;
        is.read((char *)&numChains, sizeof(numChains));
        for (int i = 0; i < numChains; i++)
        {
            // TODO: Move this to chain constructor
            // Get the chain type
            char chainType;
            is.read((char *)&chainType, sizeof(chainType));
            // Get the chain length
            unsigned char chainLength;
            is.read((char *)&chainLength, sizeof(chainLength));
            // Create and load the chain
            ChainBase *chain = ChainFactory::getFactory()->createChain(chainType);
            chain->chainSize = chainLength;
            chains[i] = chain;
        }
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

    ChainBase *&operator[](int);

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

    void printHand(std::ostream &, bool);

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
        unsigned char playerNameSize = player.name.size();
        os.write(reinterpret_cast<const char *>(&playerNameSize), sizeof(playerNameSize));
        os << player.name;
        os.write(reinterpret_cast<const char *>(&player.numCoins), sizeof(player.numCoins));
        os.write(reinterpret_cast<const char *>(&player.maxNumChains), sizeof(player.maxNumChains));
        unsigned int validChains = 0;
        for (int i = 0; i < player.maxNumChains; i++)
        {
            if (player.chains[i] != nullptr)
            {
                validChains++;
            }
        }
        os.write(reinterpret_cast<const char *>(&validChains), sizeof(validChains));
        for (int i = 0; i < validChains; i++)
        {
            os << player.chains[i]->chainType();
            unsigned char size = player.chains[i]->chainSize;
            os.write(reinterpret_cast<const char *>(&size), sizeof(size));
        }
        return os;
    }

    void printChains(std::ostream &os)
    {
        int i = 0;
        for (auto chain : chains)
        {

            if (chain == nullptr)
            {
                continue;
            }
            os << "(" << i << ") " << chain->chainType() << ": ";
            for (int i = 0; i < chain->chainSize; ++i)
            {
                os << chain->chainType() << " ";
            }
            os << std::endl;
            i++;
        }
    }

    void plantTop()
    {
        Card *card = hand.front();
        hand.pop_front();
        std::string cardName = card->getName();
        // TODO: Force user to use card
        std::cout << "You've drawn a... " << cardName << std::endl;
        std::cout << "In what crop do you want to place this card? [0-";
        std::cout << getMaxNumChains() - 1 << "]" << std::endl;

        bool valid = false;
        while (!valid)
        {
            char choice;
            std ::cin >> choice;
            bool exists = false;
            // Check if choice is between 0 and getMaxNumChains() - 1
            if (choice >= '0' && choice <= '0' + getMaxNumChains() - 1)
            {
                valid = true;
                // Check if we are putting or replacing the crop
                int choiceInt = choice - '0';
                if (chains[choiceInt] == nullptr)
                {
                    chains[choiceInt] = ChainFactory::getFactory()->createChain(card->getShortName());
                    std::cout << "You've planted a " << card->getName() << " crop!" << std::endl;
                }
                if (chains[choiceInt]->chainType() == card->getShortName())
                {
                    // put crop
                    *chains[choiceInt] += card;
                    std::cout << "You've placed a " << card->getName() << " in the " << chains[choiceInt]->chainType()
                              << " crop." << std::endl;
                }
                else
                {
                    // sell crop
                    int profit = chains[choiceInt]->sell();
                    std::cout << "You've sold the " << chains[choiceInt]->chainType() << " crop for " << profit
                              << " coins." << std::endl;
                    numCoins += profit;
                    delete (chains[choiceInt]);
                    chains[choiceInt] = ChainFactory::getFactory()->createChain(card->getShortName());
                }
            }
            else
            {
                std::cout << "Invalid choice. Please try again." << std::endl;
                std::cout << "Input must be between 0 and " << getMaxNumChains() - 1 << std::endl;
            }
        }
    }

    void discardAny()
    {
        // Ask user which card in hard to discard
        std::cout << "Hand: ";
        int i = 0;
        for (auto card : hand)
        {
            std::cout << "(" << i++ << ") " << card->getName() << " | ";
        }
        std::cout << "Which card do you want to discard? [0-" << i - 1 << "]" << std::endl;
        int choice;
        std::cin >> choice;
        bool valid = false;
        while (choice < 0 && choice > i - 1)
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
            std::cout << "Input must be between 0 and " << i - 1 << std::endl;
            std::cin >> choice;
        }
        hand.remove(hand[choice]);
    }

    Hand hand;
    std::vector<ChainBase *> chains = std::vector<ChainBase *>(3);

  private:
    std::string name;
    int numCoins = 0;
    int maxNumChains = 2;
};

void Player::printHand(std::ostream &out, bool all)
{
    if (all)
    {
        for (int i = 0; i < hand.size(); ++i)
        {
            out << hand[i]->getName() << " ";
        }
    }
    else
    {
        out << hand.top()->getName();
    }
}

#endif // BEANS_PLAYER_H
