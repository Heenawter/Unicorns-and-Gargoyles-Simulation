import pandas as pd
import numpy as npseabo
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math
import re

def plotPie(df):
    plt.figure(figsize=(6, 6))
    sns.set_style("darkgrid")

    # create donut chart
    names = ["The Thinker", "The Mizer", "The Troll"]
    wedges, labels, autopct = plt.pie(x='Number of Games Won', data=df, labels=names, labeldistance=1.05,
                                    autopct='%1.1f%%', pctdistance=0.79, colors=['#ae2f9d', '#5ab248', '#0165b0'],
                                    wedgeprops={'linewidth': 7, 'edgecolor': 'white'})
    my_circle=plt.Circle( (0,0), 0.6, color='white')
    p = plt.gcf()
    p.gca().add_artist(my_circle)

    # plt.rcParams.update({'font.size': 16})
    plt.setp(labels, fontsize=18)
    plt.setp(autopct, fontsize=16, color="white")

    # plt.title("Percent of Games Won by Player Type")

    plt.tight_layout()
    plt.show()

def plotBar(df):
    plt.style.use('ggplot')

    total = sum(df["Number of Games Won"])
    df["Games Won"] = (df["Number of Games Won"] / total) * 100
    # print(df)

    flipped = df[["Games Won"]].T
    flipped = flipped.rename(columns={0: "The Fox", 1: "The Sloth", 2: "The Troll"})
    print(flipped)

    ax = flipped.plot(kind='barh', stacked=True, figsize=(12, 3), legend=False, color=['#ae2f9d', '#5ab248', '#0165b0'],)
    plt.xticks(range(0, 101, 10), [str(x) + "%" for x in range(0, 101, 10)])
    for tick in ax.xaxis.get_major_ticks():
        tick.label.set_fontsize(14)

    plt.xlim([0, 100])
    y_axis = ax.axes.get_yaxis()
    y_axis.set_visible(False)

    labels = ["The Fox", "The Sloth", "The\nTroll"]
    i = 0
    for p in ax.patches:
        # ax.annotate(str(p.get_height()), (p.get_x() * 1.005, p.get_height() * 1.005))
        plt.text(p.get_x() + (p.get_width() / 2) , p.get_y() + 0.25, "%.0f%%" % p.get_width(),
                 ha="center", va="center", color="white", fontsize=18, fontweight="bold")
        plt.text(p.get_x() + (p.get_width() / 2), p.get_y() + 0.615, labels[i],
                 ha="center", va="center", color="black", fontsize=18)
        i += 1

    # # Plot the total crashes
    # sns.set_color_codes("pastel")
    # plt.plot(type="bar", data=df2, stacked=True)

    # # Plot the crashes where alcohol was involved
    # sns.set_color_codes("muted")
    # sns.barplot(x="alcohol", y="abbrev", data=crashes,
    #             label="Alcohol-involved", color="b")

    # # Add a legend and informative axis label
    # ax.legend(ncol=2, loc="lower right", frameon=True)
    # ax.set(xlim=(0, 24), ylabel="",
    #     xlabel="Automobile collisions per billion miles")
    # sns.despine(left=True, bottom=True)

    plt.tight_layout()
    plt.show()

playerTypes = ["aggressive", "greedy", "troll"]
df = pd.read_csv("./Data/permutationsOf3Players_AllPossibleGoals_1ofEachType_10runs.csv")
print(len(df))
print(len(df[~df["End Result"].str.contains("wins")]))

df = df[df["End Result"].str.contains("wins")]
print(len(df))
df["Winning Player"] = "Player " + df["End Result"].str[7:8]
df[['Player 0', 'Player 1', 'Player 2']] = df['Player Types'].str.split(';', expand=True)
df["Player 0"] = df["Player 0"].str[1:]
df["Player 2"] = df["Player 2"].str[:-1]
# print(df["Winning Player"])


df = df[["Winning Player", "Number of Rounds", "Player 0", "Player 1", "Player 2"]]
df["Winning Player Type"] = df.lookup(df.index, df["Winning Player"])
# print(df.head(15))

df = df[["Winning Player Type", "Number of Rounds"]].groupby("Winning Player Type").count().reset_index()
df = df.rename(columns={"Number of Rounds": "Number of Games Won"})
print(df)


# plotPie(df)
plotBar(df)
# plotLine(df)
