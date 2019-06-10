
#include "Decks.h"
#include "Player.h"

#include <vector>
#include <iostream>
#include <algorithm>

void Player::drawCard(StringDeck &deck)
{
    char newCard = deck.drawCard();
    // allCards.push_back(newCard);

    std::vector<char> currentHand;

    int numHands = allHands.size();
    int numCards;

    int i, j;
    if(numHands == 0) {
        std::vector<char> newHand;
        newHand.push_back(newCard);
        allHands.insert(newHand);
    } else {
        std::unordered_set<std::vector<char>, VectorHash>::iterator it;
        std::unordered_set<std::vector<char>, VectorHash> newHands;

        for(it = allHands.begin(); it != allHands.end(); it++)
        {
            currentHand = *it;
            numCards = currentHand.size();
            for (j = 0; j <= numCards; j++)
            {
                std::vector<char> newHand(currentHand);
                newHand.insert(newHand.begin() + j, newCard);
                newHands.insert(newHand);
            }
        }

        allHands = newHands;
    }
}

std::string Player::generateString(StringDeck &deck, std::vector<char> hand, std::string current)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++) {
        deck.playCard(*it, current);
    }

    return current;
}

void Player::printHand(StringDeck &deck, std::vector<char> hand)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++)
    {
        std::cout << "[" << deck.getCardName(*it) << "]";
    }
    std::cout << std::endl;
}

bool Player::takeTurn(StringDeck &deck, std::string goalString)
{
    std::unordered_set<std::vector<char>, VectorHash>::iterator it;
    std::string currentString;

    drawCard(deck);
    for (it = allHands.begin(); it != allHands.end(); it++)
    {
        currentString = generateString(deck, *it, "");
        
        // std::cout << currentString << std::endl;
        if(currentString == goalString) {
            printHand(deck, *it);
            return true;
        }
    }

    return false;
}

void Player::printAll(StringDeck &deck)
{
    // std::list<std::vector<char>>::iterator it;
    std::unordered_set<std::vector<char>, VectorHash>::iterator it;

    for (it = allHands.begin(); it != allHands.end(); it++)
    {
        printHand(deck, *it);
    }
}

void Player::printSize(int numCards) {
    int numPerms = allHands.size();
    int elementSize = sizeof(char) * numCards;
    int handSize = elementSize + sizeof(std::vector<char>);
    int permSize = sizeof(std::unordered_set<std::vector<char>, VectorHash>) + handSize * numPerms;

    std::cout << "Number of permutations:   " << numPerms << std::endl;
    std::cout << "Size of each hand:        " << elementSize << " bytes" << std::endl; 
    std::cout << "Size of each vector:      " << handSize << " bytes" << std::endl;
    std::cout << "Size of all permutations: " << permSize << " bytes" << std::endl;
}

// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++
int Player::stringDistance(const std::string &string1, const std::string &string2)
{
    if(string1.size() > string2.size())
        return stringDistance(string2, string1);

    int minSize = string1.size();
    int maxSize = string2.size();
    std::vector<int> levenshteinDistance(minSize + 1);

    for(int k = 0; k <= minSize; k++) 
        levenshteinDistance[k] = k;

    for (int j = 1; j <= maxSize; j++)
    {
        int previous_diagonal = levenshteinDistance[0];
        int previous_diagonal_save;
        levenshteinDistance[0]++;

        for (int i = 1; i <= minSize; i++)
        {
            previous_diagonal_save = levenshteinDistance[i];
            if (string1[i - 1] == string2[j - 1])
                levenshteinDistance[i] = previous_diagonal;
            else
                levenshteinDistance[i] = std::min(std::min(levenshteinDistance[i - 1], levenshteinDistance[i]), previous_diagonal) + 1;
            
            previous_diagonal = previous_diagonal_save;
        }
    }

    int result = levenshteinDistance[minSize];

    return result;
}