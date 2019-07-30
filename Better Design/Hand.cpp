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

/**************************************************/
/*                Public Functions                */
/**************************************************/

Hand::Hand(Cards* cardInfo)
{
    this->cardInfo = cardInfo;
    this->numCards = 0;
    this->currentDistance = MAX_INT;
    this->currentString = "";
}

std::string Hand::getCardName(char card)
{
    cardInfo->getCardName(card);
}

std::string Hand::generateString()
{
    return cardInfo->generateString(this->cards);
}

std::string Hand::generateString(std::vector<char> hand)
{
    return cardInfo->generateString(hand);
}

bool Hand::operator<(Hand* h2)
{
    int distance1 = this->currentDistance;
    int distance2 = h2->currentDistance;
    return distance1 < distance2;
}

void Hand::addToHand(char card, std::string goalString)
{
    this->cards.push_back(card);
    this->numCards++;
    this->currentString = generateString();
    this->currentDistance = stringDistance(this->currentString, goalString);
}