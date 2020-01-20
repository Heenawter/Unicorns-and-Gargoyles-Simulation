
#include "Player_Greedy.h"

/**************************************************/
/*               Private Functions                */
/**************************************************/

/******************************* ACTION CARDS ******************************* /

/*  Function: discardCard()
    Goal:     Remove the first card that makes your solution better */
void GreedyPlayer::action_discardCard()
{
    // first, try removing a card that makes the hand better
    bool removed = removeFirst(std::less<int>());

    if(!removed) {
        // then, remove the first card that doesn't make the hand worse...
        std::cout << "... no card made the hand better ..." << std::endl;
        removed = removeFirst(std::less_equal<int>());

        if(!removed) {
            // then, remove the first card
            std::cout << "... can only make the hand worse ..." << std::endl;
        }
    }
}

/*  Function: springCleaning()
    Goal:     Remove a card; if the solution gets better, continue with 
              that hand and try removing another card */
void GreedyPlayer::action_springCleaning()
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    Hand currentHand = Hand(*this->hand);
    int testDistance;
    int currentDistance = currentHand.getDistance();

    for (int i = 0; i < testHand.getNumCards(); i++)
    {
        std::cout << "i: " << i << std::endl;
        testHand.removeCard(i);
        testDistance = testHand.getDistance();
        if (testDistance < currentDistance)
        {
            std::cout << " <<<< " << std::endl;
            currentHand = Hand(testHand);
        }
        testHand = Hand(currentHand);
        std::cout << "Test hand: " << testHand.toString() << std::endl;
        std::cout << "Current hand: " << currentHand.toString() << std::endl;
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

/*  Function: removeFirst()
    Goal:     Remove the first card such that func(testDistance, currentDistance)
              returns true; this function expects <, <=, ==, >=, > for func; 
              for example, if func = < = std::less,
                    - find and remove the first card such that, when it is 
                      removed, it makes the hand BETTER (i.e. closer to 
                      the goal string) */
bool GreedyPlayer::removeFirst(std::function<bool(int, int)> func)
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    int testDistance = this->hand->getDistance();
    int currentDistance = testDistance;
    bool removed = false;

    std::cout << "before: " << this->hand->toString() << std::endl;
    std::cout << "--- distance: " << currentDistance << std::endl;
    for (int i = 0; i < testHand.getNumCards() && !removed; i++)
    {
        testHand.removeCard(i);
        testDistance = testHand.getDistance();

        std::cout << i << ": " << testDistance << " ? " << currentDistance << " = ";
        std::cout << std::boolalpha << func(testDistance, currentDistance) << std::endl;

        if (func(testDistance, currentDistance)) // either <, <=, >, >=, or ==
        {
            std::cout << "removed " << i << std::endl;
            removed = true;
            this->hand->removeCard(i);
        }
        else
        {
            testHand = Hand(*this->hand);
        }
    }
    std::cout << "after: " << this->hand->toString() << std::endl;
    std::cout << "--- distance: " << this->hand->getDistance() << std::endl;
    return removed;
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
    action_discardCard();
    // action_springCleaning();
    return 'n';
}