
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
// However, we need to find the most advantageous
// position for that card!
// return the string distance and the hand
std::pair<int, std::vector<char> > Player::drawCard(Deck &deck, std::string goalString)
{
    // std::cout << "DRAW CARD" << std::endl;
    char newCard = deck.drawCard(); 
    // choose the best position to place the new card

    int bestDistance = MAX_INT, testDistance;
    std::string testString;
    std::vector<char> bestHand, testHand;

    if(numCards == 0)
    {
        bestHand.push_back(newCard);
        testString = generateString(deck, bestHand);
        bestDistance = stringDistance(testString, goalString);
    }
    else
    {
        copy(currentHand.begin(), currentHand.end(), std::back_inserter(testHand));
        // it is more efficient to insert at the back
        testHand.push_back(newCard); 
        // so make swaps from back to front to generate all insert positions
        std::vector<char>::reverse_iterator it = testHand.rbegin();
        for(int i = 0; i <= numCards; i++)
        {
            testString = generateString(deck, testHand);
            testDistance = stringDistance(testString, goalString);
            if(testDistance < bestDistance)
            {
                bestDistance = testDistance;
                bestHand.assign(testHand.begin(), testHand.end());
            }
        
            if(i < numCards) // only swap if valid (not at the end)
                std::swap(*(it + i), *(it + i + 1));
        }
    }

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


// first try every combination of swapping two cards im the
// current hand; if none of these swaps get you closer to th
// goalString, draw a new card instead
int Player::takeTurn(Deck &deck, std::string goalString)
{
    // std::cout << "CURRENT DISTANCE: " << currentDistance << std::endl;
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
    // printHand(deck, currentHand);
    // std::string testString = generateString(deck, currentHand);
    return currentDistance;
}

// https://dzone.com/articles/the-levenshtein-algorithm-1
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++
int Player::stringDistance(const std::string &string1, const std::string &string2)
{
    int rows = (string1.size() / 3) + 1; // rows, not including [ and ]
    int cols = (string2.size() / 3) + 1; // cols, not including [ and ]
    // std::cout << "rows: " << rows << ", cols: " << cols << std::endl;
    std::vector<std::vector<int>> levenshteinDistance(rows + 1, std::vector<int>(cols + 1));
    int i;  // counter for rows
    int j;  // counter for columns

    // fill with zeros
    for(i = 0; i < rows; i++)
        levenshteinDistance[i].clear();
    // initialize the first column
    // - fill with number of deletions to get to empty string
    for (i = 1; i < rows; i++)
        levenshteinDistance[i][0] = i;
    // intialize the first row
    // - fill with number of insertions to get to current string
    for (j = 1; j < cols; j++)
        levenshteinDistance[0][j] = j;

    int substitutionCost;
    int deletionCost;
    int insertionCost;
    for (i = 1; i < rows; i++) {
        for(j = 1; j < cols; j++) {
            substitutionCost = levenshteinDistance[i - 1][j - 1];
            // when checking string, need to account for skipping [ and ]
            // hence, take the (index * 3) - 2 to get the card value
            if(string1[(i * 3) - 2] != string2[(j * 3) - 2])
                substitutionCost++;
            insertionCost = levenshteinDistance[i][j - 1] + 1;
            deletionCost = levenshteinDistance[i - 1][j] + 1;
            levenshteinDistance[i][j] = std::min(substitutionCost, std::min(insertionCost, deletionCost));
        }
    }

    // for (i = 0; i < rows; i++)
    // {
    //     for (j = 0; j < cols; j++)
    //         std::cout << levenshteinDistance[i][j] << ",";
    //     std::cout << std::endl;
    // }

    return levenshteinDistance[rows - 1][cols - 1];
}