
#include "Constants.h"
#include "Decks.h"
#include "Player.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <chrono>

Player::~Player()
{
    // delete[] allHands;
}

Player::Player()
{
    currentString = "";
    numCards = 0;
    currentDistance = MAX_INT;
}

// first try every combination of swapping two cards in the
// current hand; if none of these swaps get you closer to th
// goalString, draw a new card instead
// return the new card OR NO_NEW_CARD if none drawn
char Player::takeTurn(Deck &deck, std::string goalString)
{
    std::pair<int, std::vector<char>> best;
    char newCard = NO_NEW_CARD;

    // if you have 0 or 1 cards, no point swapping - might as well
    // draw another card
    if (numCards <= 1) {
        newCard = drawCard(deck, goalString);
    }
    else
    {
        // otherwise, try swaps before drawing card
        best = moveCard(deck, goalString);
        if (currentDistance <= best.first)
        {
            // swapping cards did not get you any closer
            // to meeting the goal, so instead draw a card
            // std::cout << "draw" << std::endl;
            newCard = drawCard(deck, goalString);
        }
        else
        {
            // std::cout << "move" << std::endl;
            currentDistance = best.first;
            currentHand = best.second;
        }
    }

    printHand(deck, currentHand);
    // std::string testString = generateString(deck, currentHand);
    return newCard;
}

// assume we tried swapping, and it didn't get us any
// closer to the goal -- this means we should draw
// a card instead of swapping.
// This card is just appended to the end of our current hand.
// returns the new card
char Player::drawCard(Deck &deck, std::string goalString)
{
    // std::cout << "DRAW CARD" << std::endl;
    char newCard = deck.drawCard();

    if (newCard < ACTION_CARD_DISCARD) 
    {
        // not an action card, so add it to your hand
        // and adjust the distance from the goal
        std::string testString;

        currentHand.push_back(newCard);
        testString = generateString(deck, currentHand);
        currentDistance = stringDistance(testString, goalString);

        numCards++;
    } 
    
    return newCard;
}

std::pair<int, std::vector<char>> Player::moveCard(Deck &deck, std::string goalString)
{
    std::vector<char> bestHand, testHand, testHandRemoved;
    int bestDistance = MAX_INT, testDistance;
    std::string bestString, testString;
    int cardToMove, whereToMove;
    char currentCard;

    for (cardToMove = 0; cardToMove < numCards; cardToMove++)
    {
        testHandRemoved = currentHand;
        currentCard = testHandRemoved[cardToMove]; // save card to insert later
        // remove the card so we can insert it somewhere else
        testHandRemoved.erase(testHandRemoved.begin() + cardToMove);
        for (whereToMove = 0; whereToMove < numCards; whereToMove++)
        {
            testHand = testHandRemoved;
            // obviously, inserting a card in the exact position it already is
            // is pointless; so just skip the entire thing when this happens
            if (whereToMove != cardToMove)
            {
                testHand.insert(testHand.begin() + whereToMove, currentCard);
                testString = generateString(deck, testHand);
                testDistance = stringDistance(testString, goalString);
                if (testDistance < bestDistance)
                {
                    bestDistance = testDistance;
                    bestHand = testHand; // deep copy -- using copy constructor
                }
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
    testHand = currentHand;
    // copy(currentHand.begin(), currentHand.end(), testHand.begin());
    for (index = 0; index < numCards; index++)
    {
        testCard = testHand[index];
        testHand.erase(testHand.begin() + index);
        testString = generateString(deck, testHand);
        testDistance = stringDistance(testString, goalString);

        if (testDistance < bestDistance)
        {
            bestDistance = testDistance;
            bestHand = testHand; // deep copy -- using copy constructor
            bestCard = testCard;
        }

        testHand = currentHand; // deep copy -- using copy constructor
    }

    // make best new current and return card to the deck
    currentHand = bestHand;
    deck.putCardBack(bestCard);
    numCards--;
}

void Player::combinationUtil(std::vector<char> arr, std::vector<char> data,
                             std::vector<std::vector<char> > &allCombinations,
                             int start, int end, int index, int r)
{
    // std::cout << "start: " << start << ", end: " << end << ", index: " << index << ", r: " << r << std::endl;
    // base case
    // Current combination is ready
    // to be printed, print it
    if (index == r)
    {
        std::vector<char> newCombo;
        for (int j = 0; j < r; j++) {
            newCombo.push_back(data[j]);
            // std::cout << int(data[j]) << " ";
        }
        // std::cout << std::endl;

        // std::copy(data.begin(), data.end() - r, newCombo);
        allCombinations.push_back(newCombo);

        // std::cout << std::endl;
        return;
    }

    // replace index with all possible
    // elements. The condition "end-i+1 >= r-index"
    // makes sure that including one element
    // at index will make a combination with
    // remaining elements at remaining positions
    for (int i = start; i <= end &&
                        end - i + 1 >= r - index; i++)
    {
        data[index] = arr[i];
        combinationUtil(arr, data, allCombinations,
                        i + 1, end, index + 1, r);
    }
}

void Player::springCleaning(Deck &deck, std::string goalString)
{
    // generate all possiblities for which cards to remove
    // (remember that the order must stay the same)
    // then, choose the possibility that gets you closest
    // to the goal

    if (numCards <= 0)
        return; // nothing to do if no cards to remove!

    std::vector<char> combinations(numCards);
    std::vector<std::vector<char> > allCombinations;

    for(int i = 1; i < numCards; i++) {
        combinationUtil(currentHand, combinations, allCombinations, 0, numCards - 1, 0, i);
    }

    std::cout << allCombinations.size();
    for (int k = 0; k < allCombinations.size(); k++)
    {
        printHand(deck, allCombinations[k]);
    }
}

bool Player::winningCondition()
{
    return currentDistance == 0;
}

// using the hand, generate the string from empty
std::string Player::generateString(Deck &deck, std::vector<char> hand)
{
    std::string current = "";
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++)
    {
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

// print the current hand using the card types rather than the chars
// that represent each card (i.e. "unicorn" instead of char(0))
void Player::printCurrentHand(Deck &deck)
{
    printHand(deck, currentHand);
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