#pragma once

#ifndef CARD_H // include guard

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

static const char UNICORN = 0, GARGOYLE = 1, REVERSE = 2, ROTATE_R = 3, ROTATE_L = 4, DOUBLE = 5,
                  APPEND_1 = 6, APPEND_2 = 7, APPEND_3 = 8, REMOVE_1 = 9, REMOVE_2 = 10, REMOVE_3 = 11;
static const char ACTION_CARD_DISCARD = 12;
static const char ACTION_CARD_DRAW = 13;
static const char ACTION_CARD_SPRING_CLEANING = 14;
static const char ACTION_CARD_REVERSE = 15;
static const char ACTION_CARD_POISON = 16;
static const char ACTION_CARD_STEAL = 17;

class Cards
{
private:
    std::map<char, std::string> cardMap; // <card type, long name>

    void reverse(std::string &current);
    void rotateRight(std::string &current);
    void rotateLeft(std::string &current);
    void doubleString(std::string &current);
    void append(std::string &current, int number);
    void remove(std::string &current, int number);
    void unicorn(std::string &current);
    void gargoyle(std::string &current);

    void playCard(char type, std::string &current);
    void addToMaps(std::string line, char cardType);

public:
    Cards(std::map<char, std::string> cardMap);

    std::string generateString(std::vector<char> cardList);
    std::string getCardName(char type);
};

#endif
