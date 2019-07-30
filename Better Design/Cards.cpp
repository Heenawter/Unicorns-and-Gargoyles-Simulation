#include "Cards.h"
#include "Constants.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

void Cards::reverse(std::string &current)
{
    int left, right;

    // only need to swap every "third" character, ignoring '[' and ']'
    if (!current.empty())
    {
        left = 1;                     // right-most index to swap
        right = current.length() - 2; // left-most index to swap

        // while still things left to swap, keep swapping...
        while (left < right)
        {
            std::swap(current[left], current[right]); // character swap
            left += 3;
            right -= 3;
        }
    }
}

void Cards::rotateRight(std::string &current)
{
    if (!current.empty())
        std::rotate(current.begin(), current.end() - 3, current.end());
}

void Cards::rotateLeft(std::string &current)
{
    if (!current.empty())
        std::rotate(current.begin(), current.begin() + 3, current.end());
}

void Cards::doubleString(std::string &current)
{
    current.append(current);
}

void Cards::append(std::string &current, int number)
{
    for(int i = 0; i < number; i++)
    {
        current.append("[ ]");
    }
}

void Cards::remove(std::string &current, int number)
{
    int numToRemove = number * 3;
    int stringLength = current.size();
    if (stringLength >= numToRemove)
        current.erase(current.end() - numToRemove, current.end());
    else
        current.clear();
}

void Cards::unicorn(std::string &current)
{
    current.append("[*]");
}

void Cards::gargoyle(std::string &current)
{
    current.append("[G]");
}

/*  Function: playCard()
    Goal:     Giant ugly switch statement that calls the correct
              function based on the card type in order to apply that
              function to the given string (passed by reference) */
void Cards::playCard(char type, std::string &current)
{
    // do the correct action based on the card type
    switch (type)
    {
    case UNICORN:
        unicorn(current);
        break;
    case GARGOYLE:
        gargoyle(current);
        break;
    case REVERSE: // only need to swap every "third" character, ignoring '[' and ']'
        reverse(current);
        break;
    case ROTATE_R: // rotate to the right by 3 characters
        rotateRight(current);
        break;
    case ROTATE_L: // rotate to the left by 3 characters
        rotateLeft(current);
        break;
    case DOUBLE:
        doubleString(current);
        break;
    case APPEND_1:
        append(current, 1);
        break;
    case APPEND_2:
        append(current, 2);
        break;
    case APPEND_3:
        append(current, 3);
        break;
    case REMOVE_1: // remove a single string element [?], i.e. 3 characters
        remove(current, 1);
        break;
    case REMOVE_2: // remove two string elements [?][?], i.e. 6 characters
        remove(current, 2);
        break;
    case REMOVE_3: // remove three string elements [?][?][?], i.e. 9 characters
        remove(current, 3);
        break;
    case 'x':
        std::cout << "Ran out of cards!" << std::endl;
        break;
    default:
        std::cout << "Something went wrong with type " << int(type) << " -- check yo switch statement, boo." << std::endl;
    }
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

Cards::Cards(std::map<char, std::string> cardMap)
{
    this->cardMap = cardMap;
}

/*  Function: generateString()
    Goal:     Generates a string (from empty) using a list of cards */
std::string Cards::generateString(std::vector<char> cardList)
{
    std::string cardString = "";
    std::vector<char>::iterator it;
    for (it = cardList.begin(); it < cardList.end(); it++)
    {
        playCard(*it, cardString);
    }
    return cardString;
}

/*  Function: getCardName()
    Goal:     Returns the string name of a card given the char type */
std::string Cards::getCardName(char type)
{
    std::string cardName;
    if (type < ACTION_CARD_DISCARD)
        cardName = "[" + cardMap[type] + "]";
    else
        cardName = "*" + cardMap[type] + "*";
    return cardName;
}