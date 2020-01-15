import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import math


def plotBar_meanRoundsPerGoal():
    df = pd.read_csv("../GameInfo/results.csv")

    meanRoundsPerGoal = df[["Goal", "Number of Rounds"]].groupby("Goal").mean().reset_index()
    # meanRoundsPerGoal.sort_values("Number of Rounds", inplace=True, ascending=False);

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


plotBar_meanRoundsPerGoal()
