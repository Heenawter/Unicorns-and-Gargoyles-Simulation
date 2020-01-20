
#include "Player_Greedy.h"


/**************************************************/
/*               Private Functions                */
/**************************************************/

/******************************* ACTION CARDS ******************************* /

/*  Function: discardCard()
    Goal:     Based on the goal, try to remove the card that gets you CLOSER;
              if there is no card that makes your hand bettter when removed,
              then simply remove a card that does the LEAST DAMAGE. 
                   - Best case: Removal causes an IMPROVEMENT
                   - Worst case: Removal either causes no damage OR 
                                 removal does the least damage possible
              For finding best case card, if there is a tie,
                    - remove the first one encountered
              For finding the worst case card, if there is a tie,
                    - remove the last one encountered 
                    - Why? Because, greedily, it is easier to 
                           append cards to the end, so removing
                           from the end is better 
              WAIT THIS ISN'T GREEDY ---- GREEDY WOULD BE LIKE, 
              Remove the first card that makes your solution better */
void GreedyPlayer::action_discardCard()
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy

    int testDistance = 0;
    int bestDistance = this->hand->getDistance();
    int worstDistance = MAX_INT;
    int cardToRemove_bestCase = -1;
    int cardToRemove_worstCase = -1;

    // first, see if any card will IMPROVE the current hand....
    for (int i = 0; i < this->hand->getNumCards(); i++)
    {
        // try removing every single card once
        testHand.removeCard(i);
        testDistance = testHand.getDistance();
        if (testDistance < bestDistance) 
        {
            // removing the card either made the hand BETTER or kept it the same
            bestDistance = testDistance;
            cardToRemove_bestCase = i;
        } 

        if(testDistance <= worstDistance)
        {
            // removing the card either made the hand WORSE or kept it the same
            worstDistance = testDistance;
            cardToRemove_worstCase = i;
        }
    
        testHand = Hand(*this->hand); // start over
    }

    std::cout << "Previous hand: " << this->hand->toString() << std::endl;
    std::cout << "Current distance: " << this->hand->getDistance() << std::endl;

    if (cardToRemove_bestCase != -1) {
        // there is a card that makes the hand BETTER, so remove that one
        this->hand->removeCard(cardToRemove_bestCase);
        std::cout << "Removal of card " << cardToRemove_bestCase << " made hand better..." << std::endl;
    } else {
        // no removal improves the hand, so remove the card that has the least impact
        this->hand->removeCard(cardToRemove_worstCase);
        std::cout << "Removal of card " << cardToRemove_worstCase << " was the best option..." << std::endl;
    }

    std::cout << "New hand: " << this->hand->toString() << std::endl;
    std::cout << "New distance: " << this->hand->getDistance() << std::endl;
}

/*  Function: springCleaning()
    Goal:     Remove cards ONLY if one makes the solution better; otherwise, 
              just keep everything (remember, spring cleaning makes removal
              optional, whereas discardCard action is required) 
              WAIT THIS ISN'T GREEDY ---- GREEDY WOULD BE LIKE, 
              Remove a card; if the solution gets better, continue with 
              that hand and try removing another card */
void GreedyPlayer::action_springCleaning()
{
    Hand combinations = Hand(*this->hand);
    Hand bestHand = Hand(*this->hand);

    // std::cout << "Distance: " << this->hand->getDistance() << std::endl;

    int numCards = this->hand->getNumCards();
    for (int i = 1; i <= numCards; i++)
    {
        // loop through subsets of ALL sizes, including the subset that would be
        // created by removing ZERO cards (i -less than OR EQUAL TO- numCards)
        combinationUtil(*this->hand, combinations, bestHand, 0, numCards - 1, 0, i);
    }
    // std::cout << "Distance: " << bestHand.getDistance() << std::endl;

    // now that we know which hand we are aiming for (bestHand), we must
    // actually CREATE that hand by discarding the appropriate cards
    std::vector<char> setDifference = this->hand->setDifference(bestHand);
    std::cout << this->hand->toString() << " ---> " << bestHand.toString() << std::endl;
    delete this->hand;
    this->hand = new Hand(bestHand);

    // std::cout << this->deck->toString() << std::endl;
    for(int i = 0; i < setDifference.size(); i++)
    {
        // std::cout << this->cardInfo->getCardName(setDifference[i]);
        std::cout << "-- discard "<< this->cardInfo->getCardName(setDifference[i]) << std::endl;
        this->deck->discardCard(setDifference[i]);
    }
}

/*  Function: poisonUnicorn()
    Goal:     */
std::tuple<Player *, int> GreedyPlayer::action_poisonUnicorn()
{
    return std::tuple<Player *, int>(NULL, 1);
}

/*  Function: stealCard()
    Goal:     */
std::tuple<Player *, int> GreedyPlayer::action_stealCard()
{
    return std::tuple<Player *, int>(NULL, 1);
}

/*  Function: combinationUtil()
    Goal:     This function is used to build all of the possible subsets 
              of length r (WITHOUT REORDERING CARDS) of "hand" parameter
              and finds the hand that is closest to the goal string.
              This is a helper function for springCleaning */
// https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
void GreedyPlayer::combinationUtil(Hand hand, Hand tempHand, Hand &bestHand, 
                             int start, int end, int index, int r)
{
    // base case
    if (index == r)
    {
        Hand newHand = Hand(tempHand, r);

        if (newHand <= bestHand)
        {
            // because <= and not <, this implies we are being CONSERVATIVE i.e. the player is
            // choosing to remove AS FEW CARDS AS POSSIBLE while still maintaining the best distance
            bestHand = newHand;
        }

        return;
    }

    // replace index with all possible elements. The condition "end-i+1 >= r-index"
    // makes sure that including one element at index will make a combination with
    // remaining elements at remaining positions
    for (int i = start; i <= end && end - i + 1 >= r - index; i++)
    {
        tempHand.changeCard(index, hand.getCard(i));
        combinationUtil(hand, tempHand, bestHand, i + 1, end, index + 1, r);
    }
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

GreedyPlayer::GreedyPlayer(Deck *deck, std::string goalString, Cards *cardInfo, int playerNum)
    : Player(deck, goalString, cardInfo, playerNum)
{
    
}

char GreedyPlayer::takeTurn()
{
    // action_discardCard();
    action_springCleaning();
    return 'n';
}