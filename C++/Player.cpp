
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
}

// assume we tried swapping, and it didn't get us any
// closer to the goal -- this means we should draw
// a card instead of swapping.
// However, we need to find the most advantageous
// position for that card!
// return the string distance
int Player::drawCard(Deck &deck, std::string goalString)
{
    char newCard = deck.drawCard(); 
    // choose the best position to place the new card

    int bestDistance = 100, testDistance;
    std::string bestString = "", testString;
    std::vector<char> bestHand, testHand;

    if(numCards == 0)
    {
        bestHand.push_back(newCard);
        bestString = generateString(deck, bestHand);
        bestDistance = stringDistance(bestString, goalString);
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
                bestString = testString;
            }
        
            if(i < numCards) // only swap if valid (not at the end)
                std::swap(*(it + i), *(it + i + 1));
        }
    }

    numCards++;
    currentHand = bestHand;
    currentString = bestString;
    return bestDistance;
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
char Player::takeTurn(Deck &deck, std::string goalString)
{
    // std::unordered_set<std::vector<char>, vectorHash>::iterator it;
    // std::string tryString;

    // if you have 0 or 1 cards, no point swapping - might as well
    // draw another card
    if(numCards <= 1) {
        drawCard(deck, goalString);
        return 'z';
    }
    
    // otherwise, try swaps before drawing card

    std::vector<char> testHand;
    copy(currentHand.begin(), currentHand.end(), std::back_inserter(testHand));
    int swap1, swap2;
    std::vector<char>::iterator it = testHand.begin();
    printHand(deck, testHand);

    for(swap1 = 0; swap1 < numCards; swap1++) {
        for(swap2 = swap1; swap2 < numCards; swap2++) {
            if(swap1 != swap2) 
            {
                std::swap(*(testHand.begin() + swap1), *(testHand.begin() + swap2));
                // std::cout << swap1 << " <-> " << swap2 << std::endl;
                // std::cout << "swap " << deck.getCardName(testHand.at(swap1)) << " with " << deck.getCardName(testHand.at(swap2)) << std::endl;
                printHand(deck, testHand);
                copy(currentHand.begin(), currentHand.end(), testHand.begin());
            }
        }
    }

    // int numSwaps = testHand.size() - 1;
    // std::vector<char>::iterator card2swap;
    // for (card2swap = testHand.begin(); card2swap < testHand.end(); card2swap++)
    // {
    //     std::cout << "Card 2 swap: " << deck.getCardName(*card2swap)<< std::endl;
    //     printHand(deck, testHand);
    //     for(int i = 0; i <= numSwaps; i++) {
    //         std::swap(*card2swap, *(testHand.begin() + i));
    //         printHand(deck, testHand);
    //     }
    // }

    

    return 'y';
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