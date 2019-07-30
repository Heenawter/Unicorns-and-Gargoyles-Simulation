
#include "Table.h"

Table::Table(std::map<char, int> cardCounts, Cards *cardInfo, std::string goalString)
{
    deck = new Deck(cardCounts, cardInfo);
    this->goalString = goalString;
}

Table::~Table()
{
    delete deck;
}