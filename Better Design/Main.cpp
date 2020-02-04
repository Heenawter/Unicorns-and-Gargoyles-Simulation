#include "Debugging.h"
#include "Game.h"
#include "GameStates.h"

#include <fstream>

/*
    - loop through each goal
        - loop through 2-10 players
            - report statistics
*/

void testPlayerType();
void runAnalysis();
void runGame(int numPlayers, std::string goal, std::ofstream &outputFile);

int main()
{
    // std::cout << "main" << std::endl;
    runAnalysis();

    // std::ofstream outputFile;
    // outputFile.open("./test.txt");

    // runGame(5, "[ ][ ][ ][*]", outputFile);

    // outputFile.close();
    // testPlayerType();
}

void testPlayerType()
{
    Game *game = new Game(5, "[ ][ ][ ][*]");
    Player* current;
    for(int i = 0; i < 5; i++)
    {
        current = game->getPlayer(i);
        for(int j = 0; j < 4; j++)
        {
            current->action_drawNonActionCard();
        }
    }

    for (int i = 0; i < 5; i++)
    {
        current = game->getPlayer(i);
        std::cout << i << ": " << current->toString() << ", distance: " << current->getHand()->getDistance() << std::endl;
    }
    

    current = game->getPlayer(0); 
    std::cout << "Before: " << current->toString() << std::endl;
    // current->action_poisonUnicorn_helper();
    current->takeTurn();
    // current->action_stealCard_helper();
    std::cout << "After: " << current->toString() << std::endl;

    // std::cout << current->toString() << std::endl;

    delete game;
}

void runAnalysis()
{
    std::ifstream goalsFile;
    goalsFile.open("./GameInfo/Goals.txt");

    std::ofstream outputFile;
    outputFile.open("./GameInfo/AggressiveResults_fixed.csv");

    if (goalsFile.is_open() && outputFile.is_open())
    {
        std::string goal;
        int numGoals;

        /* read the goals */
        goalsFile >> numGoals;         // the number of goals to try
        std::getline(goalsFile, goal); // garbage read of new line

        outputFile << "Goal,Number of Players,Number of Rounds,End Result" << std::endl;
        for (int i = 0; i < numGoals; i++)
        {
            std::getline(goalsFile, goal);
            std::cout << goal;

            for (int numPlayers = 2; numPlayers <= 10; numPlayers++)
            {
                for (int j = 0; j < 300; j++)
                {
                    std::cout << ".";
                    runGame(numPlayers, goal, outputFile);
                }
            }
            std::cout << std::endl;
        }

        goalsFile.close();
        outputFile.close();
    }
}

void runGame(int numPlayers, std::string goal, std::ofstream &outputFile)
{
    Game *game = new Game(numPlayers, goal);
    int roundCount = 0;
    bool win = false;
    std::string endResult = "";

    outputFile << goal << ",";
    outputFile << numPlayers << ",";

    try
    {

        while (!win)
        {
            win = game->gameRound();
            roundCount++;
        }
        endResult = "Player " + std::to_string(game->getWinningPlayer()) + " wins";

    }
    catch (RanOutOfCardsException &e1)
    {
        // std::cout << e1.what();
        endResult = "Ran out of cards";
    }
    catch (OnlyActionCardsException &e2)
    {
        // std::cout << e2.what();
        endResult = "Only action cards left";
    }

    outputFile << roundCount << ",";
    outputFile << endResult << std::endl;
    LOG("There were " + std::to_string(roundCount) + " rounds. \n");
    delete game;
}