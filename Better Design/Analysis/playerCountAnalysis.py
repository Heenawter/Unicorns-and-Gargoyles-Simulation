import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math

playerCounts = [2, 3, 4, 5, 6, 7, 8, 9, 10]
winPercentages = []
onlyActionCardsPercentages = []
tooManyCardsPercentages = []

for count in playerCounts:
    df = pd.read_csv("./Data/FIXED_" + str(count) + "Players_AllPossibleGoals_AllAggressive.csv")

    mask = df["End Result"].str.contains("win")
    onlyWins = df[mask]
    noWin = df[~mask]

    total = df.size
    wins = (onlyWins.size / total) * 100

    mask2 = noWin["End Result"] == "Too many cards in hand"
    tooManyCards_df = noWin[mask2]
    onlyActionCards_df = noWin[~mask2]
    
    tooManyCards = (tooManyCards_df.size / total) * 100
    onlyActionCards = (onlyActionCards_df.size / total) * 100

    winPercentages.append(wins)
    tooManyCardsPercentages.append(tooManyCards)
    onlyActionCardsPercentages.append(onlyActionCards)

print(tooManyCardsPercentages)

plt.figure(figsize=(12, 6))
sns.set_style("darkgrid")

ax3 = plt.bar(playerCounts, onlyActionCardsPercentages,
              bottom=[i+j for i, j in zip(winPercentages, tooManyCardsPercentages)], 
              color='#ba311c', edgecolor='white', width=0.85, label="Only Action Cards in Deck")
ax2 = plt.bar(playerCounts, tooManyCardsPercentages, bottom=winPercentages,
              color='#f59609', edgecolor='white', width=0.85, label="Too Many Cards in Hand")
ax1 = plt.bar(playerCounts, winPercentages, color='#6cbb4c',
        edgecolor='white', width=0.85, label="Reached Conclusion")


# this is a test comment
for r1, r2, r3 in zip(ax1, ax2, ax3):
    h1 = r1.get_height()
    h2 = r2.get_height()
    h3 = r3.get_height()

    if(h1 >= 3):
        plt.text(r1.get_x() + r1.get_width() / 2., h1 / 2., "%.1f%%" % h1,
                ha="center", va="center", color="white", fontsize=14, fontweight="bold")
    if(h2 >= 3):
        plt.text(r2.get_x() + r2.get_width() / 2., h1 + h2 / 2., "%.1f%%" % h2,
                ha="center", va="center", color="white", fontsize=14, fontweight="bold")
    if(h3 >= 3):
        plt.text(r3.get_x() + r3.get_width() / 2., h1 + h2 + h3 / 2., "%.1f%%" % h3,
                 ha="center", va="center", color="white", fontsize=14, fontweight="bold")

plt.title("Percentage of Games that End in a Stalemate")
plt.legend(loc='upper left', bbox_to_anchor=(1, 1), ncol=1)
plt.xlabel("Number of Players")
plt.tight_layout()

plt.show()
