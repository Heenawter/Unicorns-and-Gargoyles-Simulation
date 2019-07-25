#include "Main.h"
#include "Cards.h"

int main()
{
    std::cout << "main" << std::endl;

    Cards cardRules;

    cardRules.readCards();

    std::cout << cardRules.getCardName(17) << std::endl;
}