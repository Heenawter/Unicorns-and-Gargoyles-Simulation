
#include "Decks.h"
#include "Player.h"

#include <vector>
#include <iostream>
#include <algorithm>

void Player::generate_permutations(StringDeck &deck)
{
    int permCount = 0;

    std::sort(allCards.begin(), allCards.end());

    do {
        // std::cout << "Perm #" << permCount + 1 << ": ";
        // printHand(deck, allCards);
        permCount++;
    } while (std::next_permutation(allCards.begin(), allCards.end()));

    std::cout << "Perm count: " << permCount << std::endl;
}

void Player::drawCard(StringDeck &deck)
{
    char newCard = deck.drawCard();
    // allCards.push_back(newCard);

    std::vector<char> currentHand;

    int numHands = allHands.size();
    int numCards;

    int i, j;
    if(numHands == 0) {
        std::vector<char> newHand;
        newHand.push_back(newCard);
        allHands.insert(newHand);
    } else {
        std::unordered_set<std::vector<char>, VectorHash>::iterator it;
        std::unordered_set<std::vector<char>, VectorHash> newHands;

        for(it = allHands.begin(); it != allHands.end(); it++)
        {
            currentHand = *it;
            numCards = currentHand.size();
            for (j = 0; j <= numCards; j++)
            {
                std::vector<char> newHand(currentHand);
                newHand.insert(newHand.begin() + j, newCard);
                newHands.insert(newHand);
            }
        }

        allHands = newHands;
    }
}

std::string Player::generateString(StringDeck &deck, std::vector<char> &hand, std::string current)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++) {
        deck.playCard(*it, current);
    }

    return current;
}

void Player::printHand(StringDeck &deck, std::vector<char> hand)
{
    std::vector<char>::iterator it;
    for (it = hand.begin(); it < hand.end(); it++)
    {
        std::cout << "[" << deck.getCardName(*it) << "]";
    }
    std::cout << std::endl;
}

void Player::takeTurn(StringDeck &deck)
{
    drawCard(deck);
}

void Player::printAll(StringDeck &deck)
{
    std::cout << "print..." << std::endl;
    // std::list<std::vector<char>>::iterator it;
    std::unordered_set<std::vector<char>, VectorHash>::iterator it;

    std::vector<char> hand;
    for (it = allHands.begin(); it != allHands.end(); it++)
    {
        printHand(deck, *it);
    }
}

void Player::printSize(int numCards) {
    int numPerms = allHands.size();
    int elementSize = sizeof(char) * numCards;
    int handSize = elementSize + sizeof(std::vector<char>);
    int permSize = handSize * numPerms;

    std::cout << "Number of permutations:   " << numPerms << std::endl;
    std::cout << "Size of each hand:        " << elementSize << " bytes" << std::endl; 
    std::cout << "Size of each vector:      " << handSize << " bytes" << std::endl;
    std::cout << "Size of all permutations: " << permSize << " bytes" << std::endl;
}