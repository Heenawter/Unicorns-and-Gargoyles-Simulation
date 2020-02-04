#include "Hand.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

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

/*  Function: Hand(Hand)
    Goal:     Copy constructor; make a copy of the given hand */
Hand::Hand(const Hand &oldHand)
{
    this->cards = oldHand.cards;

    this->goalString = oldHand.goalString;
    this->cardInfo = oldHand.cardInfo;
    this->numCards = oldHand.numCards;
    this->currentDistance = oldHand.currentDistance;
    this->currentString = oldHand.currentString;
}

/*  Function: Hand(Hand, r)
    Goal:     Copies the hand from 0 to r */
Hand::Hand(Hand oldHand, int r)
{
    for(int i = 0; i < r; i++)
    {
        this->cards.push_back(oldHand.getCard(i));
    }

    this->goalString = oldHand.goalString;
    this->cardInfo = oldHand.cardInfo;
    this->numCards = r;
    this->currentString = generateString();
    this->currentDistance = stringDistance(this->currentString, this->goalString);
}

/*  Function: generateString()
    Goal:     Generates the string for the current hand */
std::string Hand::generateString()
{
    return cardInfo->generateString(this->cards);
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
              the goal string; DOES NOT ADD BACK TO DECK
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

std::vector<int> Hand::setDifference(Hand otherHand)
{
    std::vector<int> cardsRemoved;
    std::vector<char> smaller = otherHand.getCards();
    std::vector<char> larger = this->cards;

    // other is guaranteed to be <= the size of "this"
    int i = 0;
    int j = 0;
    while(i < larger.size() && j < smaller.size())
    {
        if (larger[i] == smaller[j])
        {
            i++;
            j++;
        } else {
            cardsRemoved.push_back(i);
            i++;
        }
    }
    while (i < larger.size())
    {
        // remove the rest of the hand

        cardsRemoved.push_back(i);
        i++;
    }
    
    return cardsRemoved;
}