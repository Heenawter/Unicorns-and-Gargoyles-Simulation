
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <cstdlib> // random numbers
#include <ctime> // need time for randomness
#include <random>
#include <chrono> // std::chrono::system_clock

const short UNICORN = 0, GARGOYLE = 1, REVERSE = 2, ROTATE_R = 3, ROTATE_L = 4, DOUBLE = 5, 
            APPEND_1 = 6, APPEND_2 = 7, APPEND_3 = 8, REMOVE_1 = 9, REMOVE_2 = 10, REMOVE_3 = 11;

class Deck {
    private:
        std::map<std::string, short> map;
        std::vector<short> cards;
        
        void initializeMap() {
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

        void readGameStats() {
            std::ifstream file;
            file.open("Draft1_Stats.txt");

            if(file.is_open()) {
                std::string line;
                int numCards;

                std::getline(file, line); // garbage read of first line
                file >> numCards; // the number of cards to read
                std::getline(file, line); // garbage read of new line
    
                std::string cardName;
                int count;
                short cardType;
                for(int i = 0; i < numCards - 2; i++) { // -2 because I want to ignore comment/uncomment for now
                    std::getline(file, line);

                    cardName = line.substr(0, line.find_first_of(','));
                    count = int(line[line.length() - 1]) - 48; // -48 to convert from ascii to int

                    for(int j = 0; j < count; j++) {
                        cardType = getCardType(cardName);
                        cards.push_back(cardType);
                    }
                }
                file.close();
            }
        }

        short getCardType(std::string card) {
            return map[card];
        }

        // WARNING: inefficient - avoid use
        // mostly for debugging purposes
        std::string getCardName(short cardType) {
            for (std::map<std::string, short>::iterator it = map.begin(); it != map.end(); it++)
                if (it->second == cardType)
                    return it->first;
        }

        void shuffleDeck() {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
        }

    public:
        Deck() {
            // ORDER MATTERS here - initialize map FIRST then read
            initializeMap();
            readGameStats();
            shuffleDeck();
        }

        short drawCard() {
            short newCard = cards.back();
            std::cout << getCardName(newCard) << std::endl;
            cards.pop_back();
            return newCard;
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
    // deck.readGameStats();
    std::string testString = "[ ][*][ ][G][ ]";
    std::cout << testString << std::endl;
    deck.playCard("reverse order", testString);
    std::cout << testString << std::endl;

    deck.drawCard();
}