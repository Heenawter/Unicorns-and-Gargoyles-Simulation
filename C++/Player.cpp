
#include "Constants.h"
#include "Decks.h"
#include "Player.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <chrono>

Player::~Player() {
    // delete[] allHands;
}

Player::Player() {
    // totalCards = 0;
}

char Player::drawCard(Deck &deck)
{
    char newCard = deck.drawCard();
    
    if(newCard == 'x')
        return RAN_OUT_OF_CARDS;

    std::vector<char> currentHand;

    int numHands = allHands.size();
    int numCards;

    std::chrono::high_resolution_clock::time_point tStart;
    std::chrono::high_resolution_clock::time_point tEnd;
    int difference;

    int i, j;
    if (numHands == 0)
    {
        std::vector<char> newHand;
        newHand.push_back(newCard);
        allHands.insert(newHand);
    }
    else
    {
        tStart = std::chrono::high_resolution_clock::now();
        std::unordered_set<std::vector<char>, VectorHash>::iterator it;
        std::unordered_set<std::vector<char>, VectorHash> newHands;

        for (it = allHands.begin(); it != allHands.end(); it++)
        {
            tEnd = std::chrono::high_resolution_clock::now();
            difference = std::chrono::duration_cast<std::chrono::seconds>(tEnd - tStart).count();

            if(difference > 10) {
                return RAN_OUT_OF_TIME;
            }

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

    return SUCCESSFUL_CARD_DRAW;
}

std::string Player::generateString(Deck &deck, std::vector<char> hand, std::string current)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++) {
        deck.playCard(*it, current);
    }

    return current;
}

void Player::printHand(Deck &deck, std::vector<char> hand)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++)
    {
        std::cout << "[" << deck.getCardName(*it) << "]";
    }
    std::cout << std::endl;
}

char Player::takeTurn(Deck &deck, std::string goalString)
{
    std::unordered_set<std::vector<char>, VectorHash>::iterator it;
    std::string tryString;

    char success = drawCard(deck);

    if (success == SUCCESSFUL_CARD_DRAW) {
        for (it = allHands.begin(); it != allHands.end(); it++)
        {
            tryString = generateString(deck, *it, currentString);
            // std::cout << currentString << std::endl;
            if (tryString == goalString)
            {
                return WIN;
            }
        }
        // finished all and no win
        return NO_WIN;
    } 

    return success;
}

void Player::printAll(Deck &deck)
{
    // std::list<std::vector<char>>::iterator it;
    std::unordered_set<std::vector<char>, VectorHash>::iterator it;

    for (it = allHands.begin(); it != allHands.end(); it++)
    {
        printHand(deck, *it);
    }
}

void Player::printSize(int numCards)
{
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