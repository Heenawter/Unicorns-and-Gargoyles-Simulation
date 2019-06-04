#include "Decks.h"

// class Game {
//     private: 
//         Deck mainDeck;
//         Goals goalDeck;

//         std::string readCardStats() {
//             std::getline(file, line);

//             std::string cardName = line.substr(0, line.find_first_of(','));
//             int count = int(line[line.length() - 1]) - 48; // -48 to convert from ascii to int

//             for(int j = 0; j < count; j++) {
//                 cardType = getCardType(cardName);
//                 cards.push_back(cardType);
//             }

//             return 
//         }
    
//     public:
//         void readGameStats() {
//             std::ifstream file;
//             file.open("Draft1_Stats.txt");

//             if(file.is_open()) {
//                 std::string line;
//                 int numCards;

//                 std::getline(file, line); // garbage read of first line
//                 file >> numCards; // the number of cards to read
//                 std::getline(file, line); // garbage read of new line
    
//                 std::string cardName;
//                 int count;
//                 short cardType;
//                 for(int i = 0; i < numCards - 2; i++) { // -2 because I want to ignore comment/uncomment for now
//                     std::getline(file, line);

//                     cardName = line.substr(0, line.find_first_of(','));
//                     count = int(line[line.length() - 1]) - 48; // -48 to convert from ascii to int

//                     for(int j = 0; j < count; j++) {
//                         cardType = getCardType(cardName);
//                         cards.push_back(cardType);
//                     }
//                 }
//                 file.close();
//             }
//         }

// };

int main() {
    StringDeck deck;

    std::string testString = "[ ][*][ ][G][ ]";
    std::cout << testString << std::endl;
    deck.playCard("reverse order", testString);
    std::cout << testString << std::endl;

    std::cout << "this is a test" << std::endl;
}