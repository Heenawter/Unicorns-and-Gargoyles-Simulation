#include "Constants.h"
#include "Decks.h"

#include <chrono> // need std::chrono::system_clock for randomness
#include <iostream>

std::string Deck::getCardName(char cardType)
{
    for (std::map<std::string, char>::iterator it = map.begin(); it != map.end(); it++)
        if (it->second == cardType)
            return it->first;
}

bool Deck::hasCards() {
    if(cards.size() > 0) 
        return true;
    
    return false;
}

void Deck::shuffleDeck()
{
    // unsigned seed = SEED;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

void Deck::addToDeck(std::string line, char cardType)
{
    std::string cardName;
    int count;

    cardName = line.substr(0, line.find_first_of(','));
    map[cardName] = cardType;

    count = int(line[line.length() - 1]) - 48; // -48 to convert from ascii to int

    for (int j = 0; j < count; j++)
        cards.push_back(cardType);
}

char Deck::drawCard()
{
    char newCard;
    if(cards.size() > 0) {
        newCard = cards.back();
        cards.pop_back();
    } else {
        newCard = 'x';
    }

    // std::cout << "Draw..." << getCardName(newCard) << std::endl;
    return newCard;
}

// insert card back into deck in a random position
void Deck::putCardBack(char card)
{
    if(cards.size() == 0) {
        cards.push_back(card);
        return;
    }
    int min = 0; 
    int max = cards.size() - 1;
    // unsigned seed = SEED;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);     // random-number engine used (Mersenne-Twister in this case)
    // create a randomly distributed array with values
    // from [min, max] (inclusive)
    std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased
    // grab a random integer from this range
    int randomIndex = uni(rng); // where to insert card
    cards.insert(cards.begin() + randomIndex, card);
}

bool Deck::hasNonActionCard() {
    std::vector<char>::iterator it;
    for(it = cards.begin(); it < cards.end(); it++) {
        if(*it < ACTION_CARD_DISCARD) 
            return true; // found a non-action card
    }
    return false; // did NOT find a non-action card
}

// remove the specified number of cards from a string
// if the number to remove is greater than the number of
// characters left, then simply empty the string
void Deck::removeHelper(std::string &current, int numToRemove)
{
    int stringLength = current.size();
    if (stringLength >= numToRemove)
        current.erase(current.end() - numToRemove, current.end());
    else
        current.clear();
}

void Deck::playCard(char type, std::string &current)
{
    int left;
    int right;

    // do the correct action based on the card type
    switch (type)
    {
    case UNICORN:
        current.append("[*]");
        break;
    case GARGOYLE:
        current.append("[G]");
        break;
    case REVERSE: // only need to swap every "third" character, ignoring '[' and ']'
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
        break;
    case ROTATE_R: // rotate to the right by 3 characters
        if (!current.empty())
            std::rotate(current.begin(), current.end() - 3, current.end());
        break;
    case ROTATE_L: // rotate to the left by 3 characters
        if (!current.empty())
            std::rotate(current.begin(), current.begin() + 3, current.end());
        break;
    case DOUBLE:
        current.append(current);
        break;
    case APPEND_1:
        current.append("[ ]");
        break;
    case APPEND_2:
        current.append("[ ][ ]");
        break;
    case APPEND_3:
        current.append("[ ][ ][ ]");
        break;
    case REMOVE_1: // remove a single string element [?], i.e. 3 characters
        removeHelper(current, 3);
        break;
    case REMOVE_2: // remove two string elements [?][?], i.e. 6 characters
        removeHelper(current, 6);
        break;
    case REMOVE_3: // remove three string elements [?][?][?], i.e. 9 characters
        removeHelper(current, 9);
        break;
    case 'x':
        std::cout << "Ran out of cards!" << std::endl;
        break;
    default:
        std::cout << "Something went wrong with type " << int(type) << " -- check yo switch statement, boo." << std::endl;
    }
}