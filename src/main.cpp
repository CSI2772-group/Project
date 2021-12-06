
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

enum PlayerChoice
{
    PlayCard,
    Discard,
};

Table *loadFromFile()
{
    std::ifstream save("save.bean");

    if (save.is_open())
    {
        std::cout << "Save file found! Do you want to load it? (y/N)"
                  << std::endl;
        char answer;
        std::cin >> answer;
        if (answer == 'y')
            return Utils::loadGame(save);
        else
            return nullptr;
    }
}

Table *makeTable()
{
    std::string p2Name;
    std::cout << "Starting a new game!" << std::endl;

    // Ask p1 and p2 for names
    std::cout << "Player 1, what is your name?" << std::endl;
    std::string p1Name = Utils::getLine(3);
    std::cout << "Player 2, what is your name?" << std::endl;
    while (true)
    {
        p2Name = Utils::getLine(3);
        if (p2Name != p1Name)
        {
            break;
        }
        else
        {
            std::cout << "Player 2, please choose a different name!" << std::endl;
        }
    }

    // Create new game
    return new Table(p1Name, p2Name);
}

Table *loadOrMakeTable()
{
    // Print corny ascii art  🌽
    std::cout << beanAsciiArt;
    std::cout << "Welcome to the Bean Game!\nLiterally Fall Guys™ except its like an obscure card game :/\n";

    // region Save or Load
    Table *loadedTable;
    loadedTable = loadFromFile();
    if (loadedTable)
        return loadedTable;

    else
        return makeTable();
}

void unDiscard(Table *table)
{
    while (!table->discardPile.empty())
    {
        if (table->tradeArea.legal(table->discardPile.top()))
        {
            table->tradeArea += table->discardPile.pickUp();
        }
    }
}

void drawPlayerCards(Table *table, int num)
{
    Player *currentPlayer = table->getCurrentPlayer();

    for (int i = 0; i < num; i++)
    {
        if (!table->deck.empty())
            currentPlayer->hand += table->deck.draw();
    }
}

void drawToTradeArea(Table *table, int num)
{
    for (int i = 0; i < num; i++)
    {
        if (!table->deck.empty())
            table->tradeArea += table->deck.draw();
    }
}

bool askYesNo(std::string message)
{
    char c;

    std::cout << message << " (y/n)\n";
    std::cin >> c;

    while (c != 'y' && c != 'n')
    {
        std::cout << "That is not a valid input, enter \"y\" or \"n\"\n";
        std::cout << message << " (y/n)\n";
        std::cin >> c;
    }

    return (c == 'y');
}

Card *chooseCard(std::vector<Card *> cardSet)
{
    char entry;

    std::vector<char> options;

    std::cout << "Select a card:\n"
              << "Card Name (character entry)\n";
    for (auto *c : cardSet)
    {
        options.push_back(c->getShortName());
        std::cout << c->getName() << "(" << c->getShortName() << ")\n";
        std::cin >> entry;
    }

    while (std::count(options.begin(), options.end(), entry) == 0)
    {
        std::cout << "That is not a valid card in the set, try again:\n";
        std::cin >> entry;
    }

    Card *selection;
    for (auto *c : cardSet)
    {
        if (c->getShortName() == entry)
            selection = c;
    }

    return selection;
}

ChainBase *chooseChain(std::vector<ChainBase *> chainSet)
{
    char entry;

    std::vector<char> options;

    std::cout << "Select a chain:\n"
              << "Chain Name (chain entry)\n";
    for (auto *c : chainSet)
    {
        options.push_back(c->chainTypeShort());
        std::cout << c->chainType() << "(" << c->chainTypeShort() << ")\n";
        std::cin >> entry;
    }

    while (std::count(options.begin(), options.end(), entry) == 0)
    {
        std::cout << "That is not a chain you posess, try again:\n";
        std::cin >> entry;
    }

    ChainBase *selection;
    for (auto *c : chainSet)
    {
        if (c->chainTypeShort() == entry)
            selection = c;
    }

    return selection;
}

void playerDiscard(Player *player)
{
    if (askYesNo("Would you like to discard a card?"))
    {
        Card *selection = chooseCard(player->hand.getUniqueCards());
        player->hand.remove(selection);
    }
}

bool playerChainCard(Player *player, Card *card)
{
    Card *topCard = player->hand.top();

    for (auto *chain : player->chains)
    {
        if (chain->chainTypeShort() == topCard->getShortName())
        {
            *chain += card;
            return true;
        }
    }

    return false;
}

bool playerSellChain(Player *player, Card *card, bool allowQuit)
{
    std::cout << "Unfortunately, you have no chains that match card...\n";

    // allow player to do nothing if they are allowed.
    // this would happen if the player chose to play the top card, but did
    // not realize that they had no matching chains for it.
    if (allowQuit && !askYesNo("Would you like to sell a chain?"))
        return false; //player did not sell the chain

    std::cout << "Choose a chain to discard\n";

    ChainBase *selection = chooseChain(player->chains);
    player += selection->sell();
    player->removeChain(selection);

    ChainBase *newChain = ChainFactory::getFactory()->createChain(card->getShortName());
    player->chains.push_back(newChain);

    return true; //player sold the chain
}

bool playCard(Player *player, Card *card, bool allowQuit)
{
    //return whether or not player played card

    //card must be added to chain of same type.
    //if there is no chain, one of the chains has to be discarded

    if (!playerChainCard(player, card))
        return playerSellChain(player, card, allowQuit);
    else
        return true; //player was able to chain card
}

void playTopCardLoop(Player *player)
{
    Card *card = player->hand.top();
    playCard(player, card, false);
    player->hand.play();
    while (askYesNo("Would you like to play the top card?"))
    {
        playCard(player, card, true);
        player->hand.play();
    }
}

void tradeCards(Table *table, bool discard)
{
    Player *player = table->getCurrentPlayer();

    if (discard)
        std::cout << "You have the option to chain or discard cards from the trading area.\n";
    else
        std::cout << "You have the option to chain cards from the trading area or leave\n"
                  << "them for the other player.\n";

    std::cout << "Please choose whether or not you would like to chain each card.";
    table->tradeArea.pprint(std::cout);

    for (auto *c : table->tradeArea.cards)
    {
        std::cout << "Card: " << c->getName() << "\n";
        bool doChain;
        if (player->hasChainMatching(c))
            doChain = askYesNo("Would you like to chain this card? You have a matching chain.");
        else
            doChain = askYesNo("Would you like to chain this card? You do not have a matching chain.");

        bool wasPlayed; //did the player change their mind and not play the card?
        if (doChain)
            wasPlayed = playCard(player, c, true);

        if (!doChain || !wasPlayed)
            table->discardPile += c;

        // remove card in any case.
        table->tradeArea.removeCard(c);
    }
}

void runPlayerTurn(Table *table)
{
    Player *currentPlayer = table->getCurrentPlayer();

    //allow trading
    tradeCards(table, true);

    //play topmost card as many times as player wishes (minimum 1)
    playTopCardLoop(currentPlayer);

    //option to discard one arbitrary card
    playerDiscard(currentPlayer);

    //draw 3 cards, place in trade area.
    drawToTradeArea(table, 3);

    //draw from discardPile and match tradeArea
    unDiscard(table);

    //either chain cards, or leave them for next player
    tradeCards(table, false);

    //draw 2 cards and place at back
    drawPlayerCards(table, 2);
}

void handleGameEnd(Table *table)
{
    Player p1 = table->player1;
    Player p2 = table->player2;

    if (p1.getNumCoins() > p2.getNumCoins())
        std::cout << "Player 1 (" << p1.getName() << ") wins!\n"
                  << p1.getNumCoins() << " coins - " << p2.getNumCoins() << " coins";
    else if (p2.getNumCoins() > p1.getNumCoins())
        std::cout << "Player 2 (" << p2.getName() << ") wins!\n"
                  << p2.getNumCoins() << " coins - " << p1.getNumCoins() << " coins";
    else
        std::cout << "Tie game!\n"
                  << p1.getNumCoins() << " coins - " << p2.getNumCoins() << " coins";
}

int main()
{
    Table *table = loadOrMakeTable();
    // endregion

    while (!table->deck.empty())
    {
        //  TODO: Auto add trade cards to chains (later discard them)

        // Display Table
        Utils::clearScreen();
        Utils::printTable(table);

        runPlayerTurn(table);

        // Flip player's turn
        table->nextTurn();
    }

    // endregion

    handleGameEnd(table);

    return 0;
}
