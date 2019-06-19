
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
// This card is just appended to the end of our current hand.
// returns the new card so we can check if it is an
// action card or not
char Player::drawCard(Deck &deck, std::string goalString)
{
    // std::cout << "DRAW CARD" << std::endl;
    char newCard = deck.drawCard();
    char actionCard = deck.getCardName(newCard)[0];

    std::cout << actionCard << std::endl;
    if (actionCard == '*') {
        discardCard(deck, goalString);
    } else {
        std::string testString;

        currentHand.push_back(newCard);
        testString = generateString(deck, currentHand);
        currentDistance = stringDistance(testString, goalString);

        numCards++;
    }

    return newCard;
}

// find the best possible two cards to swap positions
// that is, swap the two cards that will get you AS CLOSE
// AS CURRENTLY POSSIBLE to the goal in comparison to all
// other card swaps
std::pair<int, std::vector<char> > Player::swapCards(Deck &deck, std::string goalString)
{
    // std::cout << "SWAP" << std::endl;
    std::vector<char> bestHand, testHand;
    int bestDistance = MAX_INT, testDistance;
    std::string bestString, testString;
    int swap1, swap2;

    testHand = currentHand; // deep copy -- using copy constructor
    // try all cards as the first card to swap
    for (swap1 = 0; swap1 < numCards; swap1++)
    {
        // try all cards (minus ones we already tried) as the
        // second card to swap; notice that the pattern for, say
        // four cards, would go as follows:
        // 1 <-> 2 ; 1 <-> 3 ; 1 <-> 4 ; 2 <-> 3 ; 2 <-> 4, 3 <-> 4
        // We can see that doubles, such as 2 <-> 1, are skipped
        // because swap2 = swap1
        for (swap2 = swap1; swap2 < numCards; swap2++)
        {
            // obviously, 1 <-> 1 makes no sense; so, just skip
            // this if it happens
            if (swap1 != swap2)
            {
                std::swap(*(testHand.begin() + swap1), *(testHand.begin() + swap2));

                testString = generateString(deck, testHand);
                testDistance = stringDistance(testString, goalString);
                if (testDistance < bestDistance)
                {
                    bestDistance = testDistance;
                    bestHand = testHand; // deep copy -- using copy constructor
                }

                testHand = currentHand; // deep copy -- using copy constructor
            }
        }
    }

    return std::make_pair(bestDistance, bestHand);
}

void Player::commentCard(std::string goalString)
{
    // -- action card -- 
    // first try commenting out one of your own cards;
    // if doing so does not get you any closer to the
    // goal, then comment out a card for the player
    // who is CLOSEST to winning - in this case, you 
    // would want to *decrease* their similarity to 
    // the goal string

        
}
 
void Player::discardCard(Deck &deck, std::string goalString)
{
    // -- action card --
    // find the most advantageous card to remove and
    // add it back to the deck
    // if you can't IMPROVE your hand by removing a card,
    // simply try your best not to make it worse

    if (numCards <= 0)
        return; // nothing to do if no cards to remove!

    std::vector<char> bestHand(numCards - 1), testHand(numCards);
    int bestDistance = MAX_INT, testDistance;
    std::string bestString, testString;
    char bestCard, testCard;
    int index;

    // we can safely copy since we know we have at least 1 card
    copy(currentHand.begin(), currentHand.end(), testHand.begin());
    for (index = 0; index < numCards; index++)
    {
        testCard = testHand[index];
        testHand.erase(testHand.begin() + index);
        testString = generateString(deck, testHand);
        testDistance = stringDistance(testString, goalString);

        std::cout << "distance: " << testDistance << "; ";
        printHand(deck, testHand);

        if(testDistance < bestDistance) {
            bestDistance = testDistance;
            bestHand = testHand; // deep copy -- using copy constructor
            bestCard = testCard; 
        }

        testHand.push_back('x');
        testHand = currentHand; // deep copy -- using copy constructor
    }

    std::cout << "Best:";
    printHand(deck, bestHand);
    // make best new current and return card to the deck
    currentHand = bestHand;
    deck.putCardBack(bestCard);
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
    char newCard;
    bool actionCard = false;

    // if you have 0 or 1 cards, no point swapping - might as well
    // draw another card
    if(numCards <= 1)
        newCard = drawCard(deck, goalString);
    else
    {
        // otherwise, try swaps before drawing card
        best = swapCards(deck, goalString);
        if(currentDistance <= best.first) {
            // swapping cards did not get you any closer
            // to meeting the goal, so instead draw a 
            newCard = drawCard(deck, goalString);
        } else {
            currentDistance = best.first;
            currentHand = best.second;
        }
    }

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