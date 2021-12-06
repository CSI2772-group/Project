//
// Created by fer on 2021-11-20.
//

#ifndef BEANS_PLAYER_H
#define BEANS_PLAYER_H

#include <string>

class Player
{
public:
    Player(std::string &name) : name(name) {}

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

    // returns the number of non-zero chains
    // this makes no sense, either they have chains or they dont
    // chains dont change type, so making new chains is the only thing that makes sense.
    int getNumChains() const
    {
        int num = 0;
        for (int i = 0; i < maxNumChains; i++)
        {
            if (chains[i]->chainSize > 0)
            {
                num++;
            }
        }
        return num;
    }

    void addChain(Card *card)
    {
        ChainBase *chain = ChainFactory::getFactory()->createChain(card->getShortName());
        chain->chainSize = card->getShortName();
        chains.push_back(chain);
    }

    bool hasChainMatching(Card *c)
    {
        for (auto *chain : chains)
        {
            if (chain->chainTypeShort() == c->getShortName())
                return true;
        }

        return false;
    }

    ChainBase *&operator[](int);

    // pays 3 coins to buy third chain
    bool buyThirdChain();

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
        for (auto chain : chains)
        {
            os << chain->chainType() << ": ";
            for (int i = 0; i < chain->chainSize; ++i)
            {
                os << chain->chainType() << " ";
            }
            os << std::endl;
        }
    }

    void removeChain(ChainBase *cb)
    {
        chains.erase(std::remove(chains.begin(), chains.end(), cb), chains.end());
    }

    Hand hand;
    std::vector<ChainBase *> chains;

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

#endif //BEANS_PLAYER_H
