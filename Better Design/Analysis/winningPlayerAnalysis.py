import pandas as pd
import numpy as np
from matplotlib import rcParams
import matplotlib.font_manager as font_manager
import matplotlib.ticker as mtick
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math
import re


def plotPie(df):
    plt.figure(figsize=(8, 4))
    sns.set_style("darkgrid")
    sns.set(rc={'axes.facecolor': '#f7f7f7', 'figure.facecolor': 'white'})
    # rcParams['axes.titlesize'] = 16
    rcParams['font.family'] = 'Calibri'
    rcParams['font.style'] = 'normal'
    sns.set_context(context="notebook", rc={"axes.titlesize": 18, "xtick.labelsize": 15, "xtick.fontstyle": "bold",
                                            "axes.labelsize": 18, "ytick.labelsize": 15, "legend.fontsize": 15, "axes.margins": 1})
    # Set your custom color palette
    colors = ["#f07508", "#fbc531", "#981e82", "#0a98d8", "#ee6283"]
    customPalette = sns.set_palette(sns.color_palette(colors))

    # create donut chart
    names = ["The Fox wins", "The Sloth wins", "The Troll wins",
             "A player had more than 20 cards", "Only action cards left in the deck"]
    wedges, labels, autopct = plt.pie(x='Number of Games Won', data=df, 
                                      autopct='%1.1f%%', pctdistance=0.725, colors=colors,
                                      wedgeprops={'linewidth': 6, 'edgecolor': 'white'})
    my_circle = plt.Circle((0, 0), 0.5, color='white')
    p = plt.gcf()
    p.gca().add_artist(my_circle)

    # plt.rcParams.update({'font.size': 16})
    # plt.setp(labels, fontsize=18)
    plt.setp(autopct, fontsize=18, color="black")
    # plt.setp(autopct[-3], fontsize=18, color="white")
    # plt.setp(autopct[-2], fontsize=0, color="white")
    # plt.setp(autopct[-1], fontsize=0, color="white")

    plt.setp(autopct[-3], fontsize=0)

    print(autopct)
    # plt.title("Percent of Games Won by Player Type")

    plt.tight_layout()
    plt.subplots_adjust(right=0.51, left=0, top=1, bottom=0, wspace=0, hspace=0)
    plt.legend(labels=names, loc='center right', handletextpad=0.3,
               ncol=1, bbox_to_anchor=(1.9, 0.5))
    plt.savefig("./Analysis/Plots/WinningPlayerAnalysis.png", dpi=290)
    plt.show()

def plotPoints(df):
    sns.set_style("darkgrid")
    sns.set(rc={'axes.facecolor': '#f7f7f7', 'figure.facecolor': 'white'})
    # rcParams['axes.titlesize'] = 16
    rcParams['font.family'] = 'Calibri'
    rcParams['font.style'] = 'normal'
    sns.set_context(context="notebook", rc={"axes.titlesize": 18, "xtick.labelsize": 15, "xtick.fontstyle": "bold",
                                            "axes.labelsize": 18, "ytick.labelsize": 15, "legend.fontsize": 15, "axes.margins": 1})
    # Set your custom color palette
    colors = ["#f07508", "#214aa7", "#981e82", "#d8d8d8"]
    customPalette = sns.set_palette(sns.color_palette(colors))

    # plt.figure(figsize=(6, 5))
    f, ax = plt.subplots(figsize=(6, 4))
    sns.pointplot(data=df, x="Goal Length", ax=ax, dodge=True, 
                y="Number of Rounds", hue="Winning Player Type", ci=None)

    # g = sns.barplot(
    #     data=roundDf,x="Winning Player Type", y="Number of Rounds", ci="sd")
    plt.ylabel("Mean # of Rounds")
    handles, labels = ax.get_legend_handles_labels()
    labels = ["The Fox", "The Sloth", "The Troll", "No Winner"]
    ax.legend(handles, labels, loc='upper left', handletextpad=-0.25, columnspacing=0.5,
                  ncol=1)
    plt.tight_layout()
    plt.savefig("./Analysis/Plots/WinningPlayerAnalysis_rounds.png", dpi=290)
    plt.show()
    # roundDf = roundDf[["Winning Player Type", "Number of Rounds"]].groupby(
    #     "Winning Player Type").describe()
    # print(roundDf)


def plotBar(df):
    sns.set_style("darkgrid")
    sns.set(rc={'axes.facecolor': '#f7f7f7', 'figure.facecolor': 'white'})
    # rcParams['axes.titlesize'] = 16
    rcParams['font.family'] = 'Calibri'
    rcParams['font.style'] = 'normal'
    sns.set_context(context="notebook", rc={"axes.titlesize": 18, "xtick.labelsize": 15, "xtick.fontstyle": "bold",
                                            "axes.labelsize": 18, "ytick.labelsize": 15, "legend.fontsize": 15, "axes.margins": 1})
    total = sum(df["Number of Games Won"])
    df["Games Won"] = (df["Number of Games Won"] / total) * 100
    # print(df)

    flipped = df[["Games Won"]].T
    flipped = flipped.rename(
        columns={0: "The Fox", 1: "The Sloth", 2: "The Troll"})
    print(flipped)

    colors = ["#f07508", "#214aa7", "#981e82", "#d8d8d8"]
    ax = flipped.plot(kind='barh', stacked=True, figsize=(
        12, 3), legend=False, color=colors,)
    plt.xticks(range(0, 101, 10), [str(x) + "%" for x in range(0, 101, 10)])
    for tick in ax.xaxis.get_major_ticks():
        tick.label.set_fontsize(14)

    plt.xlim([0, 100])
    y_axis = ax.axes.get_yaxis()
    y_axis.set_visible(False)

    labels = ["The Fox", "The Sloth", "The Troll", ""]
    i = 0
    for p in ax.patches:
        # ax.annotate(str(p.get_height()), (p.get_x() * 1.005, p.get_height() * 1.005))
        plt.text(p.get_x() + (p.get_width() / 2), p.get_y() + 0.25, "%1.1f%%" % p.get_width(),
                 ha="center", va="center", color="white", fontsize=18, fontweight="bold")
        plt.text(p.get_x() + (p.get_width() / 2), p.get_y() + 0.615, labels[i],
                 ha="center", va="center", color="black", fontsize=18)
        i += 1

    plt.tight_layout()
    plt.savefig("./Analysis/Plots/WinningPlayerAnalysis_bar.png", dpi=290)

    plt.show()


df = pd.read_csv("./Analysis/Data/playerTypeTest_whoWins.csv")

numPlayers = 3
playerList = []
for i in range(numPlayers):
    playerList.append("Player " + str(i))

df[playerList] = df['Player Types'].str.split(';', expand=True)
print(df.head(10))
print("Total time: ", df["Game Time"].sum())

df["Too many cards in"] = "too many"
df["Only action cards"] = "only action"
df["Player 0"] = df["Player 0"].str[1:]
df["Player " + str(numPlayers - 1)] = df["Player " + str(numPlayers - 1)].str[:-1]
df["Winning Player"] = df["End Result"].str[:-5]

df["Winning Player Type"] = df.lookup(df.index, df["Winning Player"])

winnerDf = df.drop(playerList + ["Seed", "Goal", "Player Types", "Number of Players",
                    "End Result", "Too many cards in", "Only action cards"], axis=1)
winnerDf = winnerDf[["Winning Player Type", "Number of Rounds"]].groupby(
    "Winning Player Type").count()
winnerDf["Percentage"] = winnerDf["Number of Rounds"] / winnerDf["Number of Rounds"].sum()
print(winnerDf)
winnerDf = winnerDf.reindex(["aggressive", "greedy", "troll", "too many", "only action"])
winnerDf = winnerDf.rename(columns={"Number of Rounds": "Number of Games Won"})

# plotPie(winnerDf)

# roundDf = df.drop(["Seed", "Player Types", "Number of Players",
#                     "End Result"] + playerList, axis=1)

# roundDf["Mermaids"] = roundDf["Goal"].str.count("[*]", re.I)
# roundDf["Bubbles"] = roundDf["Goal"].str.count("[ ]", re.I)
# roundDf["Goal Length"] = roundDf["Mermaids"] + roundDf["Bubbles"]

# plotPoints(roundDf)


# test = df.drop(["Seed", "Goal", "Player Types", "Number of Players", "End Result"] + playerList, axis=1)
# test = test.groupby(["Winning Player"]).count()
# plt.pie(test["Number of Rounds"], labels=test.index, autopct='%1.1f%%')
# plt.show()
# print(test["Number of Rounds"])
