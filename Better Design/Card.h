#pragma once

#ifndef CARD_H // include guard

#include <string>
#include <map>
#include <vector>
#include <algorithm>

class Card
{
private:
    std::string name;
    char type;
    bool isAction;

    void reverse(std::string &current);
    void append(std::string &current, int number);
    void remove(std::string &current, int number);
    void unicorn(std::string &current);
    void gargoyle(std::string &current);

    void discardCard(std::string goalString);
    void springCleaning(std::string goalString);
    void drawNonAction();
    void poisonCard(std::string goalString, std::vector<Player *> &otherPlayers);
    void stealCard(std::string goalString, std::vector<Player *> &otherPlayers);
    void reversePlayOrder();

public:
    void playCard(std::string &current);
    void printCard();
};

#endif
