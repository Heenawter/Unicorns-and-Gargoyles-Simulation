
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

const short UNICORN = 0, GARGOYLE = 1, REVERSE = 2, ROTATE_R = 3, ROTATE_L = 4, DOUBLE = 5, 
            APPEND_1 = 6, APPEND_2 = 7, APPEND_3 = 8, REMOVE_1 = 9, REMOVE_2 = 10, REMOVE_3 = 11;

class Deck {
    private:
        std::map<std::string, short> map;

    public:
        Deck() {
            map["unicorn"] = UNICORN;
            map["gargoyle"] = GARGOYLE;
            map["reverse order"] = REVERSE;
            map["rotate right"] = ROTATE_R;
            map["rotate left"] = ROTATE_L;
            map["double row"] = DOUBLE;
            map["append 1"] = APPEND_1;
            map["append 2"] = APPEND_2;
            map["append 3"] = APPEND_3;
            map["remove 1"] = REMOVE_1;
            map["remove 2"] = REMOVE_2;
            map["remove 3"] = REMOVE_3;
        }

        short getCardType(std::string card) {
            return map[card];
        }

        void playCard(std::string card, std::string &current) {
            short type = getCardType(card);
            int left;
            int right;

            // do the correct action based on the card type
            switch(type) {
                case UNICORN:
                    current.append("[*]");
                    break;
                case GARGOYLE:
                    current.append("[G]");
                    break;
                case REVERSE: // only need to swap every "third" character, ignoring '[' and ']'
                    left = 1;                       // right-most index to swap
                    right = current.length() - 2;   // left-most index to swap

                    // while still things left to swap, keep swapping...
                    while(left < right) {          
                        std::swap(current[left], current[right]);   // character swap
                        left += 3;
                        right -= 3;
                    }
                    break;
                case ROTATE_R: // rotate to the right by 3 characters
                    std::rotate(current.begin(), current.end() - 3, current.end());
                    break;
                case ROTATE_L: // rotate to the left by 3 characters
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
                    current.erase(current.end() - 3, current.end());
                    break;
                case REMOVE_2: // remove two string elements [?][?], i.e. 6 characters
                    current.erase(current.end() - 6, current.end());
                    break;
                case REMOVE_3: // remove three string elements [?][?][?], i.e. 9 characters
                    current.erase(current.end() - 9, current.end());
                    break;
            }
        }
};

int main() {
    Deck deck;
    std::string testString = "[ ][*][ ][G][ ]";
    std::cout << testString << std::endl;
    deck.playCard("reverse order", testString);
    std::cout << testString << std::endl;
}