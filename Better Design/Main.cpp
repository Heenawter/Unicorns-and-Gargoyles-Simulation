#include "Debugging.h"
#include "Game.h"
#include "GameStates.h"

#include <fstream>

/*
    - loop through each goal
        - loop through 2-10 players
            - report statistics
*/

void testPlayerType(int numPlayers, std::string type);
void runAnalysis(int numPlayers);
void runGame(int numPlayers, std::vector<std::string> playerTypes,
             std::string goal, std::ofstream &outputFile, std::ofstream &handOutputFile);
void generateGoals_Recursive(std::ofstream &outputFile, std::string prefix, int k, int maxUnicorns);
void generateGoals(int minLength, int maxLength, int maxUnicorns);


int main()
{
<<<<<<< HEAD

    // if (__cplusplus == 201703L) std::cout << "C++17\n";
    // else if (__cplusplus == 201402L) std::cout << "C++14\n";
    // else if (__cplusplus == 201103L) std::cout << "C++11\n";
    // else if (__cplusplus == 199711L) std::cout << "C++98\n";
    // else std::cout << "pre-standard C++\n";

    // std::cout << "main" << std::endl;
    runAnalysis(3);
    runAnalysis(4);
    runAnalysis(6);
    runAnalysis(7);
    runAnalysis(8);
    runAnalysis(9);

    // generateGoals(1, 10, 8);


=======
    // std::cout << "main" << std::endl;
    runAnalysis();

>>>>>>> f3b237074d3a5d2d20e6bc36e1c3809893dfc6a8
    // std::ofstream outputFile;
    // outputFile.open("./test.txt");

    // runGame(5, "[ ][ ][ ][*]", outputFile);

    // outputFile.close();
    // testPlayerType();
<<<<<<< HEAD

    return 0;
=======
>>>>>>> f3b237074d3a5d2d20e6bc36e1c3809893dfc6a8
}

void testPlayerType(int numPlayers, std::string type)
{
<<<<<<< HEAD
    std::vector<std::string> playerTypes(numPlayers, type);
    Game *game = new Game(numPlayers, playerTypes, "[ ][ ][ ][*]");
=======
    Game *game = new Game(5, "[ ][ ][ ][*]");
>>>>>>> f3b237074d3a5d2d20e6bc36e1c3809893dfc6a8
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

void generateGoals(int minLength, int maxLength, int maxUnicorns)
{
    std::ofstream outputFile;
    outputFile.open("./GameInfo/LongGoals.txt");

    if (outputFile.is_open())
    {
        for(int length = minLength; length <= maxLength; length++)
        {
            generateGoals_Recursive(outputFile, "", length, maxUnicorns);
        }

        outputFile.close();
    }
}

void generateGoals_Recursive(std::ofstream &outputFile, std::string prefix, int k, int maxUnicorns)
{
    if (k == 0) // base case
    { 
        outputFile << (prefix) << std::endl; 
        return; 
    } 
    
    // One by one add all characters  
    // from set and recursively  
    // call for k equals to k-1 
    std::string newPrefix = prefix + "[ ]";
    generateGoals_Recursive(outputFile, newPrefix, k - 1, maxUnicorns); 

    if(maxUnicorns > 0)
    {
        std::string newPrefix = prefix + "[*]";
        generateGoals_Recursive(outputFile, newPrefix, k - 1, maxUnicorns - 1); 
    }
}

void runAnalysis(int numPlayers)
{
    std::ifstream goalsFile;
    goalsFile.open("./GameInfo/LongGoals.txt");

    std::ofstream outputFile;
<<<<<<< HEAD
    outputFile.open("./Analysis/Data/" + std::to_string(numPlayers) + "Players_AllPossibleGoals_AllAggressive.csv");
    std::ofstream handSizeOutputFile;
    handSizeOutputFile.open("./Analysis/Data/" + std::to_string(numPlayers) + "Players_AllPossibleGoals_AllAggressive_HandSize.csv");

    // std::cout << "Files Opened" << std::endl;
    std::vector<std::string> playerTypes (numPlayers, "aggressive");
    // std::vector<std::string> playerTypes { "aggressive", "aggressive", "aggressive", "aggressive", "aggressive",
                                        //    "aggressive", "aggressive", "aggressive", "aggressive", "aggressive" };              
=======
    outputFile.open("./GameInfo/AggressiveResults_fixed.csv");
>>>>>>> f3b237074d3a5d2d20e6bc36e1c3809893dfc6a8

    if (goalsFile.is_open() && outputFile.is_open() && handSizeOutputFile.is_open())
    {
        // std::cout << "inside if" << std::endl;
        std::string goal;
        int numGoals;

        /* read the goals */
        goalsFile >> numGoals;         // the number of goals to try
        std::getline(goalsFile, goal); // garbage read of new line

        outputFile << "Goal,Number of Players,Number of Rounds,End Result" << std::endl;
        handSizeOutputFile << "Goal,Number of Players,Player Number,Type,Winner,Hand Size" << std::endl;
        for (int i = 0; i < numGoals; i++)
        {
            // std::cout << i << std::endl;
            std::getline(goalsFile, goal);
            goal = goal.erase(goal.find_last_not_of("\t\n\v\f\r ") + 1);
            std::cout << goal << std::endl;

            // for (int numPlayers = 2; numPlayers <= 10; numPlayers++)
            // {
                for (int j = 0; j < 100; j++)
                {
                    std::cout << ".";
                    std::cout.flush();
                    runGame(numPlayers, playerTypes, goal, outputFile, handSizeOutputFile);
                }
            // }
            std::cout << std::endl;

        }

        // std::cout << "before close" << std::endl;
        goalsFile.close();
        outputFile.close();
        handSizeOutputFile.close();
        // std::cout << "after close" << std::endl;
    }

    return;
}

void runGame(int numPlayers, std::vector<std::string> playerTypes, 
             std::string goal, std::ofstream &outputFile, std::ofstream &handOutputFile)
{

    Game *game = new Game(numPlayers, playerTypes, goal);
    int roundCount = 0;
    bool win = false;
    int winningPlayer = -1;
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
        winningPlayer = game->getWinningPlayer();
        endResult = "Player " + std::to_string(winningPlayer) + " wins";

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
    catch (TooManyCardsException &e2)
    {
        // std::cout << e2.what();
        endResult = "Too many cards in hand";
    }

    outputFile << roundCount << ",";
    outputFile << endResult << std::endl;
    
    Player* currentPlayer;
    for(int i = 0; i < numPlayers; i++)
    {
        currentPlayer = game->getPlayer(i);
        handOutputFile << goal << ",";
        handOutputFile << numPlayers << ",";
        handOutputFile << "Player " << i + 1 << ",";
        handOutputFile << currentPlayer->getType() << ",";
        if (i == winningPlayer)
            handOutputFile << "True,";
        else
            handOutputFile << "False,";
        handOutputFile << currentPlayer->getHandSize() << std::endl;
    }
    LOG("There were " + std::to_string(roundCount) + " rounds. \n");

    // std::cout << "before delete" << std::endl;
    delete game;
    // std::cout << "after delete" << std::endl;
    return;
}