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

/******************************************************/
/*                  GAME FUNCTIONS                    */
/******************************************************/

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
    if (numCards <= 1)
    {
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
            newCard = drawCard(deck, goalString);
        }
        else
        {
            currentDistance = best.first;
            currentHand = best.second;
        }
    }

    return newCard;
}

// assume we tried swapping, and it didn't get us any
// closer to the goal -- this means we should draw
// a card instead of swapping.
// This card is just appended to the end of our current hand.
// returns the new card
char Player::drawCard(Deck &deck, std::string goalString)
{
    char newCard = deck.drawCard();

    // if not an action card....
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

bool Player::winningCondition()
{
    return currentDistance == 0;
}

/******************************************************/
/*                   ACTION CARDS                     */
/******************************************************/

// -- action card --
// find the most advantageous card to remove and
// add it back to the deck
// if you can't IMPROVE your hand by removing a card,
// simply try your best not to make it worse
void Player::discardCard(Deck &deck, std::string goalString)
{
    if (numCards <= 0)
        return; // nothing to do if no cards to remove!

    std::vector<char> bestHand(numCards - 1), testHand(numCards);
    int bestDistance = MAX_INT, testDistance;
    std::string bestString, testString;
    char bestCard, testCard;
    int index;

    // we can safely copy since we know we have at least 1 card
    testHand = currentHand;
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

// -- action card --
// generate all possiblities for which cards to remove
// (remember that the order must stay the same)
// then, choose the possibility that gets you closest
// to the goal
void Player::springCleaning(Deck &deck, std::string goalString)
{
    if (numCards <= 0)
        return; // nothing to do if no cards to remove!

    std::vector<char> combinations(numCards);
    // std::vector<std::vector<char> > allCombinations;
    int bestDistance = currentDistance;
    std::vector<char> bestHand;

    for (int i = 1; i <= numCards; i++)
    {
        combinationUtil(currentHand, combinations, bestHand, bestDistance,
                        deck, goalString, 0, numCards - 1, 0, i);
    }

    // now that we found the best possible hand, let's make sure to
    // add the removed cards back to our deck
    std::vector<char> cardsRemoved;
    std::set_difference(currentHand.begin(), currentHand.end(), bestHand.begin(), bestHand.end(),
                        std::back_inserter(cardsRemoved));
    std::vector<char>::iterator it;
    for (it = cardsRemoved.begin(); it < cardsRemoved.end(); it++)
    {
        deck.putCardBack(*it);
    }

    // now, set the best to the current
    numCards = bestHand.size();
    currentHand = bestHand;
    currentDistance = bestDistance;
}

// -- action card --
// pick a unicorn to poison and permanantly remove from play
// try to do the most damage - i.e. target the player
// whose unicorn removal moves them furthest from the goal
void Player::poisonCard(Deck &deck, std::string goalString, std::vector<Player*> &otherPlayers)
{
    // for each player, 
    //      for each unicorn in their hand
    //          remove that unicorn
    //          if (new distance - old distance) > previous best
    //              new best = (new distance - old distance)
    //              player to target = current player
    //              unicorn to remove = current unicorn

    int testDamage;
    int bestDamage = -1;
    int unicornToRemove = 0;
    Player *playerToTarget;

    int previousDistance, newDistance, testDistance;

    std::vector<Player*>::iterator pit; // player iterator
    int handIndex = 0;
    std::vector<char> currentHand, testHand;
    std::string testString;

    // for each OTHER player (not including yourself)
    for(pit = otherPlayers.begin(); pit < otherPlayers.end(); pit++)
    {
        previousDistance = (*pit)->getDistance();
        currentHand = (*pit)->getHand();

        // loop through their hand looking for potential
        // unicorns to remove
        for(handIndex = 0; handIndex < currentHand.size(); handIndex++)
        {
            testHand = currentHand;
            if(currentHand[handIndex] == UNICORN)
            {
                // found a unicorn! so remove it
                testHand.erase(testHand.begin() + handIndex);

                // and check if removing it caused damage
                testString = generateString(deck, testHand);
                testDistance = stringDistance(goalString, testString);
                if((testDistance - previousDistance) > bestDamage)
                {
                    // it caused more damage than previous
                    // removal(s), so make this the new best
                    bestDamage = (testDistance - previousDistance);
                    playerToTarget = *pit;
                    unicornToRemove = handIndex;
                }
            }
        }
    }

    if(bestDamage >= 0)
    {
        // there was a unicorn to remove, so remove
        // the unicorn that does the most damage
        // possibly this unicorn does not do any damage
        // but MUST remove a unicorn if one to remove
        // otherwise, no unicorns to remove
        playerToTarget->removeCard(deck, goalString, unicornToRemove);
    }
}

void Player::stealCard(Deck &deck, std::string goalString, std::vector<Player *> &otherPlayers)
{
    // either target the player closest to winning and stop them
    // (AGGRESSIVE) OR target the player that has a card that will
    // be advantageous to you (PASSIVE) --- for now, do PASSIVE
    std::vector<char> targetHand, testHand;
    std::string testString;
    int testDistance;

    int currentPlayer, currentCard;
    int currentBest = MAX_INT;
    int currentTargetPlayer;
    int currentTargetCard;
    for(currentPlayer = 0; currentPlayer < NUM_PLAYERS - 1; currentPlayer++)
    {
        // for each other player ...
        targetHand = otherPlayers[currentPlayer]->getHand();
        for (currentCard = 0; currentCard < targetHand.size(); currentCard++)
        {
            // loop through their hand and look for a card
            // to mark for potential robbery
            testHand = currentHand;
            testHand.push_back(targetHand[currentCard]);
            testString = generateString(deck, testHand);
            testDistance = stringDistance(testString, goalString);
            if (testDistance < currentBest)
            {
                // you found a card that helps, more than the
                // previous card, so save it
                currentTargetPlayer = currentPlayer;
                currentTargetCard = currentCard;
                currentBest = testDistance;
            }
        }
    }

    Player* targetPlayer = otherPlayers[currentTargetPlayer];

    std::cout << "Previous distance: " << currentDistance << std::endl;
    std::cout << "Card to steal: " << deck.getCardName(targetPlayer->getHand()[currentTargetCard]) << std::endl;
    std::cout << "Previous hand: ";
    printCurrentHand(deck);

    currentDistance = currentBest;
    currentHand.push_back(targetPlayer->getHand()[currentTargetCard]);
    numCards++;
    currentString = generateString(deck, currentHand);

    std::cout << "After distance: " << currentDistance << std::endl;
    std::cout << "After hand: ";
    printCurrentHand(deck);

    targetPlayer->removeCard(deck, goalString, currentTargetCard);

    return;
}

void Player::removeCard(Deck &deck, std::string goalString, int cardToRemove)
{
    currentHand.erase(currentHand.begin() + cardToRemove);
    currentString = generateString(deck, currentHand);
    currentDistance = stringDistance(currentString, goalString);
    numCards--;
}

/******************************************************/
/*                  HELPER FUNCTIONS                  */
/******************************************************/

// https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
void Player::combinationUtil(std::vector<char> hand, std::vector<char> tempHand,
                             std::vector<char> &bestHand, int &bestDistance,
                             Deck &deck, std::string goalString,
                             int start, int end, int index, int r)
{
    // base case
    if (index == r)
    {
        std::vector<char> newCombo;
        for (int j = 0; j < r; j++)
        {
            newCombo.push_back(tempHand[j]);
        }

        std::string testString = generateString(deck, newCombo);
        int testDistance = stringDistance(testString, goalString);

        if (testDistance <= bestDistance)
        {
            // because <= and not <, this implies we are being CONSERVATIVE
            // i.e. the player is choosing to remove AS FEW CARDS
            // AS POSSIBLE while still maintaining the best distance
            bestDistance = testDistance;
            bestHand = newCombo;
        }

        return;
    }

    // replace index with all possible
    // elements. The condition "end-i+1 >= r-index"
    // makes sure that including one element
    // at index will make a combination with
    // remaining elements at remaining positions
    for (int i = start; i <= end &&
                        end - i + 1 >= r - index;
         i++)
    {
        tempHand[index] = hand[i];
        combinationUtil(hand, tempHand, bestHand, bestDistance,
                        deck, goalString,
                        i + 1, end, index + 1, r);
    }
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
        LOG("[" + deck.getCardName(*it) + "]");
        // std::cout << "[" << deck.getCardName(*it) << "]";
    }
    LOG("\n");
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