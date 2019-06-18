
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
    currentString = "";
    numCards = 0;
    currentDistance = MAX_INT;
}

// assume we tried swapping, and it didn't get us any
// closer to the goal -- this means we should draw
// a card instead of swapping.
// This card is just appended to the end of our hand.
std::pair<int, std::vector<char> > Player::drawCard(Deck &deck, std::string goalString)
{
    // std::cout << "DRAW CARD" << std::endl;
    char newCard = deck.drawCard(); 

    int bestDistance;
    std::string testString;
    std::vector<char> bestHand;

    if(numCards > 0)
        copy(currentHand.begin(), currentHand.end(), std::back_inserter(bestHand));
    
    bestHand.push_back(newCard);
    testString = generateString(deck, bestHand);
    bestDistance = stringDistance(testString, goalString);

    numCards++;
    return std::make_pair(bestDistance, bestHand);
}

std::pair<int, std::vector<char> > Player::swapCards(Deck &deck, std::string goalString)
{
    // std::cout << "SWAP" << std::endl;
    std::vector<char> bestHand, testHand;
    int bestDistance = MAX_INT, testDistance;
    std::string bestString, testString;
    int swap1, swap2;

    copy(currentHand.begin(), currentHand.end(), std::back_inserter(testHand));
    for (swap1 = 0; swap1 < numCards; swap1++)
    {
        for (swap2 = swap1; swap2 < numCards; swap2++)
        {
            if (swap1 != swap2)
            {
                std::swap(*(testHand.begin() + swap1), *(testHand.begin() + swap2));

                testString = generateString(deck, testHand);
                testDistance = stringDistance(testString, goalString);
                if (testDistance < bestDistance)
                {
                    bestDistance = testDistance;
                    bestHand.assign(testHand.begin(), testHand.end());
                }

                copy(currentHand.begin(), currentHand.end(), testHand.begin());
            }
        }
    }

    return std::make_pair(bestDistance, bestHand);
}

// using the hand, generate the string from empty
std::string Player::generateString(Deck &deck, std::vector<char> hand)
{
    std::string current = "";
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++) {
        deck.playCard(*it, current);
    }

    return current;
}

// print the given hand using the card types rather than the chars
// that represent each card (i.e. "unicorn" instead of char(0))
void Player::printHand(Deck &deck, std::vector<char> hand)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++)
    {
        std::cout << "[" << deck.getCardName(*it) << "]";
    }
    std::cout << std::endl;
}

// first try every combination of swapping two cards in the
// current hand; if none of these swaps get you closer to th
// goalString, draw a new card instead
int Player::takeTurn(Deck &deck, std::string goalString)
{
    std::pair<int, std::vector<char> > best;

    // if you have 0 or 1 cards, no point swapping - might as well
    // draw another card
    if(numCards <= 1)
        best = drawCard(deck, goalString);
    else
    {
        // otherwise, try swaps before drawing card
        best = swapCards(deck, goalString);
        if(currentDistance <= best.first)
        {
            // swapping cards did not get you any closer
            // to meeting the goal, so instead draw a 
            best = drawCard(deck, goalString);
        }
    }
    
    currentDistance = best.first;
    currentHand = best.second;
    printHand(deck, currentHand);
    // std::string testString = generateString(deck, currentHand);
    return currentDistance;
}

// https://dzone.com/articles/the-levenshtein-algorithm-1
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++
int Player::stringDistance(const std::string &string1, const std::string &string2)
{
    int minSize = (string1.size() / 3) + 1;
    int maxSize = (string2.size() / 3) + 1;

    // relies on string1 being smaller than string2
    if (minSize > maxSize)
        return stringDistance(string2, string1);

    std::vector<int> levenshteinDistance(minSize);

    // initialize the vector with the first row [0, 1, 2, 3...]
    for (int k = 0; k < minSize; k++)
        levenshteinDistance[k] = k;

    int prevSubstitutionCost;
    int substitutionCost;
    int insertionCost;
    int deletionCost;
    int stringIndex1 = 1;
    int stringIndex2 = 1;
    for (int j = 1; j < maxSize; j++)
    {
        prevSubstitutionCost = levenshteinDistance[0];
        levenshteinDistance[0]++; 
        for (int i = 1; i < minSize; i++)
        {
            // save the last diagonal; i.e. the previous substitution cost
            substitutionCost = levenshteinDistance[i];
            // when checking string, need to account for skipping [ and ]
            // hence, string index is increased by 3 each time rather than 1
            if (string1[stringIndex1] == string2[stringIndex2])
                // copy the last diagonal over, since we have a match
                // that is, no change necessary, so copy the best solution thus far
                levenshteinDistance[i] = prevSubstitutionCost;
            else
            {
                // otherwise, consider deletion, insertion, and substitution
                deletionCost = levenshteinDistance[i - 1];
                insertionCost = levenshteinDistance[i]; // BEFORE it is overwritten, it is the "previous row" still
                levenshteinDistance[i] = std::min(std::min(deletionCost, insertionCost), prevSubstitutionCost) + 1;
            }

            prevSubstitutionCost = substitutionCost; // update previous to current
            stringIndex1 += 3;
        }
        stringIndex1 = 1;
        stringIndex2 += 3;
    }

    return levenshteinDistance[minSize - 1];
}