import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math

PLAYER_TYPE = "Troll"

def numRounds_vs_numPlayers(df):
    onlyWins = df[df["End Result"].str.contains("win")]
    onlyWins = onlyWins[["Number of Rounds", "Number of Players"]]

    sns.set_style("darkgrid")
    fig, ax = plt.subplots(figsize=(16, 8))
    ax = sns.boxplot(x="Number of Rounds", y="Number of Players", data=onlyWins, orient="h")

    plt.title("Number of Rounds VS Number of Players (for Games with a Winner)")
    plt.tight_layout()
    plt.savefig("./Plots/WinningGames_NumRoundsVSNumPlayers_All" + PLAYER_TYPE + "_fixed")
    plt.show()

def plotHist(df):
    sns.set_style("darkgrid")
    maxRounds = max(df["Number of Rounds"])
    rounded = int(math.ceil(maxRounds / 10.0)) * 10
    lastTwo = rounded % 100
    firstDigit = rounded / 100

    print(maxRounds, rounded, lastTwo, firstDigit)
    if(lastTwo != 0):
        customRange = chain(range(0, lastTwo, 10), range(lastTwo, rounded + 1, 100)) 
    else: 
        customRange = chain(range(0, lastTwo + 100, 10), range(lastTwo + 100, rounded + 1, 100)) 
    # customRange = range(0, rounded, 10);
    customBins = []
    for i in customRange:
        customBins.append(i);  

    df["Bin"] = pd.cut(df["Number of Rounds"], bins=customBins)
    df = df[["Bin", "Goal"]].groupby('Bin').count().reset_index()
    df.rename(columns={"Bin": "Number of Rounds", "Goal": "Number of Games"}, inplace=True)
    print(df)

    fig, ax = plt.subplots(figsize=(16, 8))
    ax = sns.barplot(x="Number of Rounds", y="Number of Games", data=df);
  
    plt.title("How Often Do Games Usually Last?")
    plt.tight_layout()
    plt.savefig("./Plots/GameLengths_All" + PLAYER_TYPE + "_fixed")
    plt.show()
    

def plotBar_meanRoundsPerGoal(df):
    meanRoundsPerGoal = df[["Goal", "Number of Rounds"]].groupby("Goal").mean().reset_index()

    meanRoundsPerGoal["Card Count"] = meanRoundsPerGoal["Goal"].str.len() / 3
    meanRoundsPerGoal["Unicorn Count"] = meanRoundsPerGoal.Goal.str.count('\*')
    meanRoundsPerGoal.sort_values(
        ["Card Count", "Unicorn Count"], inplace=True, ascending=False)

    def getColour(num):
        colourDict = {0: "#2ecc71", 1: "#e74c3c", 2: "#34495e"}
        return colourDict[num]
    
    colours = meanRoundsPerGoal.apply(lambda x: getColour(x["Unicorn Count"]), axis=1)

    fig = meanRoundsPerGoal.plot(x="Goal", y="Number of Rounds", kind="barh", color=colours)
    fig.set_xlabel("Mean Number of Rounds")
    plt.show()

def countResults(df):
    test = df[["End Result", "Number of Players", "Goal"]].groupby(["End Result", "Number of Players"]).count().reset_index()

    def formatResults(value):
        if(value[0] == 'P'):
            number = int(value[7]) + 1
            return "Player " + str(number)
        else:
            return "No Winner"

    sns.set_style("darkgrid")
    fig, ax = plt.subplots(nrows=3, ncols=3, figsize=(10, 10))

    test["End Result"] = test.apply(lambda x: formatResults(x["End Result"]), axis=1)

    i = 2
    for row in ax:
        for col in row:
            current = test[test["Number of Players"] == i]
            col.pie(current["Goal"], labels=current["End Result"])
            col.set_title("Who Wins? (" + str(i) + " Players)")
            i += 1


    fig.subplots_adjust(wspace=0.5, hspace=0.3)
    plt.tight_layout()
    plt.savefig("./Plots/WhoWins_" + PLAYER_TYPE + "_fixed")
    plt.show()
    
    # ax.pie(percentages, explode=explode, labels=labels,
    #        colors=color_palette_list[0:2], autopct='%1.0f%%',
    #        shadow=False, startangle=0,
    #        pctdistance=1.2, labeldistance=1.4)
    

    # fig.subplots_adjust(wspace=.2)

    # sns.barplot(x="Number of Players", y="Goal", hue="End Result", data=test)
    # test.T.plot.pie(subplots=True, figsize=(10, 3))
    # plt.show()

    # print(test.head(5))


df = pd.read_csv("../GameInfo/" + PLAYER_TYPE + "Results_fixed.csv")

plotHist(df)
# plotBar_meanRoundsPerGoal(df)
# numRounds_vs_numPlayers(df)
# countResults(df)
