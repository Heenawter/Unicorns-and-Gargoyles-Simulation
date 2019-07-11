#include "Constants.h"
#include "Decks.h"
#include "Player.h"
#include "Game.h"

// #include <iostream>

Game::Game(int numPlayers, std::string goal)
{
    readGameStats();
    deck.shuffleDeck();

    for (int i = 0; i < numPlayers; i++)
        players.push_back(new Player());

    goalString = goal;
    winningPlayer = MAX_INT;
}

Game::~Game()
{
    std::vector<Player *>::iterator it;
    for (it = players.begin(); it != players.end(); it++)
    {
        delete *it;
    }
}

char Game::playerTurn(std::string goalString, int playerNum)
{
    int playerNum2;
    Player *currentPlayer;
    char newCard;
    char gameStatus = 'X';

    // std::cout << "Player " << playerNum + 1 << " --- ";
    currentPlayer = getPlayer(playerNum);
    newCard = currentPlayer->takeTurn(deck, goalString);

    if (newCard == ACTION_CARD_DISCARD)
    {
        // std::cout << "\n --- Action card DISCARD --- \n";
        // EACH PLAYER discards a card from their hand regardless of
        // who pulled the action card

        for (playerNum2 = 0; playerNum2 < NUM_PLAYERS; playerNum2++)
        {
            getPlayer(playerNum2)->discardCard(deck, goalString);
            // std::cout << " --- Player " << playerNum2 + 1 << " --- ";
            // getPlayer(playerNum2)->printCurrentHand(deck);
        }
    }
    else if (newCard == ACTION_CARD_DRAW)
    {
        // EACH PLAYER draws a non-action card and appends it
        // to their hand, regardless of who pulled the action card
        // std::cout << "\n --- Action card DRAW --- \n";

        char testCard;
        for (playerNum2 = 0; playerNum2 < NUM_PLAYERS; playerNum2++)
        {
            testCard = getPlayer(playerNum2)->drawCard(deck, goalString);
            while (testCard >= ACTION_CARD_DISCARD && deck.hasNonActionCard())
            {
                deck.putCardBack(testCard); // put the action card back
                // and draw a new card until it is NOT an action card
                testCard = getPlayer(playerNum2)->drawCard(deck, goalString);
            }
            // std::cout << " --- Player " << playerNum2 + 1 << " --- ";
            // getPlayer(playerNum2)->printCurrentHand(deck);
        }
    }
    else if (newCard == ACTION_CARD_SPRING_CLEANING)
    {
        // std::cout << "\n --- Action card SPRING CLEANING --- \n";
        for (playerNum2 = 0; playerNum2 < NUM_PLAYERS; playerNum2++)
        {
            getPlayer(playerNum2)->springCleaning(deck, goalString);
            // std::cout << " --- Player " << playerNum2 + 1 << " --- ";
            // getPlayer(playerNum2)->printCurrentHand(deck);
        }
    }

    // currentPlayer->printCurrentHand(deck);

    if (currentPlayer->winningCondition())
    {
        gameStatus = WIN;
        winningPlayer = playerNum;
    }
    else if (!deck.hasCards())
    {
        gameStatus = RAN_OUT_OF_CARDS;
    }

    // currentPlayer->printCurrentHand(deck);

    return gameStatus;
}

// returns game status
char Game::gameRound(std::string goalString)
{
    bool keepLooping = true;

    int playerNum;
    char gameStatus = 'X';

    if (!reversePlayOrder)
    {
        for (playerNum = 0; playerNum < NUM_PLAYERS & gameStatus == 'X'; playerNum++)
            gameStatus = playerTurn(goalString, playerNum);
    }
    else
    {
        for (playerNum = NUM_PLAYERS - 1; playerNum >= 0 & gameStatus == 'X'; playerNum--)
            gameStatus = playerTurn(goalString, playerNum);
    }

    return gameStatus;
}

void Game::readGameStats()
{
    std::ifstream file;
    file.open(INPUT_FILE);

    if (file.is_open())
    {
        std::string line;
        int numCards;
        int i;
        char cardCounter = 0;

        /* read the string deck */
        std::getline(file, line); // garbage read of first line
        file >> numCards;         // the number of cards to read
        std::getline(file, line); // garbage read of new line
        for (i = 0; i < numCards; i++)
        {
            std::getline(file, line);
            deck.addToDeck(line, cardCounter);
            cardCounter++;
        }

        file.close();
    }
}

Deck *Game::getDeck()
{
    return &deck;
}

Player *Game::getPlayer(int index)
{
    return players.at(index);
}