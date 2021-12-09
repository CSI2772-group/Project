# Card Game Project

Course Code: CSI2772A 
Course Date: Fall 2021

## Contributors

Nom étudiant: Fernando Nogueira Numéro d’étudiant: 300195829

Nom étudiant: Ben Mclean Numéro d’étudiant: 300015866

## How to run

If you're using CMake (3.15+) you can use the following commands to build and compile everything:

> cmake -B build -S .
>
> cmake --build build --target beans

If everything works well, your binaries should be in the `build` directory

## Project Instructions

This is a modified version of Bohnanza, the goal of the game is to be the player with the most amount of coins when the game ends. The game ends when there are no more cards in the deck. Players gain coins by creating sets or "chains" by grouping cards of the same type. Players can trade cards, and are able to discard cards they believe are not desirable. Players sell chains to gain one, two, three, or four coins. Each type of card has an associated requirement for one, two, three and four cards. Each card is named after a type of bean.
