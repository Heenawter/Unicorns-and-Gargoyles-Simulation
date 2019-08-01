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

/*  Function: getNextPlayer()
    Goal:     Gets a pointer to the next player from the given player; 
              Takes into account play order (i.e. reverse or not) and
              handles wrapping around (last player -> first player) */
Player* Game::getNextPlayer(Player* current)
{
    int playerNum = current->getPlayerNum();
    playerNum = (playerNum + gameDirection) % numPlayers;
    if (playerNum < 0) // wrap around for negative (mod doesn't work)
        playerNum = numPlayers - 1;

    return players[playerNum];
}

/*  Function: handleActionCard()
    Goal:     Call the appropriate function depending on which action card
              was drawn;
    Throws:   RanOutOfCardsException   -- if you run out of cards
              OnlyActionCardsException -- if there is no non-action card left 
              (Both exceptions are passed from game->actionCardDraw) */
void Game::handleActionCard(char type)
{
    try
    {
        switch (type)
        {
        case ACTION_CARD_DRAW:
            actionCard_draw();
            break;
        case ACTION_CARD_REVERSE:
            actionCard_reverse();
            break;
        default:
            break;
        }
    }
    catch (RanOutOfCardsException &e1)
    {
        throw e1;
    }
    catch (OnlyActionCardsException &e2)
    {
        throw e2;
    }
}

/*  Function: actionCard_draw()
    Goal:     Have every player (in order, starting from the current player)
              draw a non action card from the deck and add it to their hand
    Throws:   RanOutOfCardsException   -- if you run out of cards
              OnlyActionCardsException -- if there is no non-action card left 
              (Both exceptions are passed from player->drawNonActionCard) */
void Game::actionCard_draw()
{
    std::cout << "draw..." << std::endl;

    try
    {
        Player *current = currentPlayer;
        int playerCount = 0;
        while (playerCount < this->numPlayers)
        {
            std::cout << "- " << current->getPlayerNum() << std::endl;
            current = getNextPlayer(current);
            current->drawNonActionCard();
            playerCount++;
        }
    }
    catch (RanOutOfCardsException &e1)
    {
        throw e1;
    }
    catch (OnlyActionCardsException &e2)
    {
        throw e2;
    }
}

/*  Function: actionCard_reverse()
    Goal:     Reverse the current play order */
void Game::actionCard_reverse()
{
    std::cout << "reverse..." << std::endl;
    this->gameDirection *= -1;
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
    int count = 0;
    char newestCard;

    std::cout << "-- NEW ROUND --" << std::endl;
    while (count < this->numPlayers)
    {
        // a round always consists of <numPlayers> turns; 
        // because of the reverse order card, it is not guaranteed that
        // every player will go in a round.

        std::cout << "Player " << this->currentPlayer->getPlayerNum() << ": " << std::endl;
        // playerTurn(goalString, playerNum);

        try
        {
            newestCard = this->currentPlayer->takeTurn();
            if (cardInfo->isActionCard(newestCard))
            {
                handleActionCard(newestCard);
            }
        }
        catch (RanOutOfCardsException &e1)
        {
            std::cout << "RAN OUT OF CARDS EXCEPTION" << std::endl;
            throw e1;
        }
        catch (OnlyActionCardsException &e4)
        {
            throw e4;
        }
        
        currentPlayer = getNextPlayer(currentPlayer); // advance to the next player
        count++; // and increase the counter for the number of turns in this round
    }
}