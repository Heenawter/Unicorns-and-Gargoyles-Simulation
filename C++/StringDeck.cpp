#include "Decks.h"

const char UNICORN = 0, GARGOYLE = 1, REVERSE = 2, ROTATE_R = 3, ROTATE_L = 4, DOUBLE = 5,
           APPEND_1 = 6, APPEND_2 = 7, APPEND_3 = 8, REMOVE_1 = 9, REMOVE_2 = 10, REMOVE_3 = 11;

void StringDeck::removeHelper(std::string &current, int numToRemove) {
    int stringLength = current.size();
    if (stringLength >= numToRemove) {
        current.erase(current.end() - numToRemove, current.end());
    } else {
        current.clear();
    }
}

void StringDeck::playCard(char type, std::string &current)
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
        case REVERSE:                     // only need to swap every "third" character, ignoring '[' and ']'
            if(!current.empty()) {
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
        default:
            std::cout << "Something went wrong - check yo switch statement, boo." << std::endl;
    }
}