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
    plt.savefig("./Plots/WinningGames_NumRoundsVSNumPlayers_All" + PLAYER_TYPE)
    plt.show()

def plotHist(df):
    sns.set_style("darkgrid")
    maxRounds = max(df["Number of Rounds"])
    rounded = int(math.ceil(maxRounds / 10.0)) * 10
    lastTwo = rounded % 100


    customRange = chain(range(0, lastTwo, 10), range(lastTwo, rounded + 1, 100)) 
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
    plt.savefig("./Plots/GameLengths_All" + PLAYER_TYPE)
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


# plotBar_meanRoundsPerGoal()

df = pd.read_csv("../GameInfo/" + PLAYER_TYPE + "Results.csv")

plotHist(df)
numRounds_vs_numPlayers(df)
