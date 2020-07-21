import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math

playerCounts = [2, 5, 6, 10]
winPercentages = []
stalePercentages = []
longPercentages = []

for count in playerCounts:
    df = pd.read_csv("./Data/" + str(count) + "Players_AllPossibleGoals_AllAggressive.csv")

    mask = df["End Result"].str.contains("win")
    onlyWins = df[mask]
    noWin = df[~mask]

    total = df.size
    wins = (onlyWins.size / total) * 100
    stalemate = (noWin.size / total) * 100

    winPercentages.append(wins)
    stalePercentages.append(stalemate)

plt.figure(figsize=(12, 6))
sns.set_style("darkgrid")

ax2 = plt.bar(playerCounts, stalePercentages, bottom=winPercentages,
              color='#294da1', edgecolor='white', width=0.85, label="Stalemate")
ax1 = plt.bar(playerCounts, winPercentages, color='#f7a609',
        edgecolor='white', width=0.85, label="Completed")


# this is a test comment
for r1, r2 in zip(ax1, ax2):
    h1 = r1.get_height()
    h2 = r2.get_height()

    if(h1 >= 3):
        plt.text(r1.get_x() + r1.get_width() / 2., h1 / 2., "%.1f%%" % h1,
                ha="center", va="center", color="white", fontsize=14, fontweight="bold")
    if(h2 >= 3):
        plt.text(r2.get_x() + r2.get_width() / 2., h1 + h2 / 2., "%.1f%%" % h2,
                ha="center", va="center", color="white", fontsize=14, fontweight="bold")

plt.title("Percentage of Games that End in a Stalemate")
plt.legend(loc='upper left', bbox_to_anchor=(1, 1), ncol=1)
plt.xlabel("Number of Players")
plt.tight_layout()

plt.show()
