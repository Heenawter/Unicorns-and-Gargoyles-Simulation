#include "Debugging.h"
#include "Game.h"
#include "GameStates.h"

#include <fstream>

/*
    - loop through each goal
        - loop through 2-10 players
            - report statistics
*/

void runGame(int numPlayers, std::string goal);

int main()
{
    std::cout << "main" << std::endl;

    std::ifstream file;
    file.open("./GameInfo/Goals.txt");

    if (file.is_open())
    {
        std::string goal;
        int numGoals;

        /* read the goals */
        file >> numGoals;         // the number of goals to try
        std::getline(file, goal); // garbage read of new line
        for (int i = 0; i < numGoals; i++)
        {
            std::getline(file, goal);
            std::cout << goal;

            for(int numPlayers = 2; numPlayers <= 10; numPlayers++)
            {
                std::cout << ".";
                runGame(numPlayers, goal);
            }
            std::cout << std::endl;
        }

        file.close();
    }
}

void runGame(int numPlayers, std::string goal)
{
    Game *game = new Game(numPlayers, goal);
    int roundCount = 0;
    bool win = false;
    try
    {

        while (!win)
        {
            win = game->gameRound();
            roundCount++;
        }
    }
    catch (RanOutOfCardsException &e1)
    {
        // std::cout << e1.what();
    }
    catch (OnlyActionCardsException &e2)
    {
        // std::cout << e2.what();
    }

    LOG("There were " + std::to_string(roundCount) + " rounds. \n");
    delete game;
}