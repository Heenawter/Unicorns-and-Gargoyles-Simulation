#include "Constants.h"
#include "Decks.h"
#include "Player.h"
#include "Game.h"

// #include <iostream>

Game::Game(int numPlayers, std::string goal)
{
    deck = new Deck();

    readGameStats();
    deck->shuffleDeck();

    for (int i = 0; i < numPlayers; i++)
        players.push_back(new Player(deck));

    goalString = goal;
    winningPlayer = MAX_INT;
    gameDirection = 1;
    startingPlayer = 0;
}

Game::~Game()
{
    std::vector<Player *>::iterator it;
    for (it = players.begin(); it != players.end(); it++)
    {
        delete *it;
    }
    delete deck;
}

char Game::playerTurn(std::string goalString, int playerNum)
{
    int playerNum2;
    Player *currentPlayer;
    char newCard;
    char gameStatus = 'X';

    LOG("Player " + std::to_string(playerNum + 1) + " --- ");

    currentPlayer = getPlayer(playerNum);
    newCard = currentPlayer->takeTurn(goalString);

    if (newCard == ACTION_CARD_REVERSE)
    {
        LOG("Action card REVERSE\n");
        // reverse the player order
        gameDirection *= -1;
        gameStatus = REVERSE_ORDER;
        deck->discardCard(newCard);
    }
    else    
    {
        if (newCard == ACTION_CARD_DISCARD)
        {
            LOG("Action card DISCARD\n");
            // EACH PLAYER discards a card from their hand regardless of
            // who pulled the action card

            for (playerNum2 = 0; playerNum2 < NUM_PLAYERS; playerNum2++)
            {
                getPlayer(playerNum2)->discardCard(goalString);
                LOG("---> Player " + std::to_string(playerNum2 + 1) + " --- ");
                getPlayer(playerNum2)->printCurrentHand();
            }
            deck->discardCard(newCard);
        }
        else if (newCard == ACTION_CARD_DRAW)
        {
            // EACH PLAYER draws a non-action card and appends it
            // to their hand, regardless of who pulled the action card
            LOG("Action card DRAW\n");

            char testCard;
            for (playerNum2 = 0; playerNum2 < NUM_PLAYERS; playerNum2++)
            {
                testCard = getPlayer(playerNum2)->drawCard(goalString);
                while (testCard >= ACTION_CARD_DISCARD && deck->hasNonActionCard())
                {
                    deck->discardCard(testCard); // put the action card back
                    // and draw a new card until it is NOT an action card
                    testCard = getPlayer(playerNum2)->drawCard(goalString);
                }

                LOG("---> Player " + std::to_string(playerNum2 + 1) + " --- ");
                getPlayer(playerNum2)->printCurrentHand();
            }
            // deck->discardCard(newCard);
        }
        else if (newCard == ACTION_CARD_SPRING_CLEANING)
        {
            LOG("Action card SPRING CLEANING\n");

            for (playerNum2 = 0; playerNum2 < NUM_PLAYERS; playerNum2++)
            {
                getPlayer(playerNum2)->springCleaning(goalString);
                LOG("---> Player " + std::to_string(playerNum2 + 1) + " --- ");
                getPlayer(playerNum2)->printCurrentHand();
            }

            deck->discardCard(newCard);
        }
        else if (newCard == ACTION_CARD_POISON)
        {
            LOG("Action card POISON\n");

            std::vector<Player*> otherPlayers = players;
            otherPlayers.erase(otherPlayers.begin() + playerNum);
            
            getPlayer(playerNum)->poisonCard(goalString, otherPlayers);

            for (playerNum2 = 0; playerNum2 < NUM_PLAYERS; playerNum2++)
            {
                LOG("---> Player " + std::to_string(playerNum2 + 1) + " --- ");
                getPlayer(playerNum2)->printCurrentHand();
            }
        }
        else if (newCard == ACTION_CARD_STEAL)
        {
            LOG("Action card STEAL\n");

            std::vector<Player *> otherPlayers = players;
            otherPlayers.erase(otherPlayers.begin() + playerNum);

            getPlayer(playerNum)->stealCard(goalString, otherPlayers);

            for (playerNum2 = 0; playerNum2 < NUM_PLAYERS; playerNum2++)
            {
                LOG("---> Player " + std::to_string(playerNum2 + 1) + " --- ");
                getPlayer(playerNum2)->printCurrentHand();
            }

            deck->discardCard(newCard);
        } 
        else
        {
            currentPlayer->printCurrentHand(); // LOG STATEMENTS
        }


        if (currentPlayer->winningCondition())
        {
            LOG("WINNING HAND: ");
            getPlayer(playerNum)->printCurrentHand();

            gameStatus = WIN;
            winningPlayer = playerNum;
        }
        else if (!deck->hasCards())
        {
            LOG("Ran out of cards");
            gameStatus = RAN_OUT_OF_CARDS;
        }
    }

    return gameStatus;
}

// returns game status
char Game::gameRound(std::string goalString)
{
    // std::cout << "\nNEW ROUND ---- ";
    int playerNum = startingPlayer;
    char gameStatus = 'X';
    int count = 0;
    while(gameStatus == 'X' && count < NUM_PLAYERS)
    {
        gameStatus = playerTurn(goalString, playerNum);
        playerNum = (playerNum + gameDirection) % NUM_PLAYERS;
        if (playerNum < 0)  // wrap around for negative (mod doesn't work)
            playerNum = NUM_PLAYERS - 1;

        if (gameStatus == REVERSE_ORDER)
            startingPlayer = playerNum;
        else 
            count++;
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
            deck->addToDeck(line, cardCounter);
            cardCounter++;
        }

        file.close();
    }
}