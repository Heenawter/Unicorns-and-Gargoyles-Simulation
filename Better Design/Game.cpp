#include "Game.h"
#include "GameStates.h"

/**************************************************/
/*                Private Functions               */
/**************************************************/

/*  Function: readCards()
    Goal:     Read the card information from the given file;
              it should contain the card names and their counts */
void Game::readCards()
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
        file >> numCards;         // the number of cards to read
        std::getline(file, line); // garbage read of new line
        for (i = 0; i < numCards; i++)
        {
            std::getline(file, line);
            addToMaps(line, cardCounter);
            cardCounter++;
        }

        file.close();
    }
}

/*  Function: addToMaps()
    Goal:     Adds a card to the map from the line in the file */
void Game::addToMaps(std::string line, char cardType)
{
    std::string cardName = line.substr(0, line.find_first_of(','));
    int count = int(line[line.length() - 1]) - 48; // -48 to convert from ascii to int

    cardMap[cardType] = cardName;
    cardCounts[cardType] = count;
}

int Game::getNextPlayer(int currentPlayerIndex)
{
    int playerNum = currentPlayerIndex;
    playerNum = (playerNum + gameDirection) % numPlayers;
    if (playerNum < 0) // wrap around for negative (mod doesn't work)
        playerNum = numPlayers - 1;
    return playerNum;
}

/**************************************************/
/*                Public Functions                */
/**************************************************/

/*  Function: Game()
    Goal:     Constructor for the game object; 
              requires the number of players and the goal for that round. 
              Reads the card info (MOST LIKELY SHOULD MOVE TO SIMULATION)
              Creates the card, table, and player objects. */
Game::Game(int numPlayers, std::string goal)
{
    this->numPlayers = numPlayers;

    readCards();
    cardInfo = new Cards(cardMap);
    deck = new Deck(cardCounts, cardInfo);

    for(int i = 0; i < this->numPlayers; i++)
    {
        players.push_back(new Player(deck, goal, cardInfo, i));
    }

    winningPlayer = MAX_INT;
    gameDirection = 1;
    startingPlayer = 0;
    currentPlayer = players[0];
}

/*  Function: ~Game()
    Goal:     Deconstructor for the game object; 
              For every new, there must be a delete. */
Game::~Game()
{
    delete cardInfo;
    delete deck;
    for (int i = 0; i < this->numPlayers; i++)
    {
        delete(players[i]);
    }
}

void Game::gameRound()
{
    int playerNum = this->startingPlayer;
    int count = 0;

    try
    {
        while (count < this->numPlayers)
        {
            // playerTurn(goalString, playerNum);
            playerNum = getNextPlayer(playerNum);
            players[playerNum]->takeTurn();
            count++;

            // if (gameStatus == REVERSE_ORDER)
            //     startingPlayer = playerNum;
            // else
            //     count++;
        }
    }
    catch (RanOutOfCardsException &e1)
    {
        throw e1;
    }
    catch (ActionCardException &e2)
    {
        try
        {
            handleActionCard(e2.triggeringPlayer, e2.type);
        }
        catch (RanOutOfCardsException &e3)
        {
            throw e3;
        }
        catch (OnlyActionCardsException &e4)
        {
            std::cout << "throw... ONLY ACTION CARDS" << std::endl;
            throw e4;
        }
    }
}

void Game::handleActionCard(Player *triggeringPlayer, char type)
{
    switch (type)
    {
    case ACTION_CARD_DRAW:
        try
        {
            actionCard_draw(triggeringPlayer);
        }
        catch (RanOutOfCardsException &e1)
        {
            throw e1;
        }
        catch (OnlyActionCardsException &e2)
        {
            throw e2;
        }
        break;
    case ACTION_CARD_REVERSE:
        actionCard_reverse(triggeringPlayer);
    default:
        break;
    }
}

void Game::actionCard_draw(Player* triggeringPlayer)
{
    // start by finding the index of the triggering player
    int playerNum = triggeringPlayer->getPlayerNum();
    int playerCount = 0;
    while(playerCount < this->numPlayers)
    {
        playerNum = getNextPlayer(playerNum);
        try
        {
            players[playerNum]->drawNonActionCard();
        }
        catch (RanOutOfCardsException &e1)
        {
            throw e1;
        }
        catch (OnlyActionCardsException &e2)
        {
            throw e2;
        }

        playerCount++;
        std::cout << "Count: " << playerCount << " --- " << playerNum << std::endl;
    }

    std::cout << "player num: " << playerNum << std::endl;
}

void Game::actionCard_reverse(Player* triggeringPlayer)
{
    // start by finding the index of the triggering player
    int playerNum = triggeringPlayer->getPlayerNum();
    this->gameDirection *= -1;

    int nextPlayer = getNextPlayer(playerNum);
    currentPlayer = players[nextPlayer];
    startingPlayer = nextPlayer;
}