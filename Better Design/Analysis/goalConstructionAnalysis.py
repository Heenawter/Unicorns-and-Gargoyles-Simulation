import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math
import re


playerCounts = [3, 4, 5, 6, 7, 8, 9, 10]

df = pd.read_csv("./Data/FIXED_2Players_AllPossibleGoals_AllAggressive.csv")
for count in playerCounts:
    df2 = pd.read_csv("./Data/FIXED_" + str(count) + "Players_AllPossibleGoals_AllAggressive.csv")
    df = df.append(df2)

# df1 = pd.read_csv("./Data/5Players_AllPossibleGoals_AllAggressive.csv")
# df2 = pd.read_csv("./Data/10Players_AllPossibleGoals_AllAggressive.csv")

# df = df1.append(df2)

df["Mermaids"] = df["Goal"].str.count("[*]", re.I)
df["Bubbles"] = df["Goal"].str.count("[ ]", re.I)
df["Length"] = df["Mermaids"] + df["Bubbles"]

mask = df["End Result"].str.contains("wins")
onlyWins = df[mask]
mermaidsDf = onlyWins[["Mermaids", "Number of Rounds"]].groupby("Mermaids").median().reset_index()

lengthDf = df[["Length", "Number of Rounds"]].groupby("Length").median().reset_index()

plt.figure(figsize=(12, 6))
sns.set_style("darkgrid")

plt.plot('Mermaids', 'Number of Rounds', data=mermaidsDf, marker='o', color='#1897d5',
         linewidth=2, label="Number of Mermaids in Goal", markersize=10)
plt.plot('Length', 'Number of Rounds', data=lengthDf, marker='o', color='#bd331c',
         linewidth=2, linestyle='dashed', label="Length of Goal", markersize=10)
plt.legend(loc='upper left')

plt.tight_layout()
plt.show()
