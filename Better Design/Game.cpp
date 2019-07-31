#include "Game.h"

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
        players.push_back(new Player(deck, goal, cardInfo));
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
    int playerNum = startingPlayer;
    int count = 0;
    while (count < NUM_PLAYERS)
    {
        // playerTurn(goalString, playerNum);
        playerNum = (playerNum + gameDirection) % NUM_PLAYERS;
        if (playerNum < 0) // wrap around for negative (mod doesn't work)
            playerNum = NUM_PLAYERS - 1;

        // if (gameStatus == REVERSE_ORDER)
        //     startingPlayer = playerNum;
        // else
        //     count++;
    }
}