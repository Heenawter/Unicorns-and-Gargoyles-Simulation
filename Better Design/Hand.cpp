#include "Hand.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

// https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
void Hand::combinationUtil(std::vector<char> hand, std::vector<char> tempHand,
                             std::vector<char> &bestHand, int &bestDistance,
                             std::string goalString,
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

        std::string testString = generateString(newCombo);
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
                        goalString,
                        i + 1, end, index + 1, r);
    }
}

// https://dzone.com/articles/the-levenshtein-algorithm-1
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++
int Hand::stringDistance(const std::string &string1, const std::string &string2)
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

/*  Function: generateString(vector<char>)
    Goal:     Generates the string for the GIVEN vector of cards */
std::string Hand::generateString(std::vector<char> cards)
{
    return cardInfo->generateString(cards);
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

/*  Function: Hand()
    Goal:     Constructor for the Hand object;
              Stores information about the cards in the 
              hand as WELL as the distance from the goal,
              the current string, etc. */
Hand::Hand(std::string goalString, Cards *cardInfo)
{
    this->goalString = goalString;
    this->cardInfo = cardInfo;
    this->numCards = 0;
    this->currentDistance = MAX_INT;
    this->currentString = "";
}

Hand::Hand(const Hand &oldHand)
{
    this->cards = oldHand.cards;

    this->goalString = oldHand.goalString;
    this->cardInfo = oldHand.cardInfo;
    this->numCards = oldHand.numCards;
    this->currentDistance = oldHand.currentDistance;
    this->currentString = oldHand.currentString;
}

/*  Function: getCardName(chard)
    Goal:     Get the card name for the given card */
std::string Hand::getCardName(char card)
{
    cardInfo->getCardName(card);
}

/*  Function: generateString()
    Goal:     Generates the string for the current hand */
std::string Hand::generateString()
{
    return cardInfo->generateString(this->cards);
}

/*  Function: operator <
    Goal:     Compare two hands with the < symbol through overriding;
              i.e. consider hand1 < hand2;
              If hand1 is closer to the goal than hand2, return true
                    -- since hand1 has smaller distance than hand2
              If hand2 is farther from the goal than hand2, return false
    Example:  If hand1, hand2 are of type Hand*
              call with (*hand1) < (*hand2) */
bool Hand::operator < (const Hand &h2)
{
    int distance1 = this->currentDistance;
    int distance2 = h2.currentDistance;
    return distance1 < distance2;
}

/*  Function: addToHand()
    Goal:     Adds the given card to the hand;
              also increases the number of cards, generates the
              new string, and calculates the new distance from 
              the goal string */
void Hand::addToHand(char card)
{
    this->cards.push_back(card);
    this->numCards++;
    this->currentString = generateString();
    this->currentDistance = stringDistance(this->currentString, this->goalString);
}

void Hand::moveCard(int oldIndex, int newIndex)
{
    // first, save the target card's value
    char cardToMove = this->cards[oldIndex];
    // then, remove the card from the hand
    this->cards.erase(this->cards.begin() + oldIndex);
    // and reinsert it in the proper location
    this->cards.insert(this->cards.begin() + newIndex, cardToMove);

    this->currentString = generateString();
    this->currentDistance = stringDistance(this->currentString, this->goalString);
}

/*  Function: removeCard()
    Goal:     Removes the card at the given index from the hand;
              also decreases the number of cards, generates the
              new string, and calculates the new distance from 
              the goal string 
    Returns:  The card removed */
char Hand::removeCard(int i)
{
    char cardType = this->cards[i];

    this->cards.erase(this->cards.begin() + i);

    this->numCards--;
    this->currentString = generateString();
    this->currentDistance = stringDistance(this->currentString, this->goalString);

    return cardType;
}

/*  Function: getNumUnicorns()
    Goal:     Return the number of unicorns in the hand */
int Hand::getNumUnicorns()
{
    int numUnicorns = 0;
    for(int i = 0; i < this->numCards; i++) 
    {
        if(cards[i] == UNICORN)
            numUnicorns++;
    }
    return numUnicorns;
}

/*  Function: removeUnicorn()
    Goal:     Removes the Nth unicorn where N=unicornNumber; that is, 
              you must count the unicorns until you find the unicornNumber-nth
              unicorn and remove that one; for example, 
                    Suppose unicornNumber = 2
                    Before:   [append 1][append 2][unicorn][append 2][unicorn][append 3]
                                                    ^ 1                 ^ 2
                    After:    [append 1][append 2][unicorn][append 2][append 3] */
void Hand::removeUnicorn(int unicornNumber)
{
    int unicornCounter = 0;
    bool unicornFound = false;

    int i;
    for(i = 0; i < this->numCards && !unicornFound; i++)
    {
        if(this->cards[i] == UNICORN)
            unicornCounter++;

        if(unicornCounter == unicornNumber)
        {
            unicornFound = true;
            removeCard(i);
        }
    }
}

/*  Function: toString()
    Goal:     Generate string for all the cards in the hand  */
std::string Hand::toString() 
{
    std::string str = "";
    for (int i = 0; i < this->numCards; i++)
    {
        str += this->cardInfo->getCardName(this->cards[i]);
    }
    return str;
}