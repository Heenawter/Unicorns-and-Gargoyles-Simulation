import pandas as pd
import numpy as np
from matplotlib import rcParams
import matplotlib.font_manager as font_manager
# font_dirs = ['C:/Users/Hannah/AppData/Local/Microsoft/Windows/Fonts']
# font_files = font_manager.findSystemFonts(fontpaths=font_dirs)
# font_list = font_manager.createFontList(font_files)
# font_manager.fontManager.ttflist.extend(font_list)
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math

# df = pd.read_csv("./Analysis/Data/MiddleSchoolClassroomResults_322222222222_shorterClass.csv")
# df = pd.read_csv("./Analysis/Data/MiddleSchoolClassroomResults_smallTest.csv")
# df = pd.read_csv("./Analysis/Data/UniversityClassroomResults_25x6_40s.csv")
# df = pd.read_csv("./Analysis/Data/UniversityClassroomResults_25x6_40s_50mins.csv")
# df = pd.read_csv("./Analysis/Data/UniversityClassroomResults_tutorial_5x5.csv")
df = pd.read_csv("./Analysis/Data/UniversityClassroomResults_tutorial_3x1_2x11.csv")


def printSummary(df, meanDescription):
    print("- mean:", df["mean"], "\t", meanDescription)
    print("- std: ", df["std"])
    print("- min: ", df["min"])
    print("- 25%: ", df["25%"])
    print("- 50%: ", df["50%"])
    print("- 75%: ", df["75%"])
    print("- max: ", df["max"], "\n")


df["Number of Incomplete Games"] = df["End Results"].str.count("Ran out of time")
df["Number of Completed Games"] = df["End Results"].str.count("wins")

totalGames = df["Number of Games"].sum()
print("Total # of games:", totalGames)

df2 = df[["Session", "Number of Games"]].groupby(["Session"]).sum()["Number of Games"].describe()
print("# of games per session trial:")
print("- mean:", df2["mean"], "\t(average games per trial)")
print("- std: ", df2["std"])
df2 = df[["Session", "Number of Games"]].groupby(["Session"]).mean()["Number of Games"].describe()
print("# of games per group per trial:")
print("- mean:", df2["mean"], "\t(average games per group in a single trial)")
print("- std: ", df2["std"], "\n")

singleGames = df[df["Number of Games"] == 1]
singleGames_notCompleted = singleGames[singleGames["End Results"].str.contains("Ran out of time")]
singleGames_notCompleted_bySession = singleGames_notCompleted[["Session", "Group"]].groupby(["Session"]).count()

# print("Total # of groups that had a single game (partial or incomplete):", len(singleGames), "out of", totalGames)
# print("Total # of groups that participated in only one game:", len(singleGames))
# print("Total # of groups that couldn't complete a single game:", len(singleGames_notCompleted))
# print("Total # of groups that won exactly one game:", len(singleGames_winner))
print("Total # of trials with at least one group that couldn't complete a single game:",
      len(singleGames_notCompleted_bySession), "out of", len(df["Session"].unique()))
print("Description of # of games where not a single game had a winner:")
printSummary(singleGames_notCompleted_bySession["Group"].describe(),
             "(average # of incomplete games in trials where there are incomplete games)")

totalGames_bySession = df[["Number of Completed Games", "Session"]].groupby("Session").sum()
print("Total # of *completed* games where a winner is declared:", df["Number of Completed Games"].sum())
printSummary(totalGames_bySession["Number of Completed Games"].describe(),
             "(average # of games with a winner in a single trial)")

def findOvertime(val):
    split = val.split(";")
    return int(split[len(split) - 1])

def sumTurns(val):
    split = val.split(";")
    turnSum = 0
    for turn in split:
        turnSum += int(turn)
    return turnSum

def turnsPerCompleted(row):
    completed = row["Number of Completed Games"]
    split = row["Number of Turns"].split(";")
    turnSum = 0
    for i in range(completed):
        turnSum += int(split[i])

    if(completed > 0):
        return turnSum / completed
    return 0

def lengthOfCompletedGames(row):
    numGames = row["Number of Games"]
    splitResults = row["End Results"].split(";")
    splitTurns = row["Number of Turns"].split(";")

    times = 0
    for i in range(numGames):
        if("wins" in splitResults[i]):
            turnCount = int(splitTurns[i])
            time = (turnCount * 40) + 180
            times += time

    return times

def finalGameEnding(val):
    split = val.split(";")
    final = split[len(split) - 1]
    if("Ran out of time" in final): 
        return "Ran out of time"
    elif("wins" in final):
        return "Player wins"
    else:
        return split[len(split) - 1]

def totalGames_fixed(row):
    if(row["Number of Games"] > 1 and row["Remaining Time"] < 0):
        return row["Number of Games"] - 1
    else:
        return row["Number of Games"]

groupsWithWinner = df[df["Number of Completed Games"] > 0]
print("Total number of groups in every trial with a winner:", len(groupsWithWinner), "out of", len(df))
printSummary(df["Number of Completed Games"].describe(),
             "(average # of games with a winner in a single trial for a single group)")
print("Excluding groups that were unable to complete a single game, describe the number of games:")
printSummary(groupsWithWinner["Number of Completed Games"].describe(),
             "(average # of games with a winner in a single trial for a single group)")

df["Remaining Time"] = df["Remaining Class Times"].apply(findOvertime)
df["Fixed Game Count"] = df.apply(totalGames_fixed, axis=1)
print("Total # of games excluding partials if group already completed a game:", df["Fixed Game Count"].sum(), "out of", totalGames)

df["Length of Completed Games"] = df.apply(lengthOfCompletedGames, axis=1)
df["Average Length of Completed Games"] = (df["Length of Completed Games"] / df["Number of Completed Games"]) / 60
print("\nLength of games with a winner:")
printSummary(df["Average Length of Completed Games"].describe(), "(average length of winning games, in minutes)")

overtime = df[df["Remaining Time"] < 0]
print("Number of groups that went overtime:", len(overtime), "out of", len(df))
print("- of these,", len(overtime[overtime["End Results"].str.contains("win")]), "had already won a game")
print("Number of groups that completed a game:", len(df[df["End Results"].str.contains("win")]), "out of", len(df))
print("Number of groups that couldn't complete a game:",
       len(df[(df["End Results"].str.contains("out of time")) & (df["Number of Games"] == 1)]), "out of", len(df))

df["Total number of turns"] = df["Number of Turns"].apply(sumTurns)
df["Number of Players"] = df["Player Types"].str.count(";") + 1
df["Turns per Player"] = df["Total number of turns"] / df["Number of Players"]
print("Description of number of turns typical game lasts:")
printSummary(df["Turns per Player"].describe(),
             "(average number of turns each player takes)")

df["Turns per Winning Game"] = df.apply(turnsPerCompleted, axis=1)
df["Turns per Winning Game per Player"] = df["Turns per Winning Game"] / df["Number of Players"]
withAWinner = df[df["End Results"].str.contains("win")]
print(withAWinner["Turns per Winning Game per Player"].describe())

print("Class time left after all games:\n", df["Remaining Time"].describe())

# df["Final Game Result"] = df["End Results"].apply(finalGameEnding)
# df["Remaining Time"] = df["Remaining Class Times"].apply(findOvertime)
# groupsWithCompletedGames_partialEnding = df[(df["Final Game Result"] == "Ran out of time") & (df["Number of Games"] > 1)]
# print("Number of groups that completed a game but ended with a partial:", len(groupsWithCompletedGames_partialEnding))
# print("- Number of complete games from groups above:", groupsWithCompletedGames_partialEnding["Number of Completed Games"].sum())



# df["Total Games Excluding Final if Partial"] = df.apply(totalGames_excludingFinalPartial, axis=1)
# print("--->", df["Number of Completed Games"].sum(), "out of", totalGames, "had a winner.")
# print("--->", df["Number of Completed Games"].sum(), "out of", 
#                 df["Total Games Excluding Final if Partial"].sum(),
#                 "had a winner, excluding the partial games when one game already completed.")

# print("Class time left after all games:\n", df["Remaining Time"].describe())

# df["Total Turns"] = df["Number of Turns"].apply(sumTurns)
# df["Turns Per Player"] = df["Total Turns"] / (df["Player Types"].str.count(";") + 1)
# print(df[df["Total Turns"] < 5])
# print("Total turns per session per group:\n", df["Total Turns"].describe())
# print("Turns per player per session:\n", df["Turns Per Player"].describe())

# df["Length of Completed Games"] = df.apply(lengthOfCompletedGames, axis=1)
# print("Average length of games with a winner:\n", df[ "Length of Completed Games"].describe())

# df["Length of Partial Games"] = df.apply(lengthOfIncompleteGames, axis=1)
# print("Average length of games that ran out of time:\n", df["Length of Partial Games"].describe())

# print("Number of games that ended early:", len(df[df["Remaining Time"] >= 0]))
# print("- Describe per session:\n", df[df["Remaining Time"] >= 0].groupby("Session").count()["Remaining Time"].describe())

# print("Number of games that ended late:", len(df[df["Remaining Time"] < 0]))
# print("- Describe per session:\n", df[df["Remaining Time"] < 0].groupby("Session").count()["Remaining Time"].describe())
# print("Total number of turns per group per session:\n", df["Total Turns"].describe())


















# print(df[["Session","Number of Incomplete Games"]].groupby("Session").sum()["Number of Incomplete Games"].describe())
# print("- Number of greedy:\n", singleGames_notCompleted[singleGames_notCompleted[""]])

# def mapVals(val):
#     if "wins" in val:
#         return "Player wins"
#     elif "Ran out of time" in val:
#         return "Ran out of time"
#     else:
#         return val;

# df["Simplified End Result"] = df["End Result"].apply(mapVals)

# # remove the partial games were at least a single game has been completed previously
# removeParial = df[~((df["Simplified End Result"] == "Ran out of time") & (df["Game Number"] > 1))]

# # find all sessions where a game was incomplete
# print("--- No games completed ---")
# df2 = df
# df2["First Game Incomplete"] = (df2["Simplified End Result"] == "Ran out of time") & (df2["Game Number"] == 1)
# print(df2[["Session", "Group", "First Game Incomplete"]].groupby(["Session", "First Game Incomplete"]).count())
# noGamesComplete = noGamesComplete[["Session", "Group"]].groupby(["Session"]).count()
# print(noGamesComplete["Group"].describe())


# print("--- Total games won ---")
# wonGames = df[df["Simplified End Result"] == "Player wins"]
# wonGames = wonGames[["Group", "Session", "Game Number"]].groupby(["Group", "Session"]).count()
# print(wonGames)

# print(removeParial[(df["Simplified End Result"] == "Player wins") & (df["Game Number"] > 1)])

# gameCounts = removeParial[["Group", "Session", "Game Number",]].groupby(["Group", "Session"]).count()
# print(gameCounts["Game Number"].describe())
# print(gameCounts[gameCounts["Game Number"] == 4])


# df["Simplified End Result"] = df["End Result"].apply(mapVals)


# print(df[["Simplified End Result", "Group"]].groupby("Simplified End Result").describe())

# sns.barplot(data=df.unstack())
# plt.show()

