#include <iostream>
#include "Table.h"

const std::string beanAsciiArt = "      ████████\n"
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

int main()
{
    // 🌽 Print corny ascii art 🌽
    // or beany 🫘
    std::cout << beanAsciiArt;
    std::cout << "Welcome to the Bean Game!\nLiterally Fall Guys™ except its "
                 "like an obscure card game :/\n";


    Table *table = Table::loadOrMakeTable();

    table->playGame();

    return 0;
}
