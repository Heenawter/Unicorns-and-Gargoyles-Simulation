#include "Debugging.h"
#include "Game.h"
#include "GameStates.h"

#include <fstream>

/*
    - loop through each goal
        - loop through 2-10 players
            - report statistics
*/


void classroomSimulation(std::discrete_distribution<> typeDistribution, std::vector<int> groupSizes,
                         std::vector<int> seeds, std::vector<std::string> goals, int classDuration_s, 
                         int timePerRound_s, std::string outputFileName);

void testTroll_taxCrabSpeed(std::vector<int> seeds, std::vector<std::string> goals);
void speedTest(int numPlayers,std::vector<int> seeds, std::vector<std::string> goals);
void degeneracyTest(std::string outputFileName, std::vector<int> seeds, std::vector<std::string> goals);
void degeneracyTest_constantPlayers(std::string outputFileName, std::vector<int> seeds, 
                                    std::vector<std::string> goals, std::vector<std::string> types);
void traceOutput(int numPlayers, std::string goal, std::vector<int> seeds);
void testGoals(std::vector<int> seeds, int numPlayers);

void testPlayerType(int numPlayers, std::string type);

void runAnalysis_files(std::vector<std::string> playerTypes, std::ifstream &goalsFile,
                       std::ofstream &outputFile, std::ofstream &handSizeOutputFile);
void runAnalysis(std::vector<std::string> playerTypes);
void runAnalysis_permutations(std::vector<std::string> playerTypes);

void runGame(int numPlayers, std::vector<std::string> playerTypes,
             std::string goal, std::ofstream &outputFile, std::ofstream &handOutputFile);
void runLimitedGame(int numPlayers, std::vector<std::string> playerTypes, int seed, std::string goal,
                    int* remainingTime_s, int timePerTurn_s, int* numTurns, std::string* endResult);
void runGameWithSeed(int numPlayers, std::vector<std::string> playerTypes, int seed,
                     std::string goal, std::ofstream &outputFile);

std::vector<std::string> generateRandomPlayers(int numPlayers, int seed);
std::vector<int> generateRandomSeeds(int numSeeds);
std::vector<int> readSeedsFromFile(std::string fileName);
std::vector<std::string> readGoals(std::string fileName);

void generateGoals_Recursive(std::ofstream &outputFile, std::string prefix, int k, int maxUnicorns);
void generateGoals(std::string fileName, int minLength, int maxLength, int maxUnicorns);


int main()
{
    #ifdef _DEBUGSTATEMENTS
        std::ofstream out("./Analysis/Data/FixedSeed_FullTrace.txt");
        std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
        std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    #endif

    #ifdef _TESTSTATEMENTS
        std::ofstream out("./Analysis/Data/FixedSeed_Results.txt");
        std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
        std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    #endif

    // for(int b = 3; b <= 5; b++)
    // {
    //     for(int m = 0; m <= 3; m++) {
    //         generateGoals("./GameInfo/" + std::to_string(b) + "bits_" + std::to_string(m) + "mermaids.txt", 1, b, m);
    //     }
    // }
    // generateGoals("./GameInfo/7bits_3mermaids.txt", 2, 7, 3);

    // std::vector<int> seeds {0, 17, 500, 1234, 2020, 30123};
    // std::vector<int> seeds {8545, 9629, 32321, 14651, 6857, 23324, 21986, 12866, 30523, 6517};
    // std::vector<int> seeds = readSeedsFromFile("./Analysis/Data/allSeeds.txt");

    // std::vector<std::string> allGoals = readGoals("./GameInfo/10bits_8mermaids.txt");
    // std::vector<int> seeds = generateRandomSeeds(100000);
    // testTroll_taxCrabSpeed(seeds, allGoals); 

    // int b = 5;
    // for(int b = 3; b <= 6; b++)
    // {
    //     std::cout << "B: " << b << std::endl;
    //     for(int m = 1; m <= 3; m++) {
    //         std::cout << "- M: " << m << std::endl;
    //         std::vector<std::string> allGoals = readGoals("./GameInfo/" + std::to_string(b) + "bits_" + std::to_string(m) + "mermaids.txt");
    //         degeneracyTest("./Analysis/Data/degeneracyTest_results_" + std::to_string(b) + "bits_" + std::to_string(m) + "mermaids.csv", seeds, allGoals);
    //     }
    // }

    // std::vector<std::string> types { "aggressive", "greedy", "troll" };
    // std::vector<std::string> minMax {"[ ]", "[*]", "[ ][ ][ ][ ][ ][ ]", "[ ][ ][ ][ ][*][*]",
    //                                 "[ ][ ][ ][*][ ][*]", "[ ][ ][ ][*][*][ ]", "[ ][ ][*][ ][ ][*]", 
    //                                 "[ ][ ][*][ ][*][ ]", "[ ][ ][*][*][ ][ ]", "[ ][*][ ][ ][ ][*]",
    //                                 "[ ][*][ ][ ][*][ ]", "[ ][*][ ][*][ ][ ]", "[ ][*][*][ ][ ][ ]", 
    //                                 "[*][ ][ ][ ][ ][*]", "[*][ ][ ][ ][*][ ]", "[*][ ][ ][*][ ][ ]",  
    //                                 "[*][ ][*][ ][ ][ ]", "[*][*][ ][ ][ ][ ]"};
    // degeneracyTest_constantPlayers("./Analysis/Data/degeneracyTest_minMax_results.csv", seeds, minMax, types);


    // std::vector<std::string> allGoals = readGoals("./GameInfo/6bits_0mermaids.txt");
    // degeneracyTest("./Analysis/Data/degeneracyTest_results_6bits_0mermaids.csv", seeds, allGoals);

    // traceOutput(5, "[ ][ ][*][ ][ ]", seeds);

    // std::vector<int> seeds = generateRandomSeeds(100);
    // for(int i = 2; i <= 6; i++ )
    // {
    //     auto start = std::chrono::high_resolution_clock::now();
        
    //     testGoals(seeds, i);

    //     auto finish = std::chrono::high_resolution_clock::now();
    //     auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(finish - start).count();
    //     std::cout << "------> Elapsed time for " << i << " players: " << elapsed << " mins" << std::endl;
    // }


    std::vector<std::string> allGoals = readGoals("./GameInfo/6bits_2mermaids.txt");

    auto start = std::chrono::high_resolution_clock::now();

    // std::vector<std::string> types {  "aggressive", "greedy", "troll"  };
    // std::vector<int> seeds = generateRandomSeeds(100);
    // std::vector<std::string> allGoals = readGoals("./GameInfo/10bits_8mermaids.txt");
    // degeneracyTest_constantPlayers("./Analysis/Data/playerTypeTest_whoWins_allGoals_pt2.csv", seeds, 
    //                                 allGoals, types);

    // distrubution of aggressive, greedy, troll respectively;
    int numSeeds = 10000;
    std::vector<int> seeds = generateRandomSeeds(numSeeds);

    // std::vector<int> groupSizes { 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2  };
    // std::discrete_distribution<> middleSchoolDistr({ 40, 50, 10 });
    // int middleSchoolClassDuration = 40 * 60; // 50 minutes * 60 seconds = 3000 seconds
    // int middleSchoolTurnDuration = 40;    // approximately 30 seconds per turn
    
    // classroomSimulation(middleSchoolDistr, groupSizes, seeds, allGoals, middleSchoolClassDuration, 
    //                     middleSchoolTurnDuration,
    //                     "./Analysis/Data/MiddleSchoolClassroomResults_322222222222_shorterClass.csv");

    
    // divide students in to groups
    // int numStudents = 150;
    // int numGroups = (numStudents / 6);
    // if(numStudents % 6 != 0) {
    //     numGroups++;
    // }
    // std::vector<int> groupSizes(numGroups, 0);
    // int remainingStudents = numStudents;
    // while (remainingStudents > 0) {
    //     for(int i = 0; i < numGroups && remainingStudents > 0; i++)
    //     {
    //         groupSizes[i]++;
    //         remainingStudents--;
    //     }
    // }

    std::vector<int> groupSizes { 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
    // std::discrete_distribution<> uniDistr({ 60, 30, 10 });
    std::discrete_distribution<> uniDistr({ 100, 0, 0 });
    int uniClassDuration = 50 * 60; // 75 minutes * 60 seconds
    int uniTurnDuration = 40;    // approximately 30 seconds per turn
    classroomSimulation(uniDistr, groupSizes, seeds, allGoals, uniClassDuration, 
                        uniTurnDuration, "./Analysis/Data/UniversityClassroomResults_allFoxes_tutorial_3x1_2x11.csv");
    
    std::vector<int> groupSizes2 { 5, 5, 5, 5, 5 };
    classroomSimulation(uniDistr, groupSizes2, seeds, allGoals, uniClassDuration, 
                        uniTurnDuration, "./Analysis/Data/UniversityClassroomResults_allFoxes_tutorial_5x5.csv");
    
    
    // std::vector<int> groupSizes_small(50, 3);
    // classroomSimulation(uniDistr, groupSizes_small, seeds, allGoals, uniClassDuration, 
    //                     uniTurnDuration, "./Analysis/Data/UniversityClassroomResults_50x3_40s.csv");


    auto finish = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(finish - start).count();
    std::cout << "------> Elapsed time: " << elapsed << " mins" << std::endl;

    #ifdef _DEBUGSTATEMENTS
        std::cout.rdbuf(coutbuf); //reset to standard output again
    #endif

    #ifdef _TESTSTATEMENTS
        std::cout.rdbuf(coutbuf); //reset to standard output again
    #endif

    // speedTest(5);

    // testPlayerType(4, "aggressive"); 
    
    // // std::vector<std::string> playerTypes (numPlayers, "aggressive");
    // std::vector<std::string> playerTypes { "aggressive", "greedy", "troll" };
    // auto start = std::chrono::high_resolution_clock::now();
    // runAnalysis(playerTypes);
    // // runAnalysis_permutations(playerTypes);       
    // auto finish = std::chrono::high_resolution_clock::now();
    // auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(finish - start).count();
    // std::cout << "------> Elapsed time: " << elapsed << " mins" << std::endl;

    return 0;
}

void classroomSimulation(std::discrete_distribution<> typeDistribution, std::vector<int> groupSizes, std::vector<int> seeds,
    std::vector<std::string> goals, int classDuration_s, int timePerTurn_s, std::string outputFileName)
{ 
    std::vector<std::string> types { "aggressive", "greedy", "troll" };
    std::vector<std::string> playerTypes;

    int numGroups = groupSizes.size();
    
    // play the game
    std::ofstream outputFile;
    outputFile.open(outputFileName);
    int numPlayers = groupSizes[0], numTurns = 0, remainingTime_s = classDuration_s;
    std::string playerTypeString, turnString, classTimeString, endResultsString;
    
    // int constantSeed = std::chrono::system_clock::now().time_since_epoch().count();

    std::uniform_int_distribution<> goalDistribution(0, goals.size() - 1);
    std::string type, goal, endResult;
    int session = 1, gameNumber = 0;
    if (outputFile.is_open())
    {   
        outputFile << "Session,Seed,Group,Player Types,Number of Games,End Results," 
                   << "Number of Turns,Remaining Class Times" << std::endl;

        for(auto seed : seeds)
        {
            std::mt19937 randomGenerator(seed);

            // for each group...
            for(int group = 0; group < numGroups; group++)
            {
                // loop through players and assign type using probablities
                numPlayers = groupSizes[group];
                playerTypeString = "";
                for(int n = 0; n < numPlayers; n++)
                {
                    type = types[typeDistribution(randomGenerator)];
                    playerTypes.push_back(type);
                    playerTypeString += type + ";";
                }
                playerTypeString.replace(playerTypeString.end()-1,playerTypeString.end(),"");    
            
                outputFile << session << "," << seed << "," << group << "," << playerTypeString << ",";

                remainingTime_s = classDuration_s;
                gameNumber = 0;
                turnString = "";
                classTimeString = "";
                endResultsString = "";
                int setupTime = 120, cleanupTime = 60;

                while(remainingTime_s > setupTime + cleanupTime) {
                    gameNumber++;
                    remainingTime_s -= setupTime; // time of setup

                    goal = goals[goalDistribution(randomGenerator)];

                    runLimitedGame(numPlayers, playerTypes, seed, goal, &remainingTime_s,
                                   timePerTurn_s, &numTurns, &endResult);

                    remainingTime_s -= cleanupTime; // time of cleanup 
                    
                    // std::cout << totalTurns << std::endl;
                    turnString += std::to_string(numTurns) + ";";
                    classTimeString += std::to_string(remainingTime_s) + ";";
                    endResultsString += endResult + ";";
                }
                turnString.replace(turnString.end()-1,turnString.end(),"");  
                classTimeString.replace(classTimeString.end()-1,classTimeString.end(),"");  
                endResultsString.replace(endResultsString.end()-1,endResultsString.end(),"");  

                outputFile << gameNumber << "," << endResultsString << "," 
                           << turnString<< "," << classTimeString << std::endl;
                playerTypes.clear();  
            }
            session++;
        }
    }

    outputFile.close();
}

   
void traceOutput(int numPlayers, std::string goal, std::vector<int> seeds)
{
    Player* current;
    Game* game;
    int numCards = 10;
    std::vector<std::string> playerTypes { "aggressive", "greedy", "troll" };
    // std::vector<std::string> playerTypes { "troll" };
    std::map<int, std::string> methods{
        {0, "Try Swapping"},
        {1, "Tax Crab"},
        {2, "Spring Cleaning"},
        {3, "Fish Hook"},
        {4, "Poison"}
    };

    std::string type;
    std::string method;
    int seed, m;

    for (auto entry : methods)
    {
        m = entry.first;
        method = entry.second;
        TEST("- " + method + "\n");

        for(int s = 0; s < seeds.size(); s++)
        {
            seed = seeds[s];  
            TEST("\t- Seed " + std::to_string(seed) + "\n");

            for(int i = 0; i < playerTypes.size(); i++)
            {                    
                type = playerTypes[i];
                std::vector<std::string> playerTypes(numPlayers, type);
                game = new Game(numPlayers, playerTypes, goal, seed);

                for(int j = 0; j < numCards; j++)
                {
                    for(int k = 0; k < numPlayers; k++)
                    {
                        current = game->getPlayer(k);
                        current->action_drawNonActionCard();
                    }
                }

                TEST("\t\t- " + type + ": \n");
                DEBUG("\n---------- TEST FUNCTION: " + method + " ---------\n");
                DEBUG(" - Seed: " + std::to_string(seed) + "\n");
                DEBUG(" - Type: " + type + "\n");
                DEBUG("--------------------------------------------------\n");

                for(int k = 0; k < numPlayers; k++)
                {
                    current = game->getPlayer(k);
                    DEBUG("--- Player " + std::to_string(k) + " ---\n");
                    TEST("\t\t\t- Player " + std::to_string(k) + ": ");
                    switch(m) {
                        case 0:
                            current->trySwapping();
                            break;
                        case 1:
                            current->action_discardCard();
                            break;
                        case 2:
                            current->action_springCleaning();
                            break;
                        case 3:
                            current->action_stealCard();
                            break;
                        case 4:
                            current->action_poisonUnicorn();
                            break;
                        default:
                            break;
                    }
                    DEBUG("\n\n");
                    TEST("\n");
                }

                DEBUG("--------------------------------------------------\n");

                delete game;
            }
        }
    } 
}

void testGoals(std::vector<int> seeds, int numPlayers)
{
    std::ofstream outputFile;
    outputFile.open("./Analysis/Data/goalAnalysis_results_after_balancing_" 
                    + std::to_string(numPlayers) +  "players.csv");
    std::vector<std::string> playerTypes;
    std::vector<std::string> goals = readGoals("./GameInfo/6bits_2mermaids.txt");
    std::string playerTypesString;
    int i = 0;

    if (outputFile.is_open())
    {
        outputFile << "Seed,Goal,Number of Players,Player Types,Number of Rounds,End Result" << std::endl;
        for(auto seed: seeds)
        {
            i = 0;
            for(auto goal: goals)
            {
                playerTypes = generateRandomPlayers(numPlayers, seed + i);
                playerTypesString = "[";
                for(auto type: playerTypes)
                {
                    playerTypesString += type + ";";
                }
                playerTypesString.replace(playerTypesString.end()-1,playerTypesString.end(),"]");

                outputFile << seed << ",";
                outputFile << goal << ",";
                outputFile << numPlayers << ",";
                outputFile << playerTypesString << ",";
                runGameWithSeed(numPlayers, playerTypes, seed, goal, outputFile);
                outputFile << std::endl;
                i++;
            }
        }
    }
}

void testTroll_taxCrabSpeed(std::vector<int> seeds, std::vector<std::string> goals) 
{
    Player* current;
    Game* game;
    int numGames = 1;
    int numCards = 10;
    int numPlayers = 5;
    std::string type;
    std::string goal;
    int m, seed;

    std::ofstream outputFile;
    outputFile.open("./Analysis/Data/speedTest_troll_taxCrab_results_unoptimized.csv");
    if (outputFile.is_open())
    {
        outputFile << "Seed,Goal,Time" << std::endl;

        for(int s = 0; s < seeds.size(); s++)
        {
            seed = seeds[s];
            goal = goals[seed % 2034];
            std::cout << goal << std::endl;
            
            for(int n = 0; n < numGames; n++)
            {            
                std::vector<std::string> playerTypes(numPlayers, "greedy");
                game = new Game(numPlayers, playerTypes, goal, seed);

                outputFile << seed << "," << goal << ",";
                for(int j = 0; j < numCards; j++)
                {
                    for(int k = 0; k < numPlayers; k++)
                    {
                        current = game->getPlayer(k);
                        current->action_drawNonActionCard();
                        /* uncomment below if optimized, comment out if unoptimized */
                        // Hand bestSwap = current->trySwapping();
                        // if(bestSwap.getDistance() < current->getDistance())
                        // {
                        //     current->setHand(bestSwap);
                        // } 
                    }
                }

                auto start = std::chrono::high_resolution_clock::now();
                for(int k = 0; k < numPlayers; k++)
                {
                    current = game->getPlayer(k);
                    current->action_discardCard();
                }
                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
                outputFile << elapsed << std::endl;

                delete game;
            }
            
        }
        
        outputFile.close();
    }
}

void degeneracyTest_constantPlayers(std::string outputFileName, std::vector<int> seeds, 
                                    std::vector<std::string> goals, std::vector<std::string> types) 
{
    std::ofstream outputFile;
    outputFile.open(outputFileName);
    int seed, maxPlayers = 12;
    std::string type;
    std::string goal;
    int n = types.size();

    std::uniform_int_distribution<> typeDistribution(0, 2);
    std::uniform_int_distribution<> goalsDistribution(0, goals.size() - 1);
    std::mt19937 randomGenerator;
    if (outputFile.is_open())
    {
        outputFile << "Seed,Goal,Number of Players,Player Types,Number of Rounds,End Result,Game Time" << std::endl;

        for(int s = 0; s < seeds.size(); s++)
        {
            seed = seeds[s];
            randomGenerator = std::mt19937(seed);

            for(auto goal : goals) {
                std::shuffle(std::begin(types), std::end(types), randomGenerator);
                std::string playerTypesString = "[";
                for(int i = 0; i < n; i++)
                {
                    type = types[i];
                    playerTypesString += type + ";";
                }
                playerTypesString.replace(playerTypesString.end()-1,playerTypesString.end(),"]");    
                
                outputFile << seed << ",";
                outputFile << goal << ",";
                outputFile << n << ",";
                outputFile << playerTypesString << ",";

                auto start = std::chrono::high_resolution_clock::now();
                runGameWithSeed(n, types, seed, goal, outputFile);
                auto finish = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

                outputFile << "," << elapsed << std::endl;
            }
        }
        
        outputFile.close();
    }
}

void degeneracyTest(std::string outputFileName, std::vector<int> seeds, std::vector<std::string> goals) 
{
    std::ofstream outputFile;
    outputFile.open(outputFileName);
    int seed, maxPlayers = 12;
    std::string type;
    std::string goal;
    std::vector<std::string> playerTypes;
    std::vector<std::string> types { "aggressive", "greedy", "troll" };
    
    std::uniform_int_distribution<> typeDistribution(0, 2);
    std::uniform_int_distribution<> goalsDistribution(0, goals.size() - 1);
    std::mt19937 randomGenerator;
    if (outputFile.is_open())
    {
        outputFile << "Seed,Goal,Number of Players,Player Types,Number of Rounds,End Result" << std::endl;

        for(int s = 0; s < seeds.size(); s++)
        {
            seed = seeds[s];
            randomGenerator = std::mt19937(seed);
            goal = goals[goalsDistribution(randomGenerator)];
            for(int n = 1; n <= maxPlayers; n++)
            {            
                playerTypes.clear();
                std::string playerTypesString = "[";
                for(int i = 0; i < n; i++)
                {
                    type = types[typeDistribution(randomGenerator)];
                    playerTypes.push_back(type);
                    playerTypesString += type + ";";
                }
                playerTypesString.replace(playerTypesString.end()-1,playerTypesString.end(),"]");    
                
                outputFile << seed << ",";
                outputFile << goal << ",";
                outputFile << n << ",";
                outputFile << playerTypesString << ",";
                runGameWithSeed(n, playerTypes, seed, goal, outputFile);
                outputFile << std::endl; 
            }
        }
        
        outputFile.close();
    }
}

void speedTest(int numPlayers, std::vector<int> seeds, std::vector<std::string> goals) 
{
    Player* current;
    Game* game;
    int numGames = 1;
    int numSeeds = 100000;
    int numCards = 10;
    std::vector<std::string> playerTypes { "aggressive", "greedy", "troll" };
    std::map<int, std::string> methods{
        {'t', "Try Swapping"},
        {'a', "Tax Crab"},
        {'s', "Spring Cleaning"},
        {'f', "Fish Hook"},
        {'p', "Poison"}
    };

    std::string type;
    std::string goal;
    std::string method;
    int m, seed;

    std::ofstream outputFile;
    outputFile.open("./Analysis/Data/speedTest_results.csv");
    if (outputFile.is_open())
    {
        outputFile << "Seed,Goal,Player Type,Function,Time" << std::endl;

        for(int s = 0; s < numSeeds; s++)
        {
            seed = seeds[s];
            goal = goals[seed % 2034];
            std::cout << goal << std::endl;
            
            for (auto entry : methods)
            {           
                m = entry.first;
                method = entry.second;
                for(int i = 0; i < playerTypes.size(); i++)
                {                    
                    for(int n = 0; n < numGames; n++)
                    {            
                        type = playerTypes[i];
                        std::vector<std::string> playerTypes(numPlayers, type);
                        game = new Game(numPlayers, playerTypes, goal, seed);

                        outputFile << seed << "," << goal << "," << type << "," << method << ",";;
                        for(int j = 0; j < numCards; j++)
                        {
                            for(int k = 0; k < numPlayers; k++)
                            {
                                current = game->getPlayer(k);
                                current->action_drawNonActionCard();
                            }
                        }

                        auto start = std::chrono::high_resolution_clock::now();
                        for(int k = 0; k < numPlayers; k++)
                        {
                            current = game->getPlayer(k);
                            switch(m) {
                                case 't':
                                    current->trySwapping();
                                    break;
                                case 'a':
                                    current->action_discardCard();
                                    break;
                                case 's':
                                    current->action_springCleaning();
                                    break;
                                case 'f':
                                    current->action_stealCard();
                                    break;
                                case 'p':
                                    current->action_poisonUnicorn();
                                    break;
                                default:
                                    break;
                            }
                        }
                        auto finish = std::chrono::high_resolution_clock::now();
                        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
                        outputFile << elapsed << std::endl;

                        delete game;
                    }
                }
            }

        }
        
        outputFile.close();
    }
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

    for(int i = 0; i < 5; i++)
    {
        for (int i = 0; i < numPlayers; i++)
        {
            current = game->getPlayer(i);
            current->takeTurn();
        }
    }    

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
            for (int j = 0; j < 1; j++)
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

void runLimitedGame(int numPlayers, std::vector<std::string> playerTypes, int seed,
                     std::string goal, int* remainingTime_s, int timePerTurn_s,
                     int* numTurns, std::string* endResult)
{

    Game *game = new Game(numPlayers, playerTypes, goal, seed);
    // game->printDeck();
    bool win = false;
    int winningPlayer = -1;

    try
    {
        while (!win && *remainingTime_s >= timePerTurn_s)
        {
            win = game->gameRound_limitedTime(remainingTime_s, timePerTurn_s);
        }

        if(win)
        {
            winningPlayer = game->getWinningPlayer();
            *endResult = "Player " + std::to_string(winningPlayer) + " wins";
        } else {
            *endResult = "Ran out of time on player " + std::to_string(game->getCurrentPlayer()->getPlayerNum()) + "'s turn";
        }
    }
    catch (RanOutOfCardsException &e1)
    {
        // std::cout << e1.what();
        *endResult = "Ran out of cards";
    }
    catch (OnlyActionCardsException &e2)
    {
        // std::cout << e2.what();
        // std::cout << "---> Only action cards left: " << game->getTurnCount() << " turns, seed = " << seed << std::endl;
        *endResult = "Only action cards left";
    }
    catch (TooManyCardsException &e2)
    {
        // std::cout << e2.what();
        *endResult = "Too many cards in hand";
    }
    #ifdef _ANTIBUGSTATEMENTS
        catch (InvalidGameException &e2)
        {
            // std::cout << e2.what();
            *endResult = "More or less than 72 cards in game.";
        }
    #endif
    
    *numTurns = game->getTurnCount();
    LOG("There were " + std::to_string(roundCount) + " rounds. \n");

    delete game;
}


void runGameWithSeed(int numPlayers, std::vector<std::string> playerTypes, int seed,
                     std::string goal, std::ofstream &outputFile)
{

    Game *game = new Game(numPlayers, playerTypes, goal, seed);
    // game->printDeck();
    int roundCount = 0;
    bool win = false;
    int winningPlayer = -1;
    std::string endResult = "";

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
    #ifdef _ANTIBUGSTATEMENTS
        catch (InvalidGameException &e2)
        {
            // std::cout << e2.what();
            endResult = "More or less than 72 cards in game.";
        }
    #endif

    outputFile << roundCount << ",";
    outputFile << endResult;
    
    LOG("There were " + std::to_string(roundCount) + " rounds. \n");

    delete game;
    return;
}

std::vector<std::string> generateRandomPlayers(int numPlayers, int seed) 
{
    std::vector<std::string> playerTypes;
    std::vector<std::string> types { "aggressive", "greedy", "troll" };
    std::uniform_int_distribution<> typeDistribution(0, 2);
    std::mt19937 randomGenerator(seed);

    std::string type;
    for(int i = 0; i < numPlayers; i++)
    {
        type = types[typeDistribution(randomGenerator)];
        playerTypes.push_back(type);
    }

    return playerTypes;
}

void generateGoals(std::string filename, int minLength, int maxLength, int maxUnicorns)
{
    std::ofstream outputFile;
    outputFile.open(filename);

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


std::vector<int> generateRandomSeeds(int numSeeds)
{
    std::vector<int> seeds;
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 randomGenerator = std::mt19937(seed);
    std::uniform_int_distribution<> distribution(0, MAX_INT);
    for(int i = 0; i < numSeeds; i++)
    {
        seed = distribution(randomGenerator);
        if(std::find(seeds.begin(), seeds.end(), seed) == seeds.end()) // not found
            seeds.push_back(distribution(randomGenerator));
        else     // seed already exists
            i--; // so generate new seed
    }
    return seeds;
}

std::vector<int> readSeedsFromFile(std::string fileName)
{
    std::vector<int> seeds;
    std::ifstream allSeeds;
    allSeeds.open(fileName);
    std::string seedLine;
    int seed;
    while(std::getline(allSeeds, seedLine))
    {
        seed = std::stoi(seedLine);
        seeds.push_back(seed);
    }
    return seeds;
}

std::vector<std::string> readGoals(std::string fileName)
{
    std::ifstream goalsFile;
    goalsFile.open(fileName);
    std::vector<std::string> allGoals;
    std::string goal;
    int numGoals;
    if (goalsFile.is_open())
    {
        std::getline(goalsFile, goal);
        numGoals = std::stoi(goal);
        for (auto i = 0; i < numGoals; ++i) {
            std::getline(goalsFile, goal);
            goal = goal.erase(goal.find_last_not_of(" \n\r\t\f\v") + 1);
            allGoals.push_back(goal);
        }
        goalsFile.close();
    }
    return allGoals;
}