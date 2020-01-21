
#include "Player_Greedy.h"

/**************************************************/
/*               Private Functions                */
/**************************************************/

/******************************* ACTION CARDS ******************************* /

/*  Function: discardCard()
    Goal:     Remove the first card that makes your solution better; if no card
              IMPROVES your hand, then remove one that at least doesn't make it
              any worse; otherwise, if no such card exists, just remove the
              first card in your hand regardless of its impact */
void GreedyPlayer::action_discardCard()
{
    std::cout << "before: " << this->hand->getDistance() << ", " << this->hand->toString() << std::endl;
    // first, try removing a card that makes the hand better
    bool removed = removeFirst(std::less<int>());

    if(!removed) {
        // then, remove the first card that doesn't make the hand worse...
        removed = removeFirst(std::less_equal<int>());

        if(!removed) {
            // then, just remove the first card regardless of the consequences
            this->hand->removeCard(0);
        }
    }
    std::cout << "after:  " << this->hand->getDistance() << ", " << this->hand->toString() << std::endl;
}

/*  Function: springCleaning()
    Goal:     Remove a card; if the solution gets better, continue with 
              that hand and try removing another card.
              Basically, go through the hand ONCE and, every time you 
              encounter a card such that, when it is removed, the hand
              gets closer to the goal, remove it and continue */
void GreedyPlayer::action_springCleaning()
{
    Hand testHand = Hand(*this->hand); // use copy constructor to make a copy
    Hand currentHand = Hand(*this->hand);
    int testDistance;
    int currentDistance = currentHand.getDistance();

    for (int i = 0; i < testHand.getNumCards(); i++)
    {
        testHand.removeCard(i);
        testDistance = testHand.getDistance();

        std::cout << i << ", distance: " << currentHand.getDistance();
        std::cout << ", current hand: " << currentHand.toString() << std::endl;
        std::cout << i << ", distance: " << testHand.getDistance();
        std::cout << ", test hand:    " << testHand.toString() << std::endl;

        if (testDistance < currentDistance)
        {
            std::cout << "... remove card " << i << " ... " << std::endl;
            currentHand.removeCard(i);
            currentDistance = currentHand.getDistance();
            i--;
        }
        testHand = Hand(currentHand);
        // std::cout << "Test hand: " << testHand.toString() << std::endl;
    }
    delete this->hand;
    this->hand = new Hand(currentHand);
}

/*  Function: poisonUnicorn()
    Goal:     Target the player and unicorn such that, when the unicorn
              is removed, that player's solution gets worse; in other words,
                    for each player, 
                        for each unicorn in their hand,
                            if removal of that unicorn makes the player's
                            hand worse (i.e. makes their distance larger),
                            then target that player + that unicorn 
                            and exit both loops 
              If there are no unicorns to poison, simply do nothing
              If no removal makes the solutions worse, try >= rather than >
              If removal only makes the solutions BETTER, target the first
              player and the first unicorn */
std::tuple<Player *, int> GreedyPlayer::action_poisonUnicorn()
{
    bool removed = false;

    Player* currentPlayer;
    Hand* currentHand;
    char currentCard;
    int currentDistance;
    Hand testHand = Hand(*this->hand);
    int testDistance;

    for(int i = 0; i < this->otherPlayers.size() && !removed; i++)
    {
        currentPlayer = this->otherPlayers[i];
        currentHand = currentPlayer->getHand();
        for(int j = 0; j < currentHand->getNumCards() && !removed; j++)
        {
            currentCard = currentHand->getCard(j);
            currentDistance = currentHand->getDistance();
            if(currentCard == UNICORN)
            {
                std::cout << "Before: " << currentPlayer->toString() << std::endl;
                testHand = Hand(*currentHand);
                testHand.removeCard(j);
                testDistance = testHand.getDistance();
                std::cout << "After: " << testHand.toString() << std::endl;

                if(testDistance > currentDistance)
                {
                    removed = true;
                    std::cout << "made it worse!!" << std::endl;
                }
            }
        }
    }

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

    for (int i = 0; i < testHand.getNumCards() && !removed; i++)
    {
        testHand.removeCard(i);
        testDistance = testHand.getDistance();

        if (func(testDistance, currentDistance)) // either <, <=, >, >=, or ==
        {
            removed = true;
            this->hand->removeCard(i);
        }
        else
        {
            testHand = Hand(*this->hand);
        }
    }
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
    // action_discardCard();
    // action_springCleaning();
    action_poisonUnicorn();
    return 'n';
}