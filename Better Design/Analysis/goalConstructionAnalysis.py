import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math
import re

df1 = pd.read_csv("./Data/5Players_AllPossibleGoals_AllAggressive.csv")
df2 = pd.read_csv("./Data/10Players_AllPossibleGoals_AllAggressive.csv")

df = df1.append(df2)

df["Mermaids"] = df["Goal"].str.count("[*]", re.I)
df["Bubbles"] = df["Goal"].str.count("[ ]", re.I)
df["Length"] = df["Mermaids"] + df["Bubbles"]

mask = df["End Result"].str.contains("wins")
onlyWins = df[mask]
mermaidsDf = onlyWins[["Mermaids", "Number of Rounds"]
                      ].groupby("Mermaids").median().reset_index()

lengthDf = df[["Length", "Number of Rounds"]].groupby("Length").median().reset_index()

plt.plot('Mermaids', 'Number of Rounds', data=mermaidsDf, marker='o', color='#1897d5',
         linewidth=2, label="toto2", markersize=10)
plt.plot('Length', 'Number of Rounds', data=lengthDf, marker='o', color='#bd331c',
         linewidth=2, linestyle='dashed', label="toto", markersize=10)
plt.tight_layout()
plt.show()
