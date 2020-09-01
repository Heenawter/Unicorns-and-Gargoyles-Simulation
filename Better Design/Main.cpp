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

void runAnalysis_files(std::vector<std::string> playerTypes, std::ifstream &goalsFile,
                       std::ofstream &outputFile, std::ofstream &handSizeOutputFile);
void runAnalysis(std::vector<std::string> playerTypes);
void runAnalysis_permutations(std::vector<std::string> playerTypes);

void runGame(int numPlayers, std::vector<std::string> playerTypes,
             std::string goal, std::ofstream &outputFile, std::ofstream &handOutputFile);
void generateGoals_Recursive(std::ofstream &outputFile, std::string prefix, int k, int maxUnicorns);
void generateGoals(int minLength, int maxLength, int maxUnicorns);


int main()
{

    testPlayerType(4, "aggressive"); 

    // if (__cplusplus == 201703L) std::cout << "C++17\n";
    // else if (__cplusplus == 201402L) std::cout << "C++14\n";
    // else if (__cplusplus == 201103L) std::cout << "C++11\n";
    // else if (__cplusplus == 199711L) std::cout << "C++98\n";
    // else std::cout << "pre-standard C++\n";


    
    // // std::vector<std::string> playerTypes (numPlayers, "aggressive");
    // std::vector<std::string> playerTypes { "aggressive", "greedy", "troll" };
    // auto start = std::chrono::high_resolution_clock::now();
    // // runAnalysis(playerTypes);
    // runAnalysis_permutations(playerTypes);       
    // auto finish = std::chrono::high_resolution_clock::now();
    // // // std::chrono::duration<double> elapsed = finish - start;

    // auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(finish - start).count();
    // std::cout << "------> Elapsed time: " << elapsed << " mins" << std::endl;

    // runAnalysis_permutations(playerTypes);

    // std::srand ( unsigned ( std::time(0) ) );
    // std::cout << std::rand() << std::endl;


    // std::cout << "main" << std::endl;
    // runAnalysis(2);
    // runAnalysis(4);
    // runAnalysis(3);
    // runAnalysis(5);
    // runAnalysis(6);
    // runAnalysis(7);
    // runAnalysis(8);
    // runAnalysis(9);
    // runAnalysis(10);

    // generateGoals(1, 10, 8);


    // std::ofstream outputFile;
    // outputFile.open("./test.txt");

    // runGame(5, "[ ][ ][ ][*]", outputFile);

    // outputFile.close();
    // testPlayerType();

    return 0;
}


void testPlayerType(int numPlayers, std::string type)
{
    std::vector<std::string> playerTypes(numPlayers, type);
    Game *game = new Game(numPlayers, playerTypes, "[ ][ ][ ][*]");
    Player* current;

    int numRounds = 5;
    for(int i = 0; i < numRounds; i++)
    {
        for(int j = 0; j < numPlayers; j++)
        {
            // std::cout << "I'm here" << std::endl;1
            current = game->getPlayer(j);
            current->action_drawNonActionCard();
        }
    }

    std::cout << std::endl;
    game->printDeck();
    std::cout << std::endl << std::endl;
    for (int i = 0; i < numPlayers; i++)
    {
        current = game->getPlayer(i);
        std::cout << "Player " << i << " Before: " << current->toString() << std::endl;
    }
    
    // current->action_springCleaning_helper();
    // current->takeTurn();
    current = game->getPlayer(0);
    current->action_discardCard_helper();

    for (int i = 0; i < numPlayers; i++)
    {
        current = game->getPlayer(i);
        std::cout << "Player " << i << " After:  " << current->toString() << std::endl;
    }

    std::cout << std::endl;
    game->printDeck();

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

void runAnalysis_permutations(std::vector<std::string> playerTypes)
{
    std::ifstream goalsFile;
    goalsFile.open("./GameInfo/LongGoals.txt");

    std::ofstream outputFile;
    outputFile.open("./Analysis/Data/permutationsOf3Players_AllPossibleGoals_1ofEachType_10runs.csv");
    std::ofstream handSizeOutputFile;
    handSizeOutputFile.open("./Analysis/Data/permutationsOf3Players_AllPossibleGoals_1ofEachType_10runs_HandSize.csv");

    if (goalsFile.is_open() && outputFile.is_open() && handSizeOutputFile.is_open())
    {
        outputFile << "Goal,Number of Players,Player Types,Number of Rounds,End Result" << std::endl;
        handSizeOutputFile << "Goal,Number of Players,Player Number,Type,Winner,Hand Size" << std::endl;

        do {

                std::cout << "!! new permutation of players ---> ";
                for (auto type: playerTypes) { 
                    std::cout << type << ", ";
                }
                std::cout << std::endl; 

                runAnalysis_files(playerTypes, goalsFile, outputFile, handSizeOutputFile);
                
                // goalsFile.clear();
                goalsFile.seekg(0, goalsFile.beg);
            
        } while (std::next_permutation(playerTypes.begin(), playerTypes.end()));

        goalsFile.close();
        outputFile.close();
        handSizeOutputFile.close();
    }
}

void runAnalysis_files(std::vector<std::string> playerTypes, std::ifstream &goalsFile,
                       std::ofstream &outputFile, std::ofstream &handSizeOutputFile)
{
    // std::cout << "inside if" << std::endl;
    int numPlayers = playerTypes.size();

    std::string goal;
    int numGoals;

    /* read the goals */
    goalsFile >> numGoals;         // the number of goals to try
    std::getline(goalsFile, goal); // garbage read of new line

    for (int i = 0; i < numGoals; i++)
    {
        // std::cout << i << std::endl;
        std::getline(goalsFile, goal);
        goal = goal.erase(goal.find_last_not_of("\t\n\v\f\r ") + 1);
        std::cout << goal << std::endl;

        // for (int numPlayers = 2; numPlayers <= 10; numPlayers++)
        // {
            for (int j = 0; j < 10; j++)
            {
                std::cout << ".";
                std::cout.flush();
                runGame(numPlayers, playerTypes, goal, outputFile, handSizeOutputFile);
            }
        // }
        std::cout << std::endl;

    }
}

void runAnalysis(std::vector<std::string> playerTypes)
{
    int numPlayers = playerTypes.size();
    std::ifstream goalsFile;
    goalsFile.open("./GameInfo/LongGoals.txt");

    std::ofstream outputFile;
    outputFile.open("./Analysis/Data/FIXED_" + std::to_string(numPlayers) + "Players_AllPossibleGoals_1time.csv");
    std::ofstream handSizeOutputFile;
    handSizeOutputFile.open("./Analysis/Data/FIXED_" + std::to_string(numPlayers) + "Players_AllPossibleGoals_HandSize_1time.csv");

    // std::cout << "Files Opened" << std::endl;           

    if (goalsFile.is_open() && outputFile.is_open() && handSizeOutputFile.is_open())
    {
        outputFile << "Goal,Number of Players,Player Types,Number of Rounds,End Result" << std::endl;
        handSizeOutputFile << "Goal,Number of Players,Player Number,Type,Winner,Hand Size" << std::endl;

        // std::cout << "inside if" << std::endl;
        runAnalysis_files(playerTypes, goalsFile, outputFile, handSizeOutputFile);

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

    std::string playerTypesString = "[";
    for (auto type: playerTypes) { 
        playerTypesString += type + ";";
    }
    playerTypesString.replace(playerTypesString.end()-1,playerTypesString.end(),"]");    

    outputFile << goal << ",";
    outputFile << numPlayers << ",";
    outputFile << playerTypesString << ",";
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