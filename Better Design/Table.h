#pragma once

#ifndef TABLE_H

#include <vector>
#include <string>
#include <unordered_set>

#include "Cards.h"
#include "Deck.h"

class Table
{
private:
    std::string goalString;
    Deck* deck;

public:
    Table(std::map<char, int> cardCounts, Cards *cardInfo, std::string goalString);
    ~Table();
};

#endif