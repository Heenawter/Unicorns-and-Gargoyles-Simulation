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


df = pd.read_csv("./Analysis/Data/goalAnalysis_results_after_balancing_2players.csv")
for i in range(3, 7):
    df2 = pd.read_csv("./Analysis/Data/goalAnalysis_results_after_balancing_" + str(i) + "players.csv")
    df = df.append(df2,  ignore_index=True)


# df = pd.read_csv("./Analysis/Data/goalAnalysis_results_2players_100.csv")
# for i in range(3, 13):
#     df2 = pd.read_csv("./Analysis/Data/goalAnalysis_results_" + str(i) + "players_100.csv")
#     df = df.append(df2,  ignore_index=True)


df["Mermaid Count"] = df["Goal"].str.count("[*]", re.I)
df["Goal Length"] = df["Goal"].str.count("]")

winners = df[df["End Result"].str.contains("wins")]
winners["Fixed End Result"] = "A player wins"
df = df.join(winners["Fixed End Result"])
df["Fixed End Result"].fillna(df["End Result"], inplace=True)
df.drop(["End Result", "Goal", "Seed"], axis=1, inplace=True)

dfM = df[["Number of Rounds", "Mermaid Count", "Fixed End Result"]]
dfM2 = df[["Number of Rounds", "Mermaid Count", "Fixed End Result", "Number of Players"]]
dfM = dfM.groupby(["Mermaid Count", "Fixed End Result"]).count()
dfM = dfM.groupby(level=0).apply(lambda x: 100 * x / float(x.sum()))
dfM = dfM.unstack().fillna(0)
dfM = dfM["Number of Rounds"][["A player wins", "Too many cards in hand", "Only action cards left"]]

dfL = df[["Number of Rounds", "Goal Length", "Fixed End Result"]]
dfL2 = df[["Number of Rounds", "Goal Length", "Fixed End Result", "Number of Players"]]
dfL = dfL.groupby(["Goal Length", "Fixed End Result"]).count()
dfL = dfL.groupby(level=0).apply(lambda x: 100 * x / float(x.sum()))

dfL = dfL.unstack().fillna(0)
dfL = dfL["Number of Rounds"][["A player wins", "Too many cards in hand", "Only action cards left"]]


sns.set_style("darkgrid")
sns.set(rc={'axes.facecolor': '#f7f7f7', 'figure.facecolor': 'white'})
# rcParams['axes.titlesize'] = 16
rcParams['font.family'] = 'Calibri'
rcParams['font.style'] = 'normal'
sns.set_context(context="notebook", rc={"axes.titlesize": 18, "xtick.labelsize": 15, "xtick.fontstyle": "bold",
                                        "axes.labelsize": 18, "ytick.labelsize": 15, "legend.fontsize": 15, "axes.margins": 1})
colors = ["#59b32d", "#0a98d8", "#ee6283"]  # Set your custom color palette
customPalette = sns.set_palette(sns.color_palette(colors))

fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 6), sharex='col', sharey='row')

dfL.plot.bar(stacked=True, rot=0, ax=axes[0, 0])
axes[0, 0].set_ylabel("% of Games")
axes[0, 0].set_xlabel("")
axes[0, 0].set_yticks(np.arange(0, 101, 20))
axes[0, 0].legend().remove()

dfM.plot.bar(stacked=True, rot=0, ax=axes[0, 1])
axes[0, 1].legend().remove()
axes[0, 1].set_ylabel("")
axes[0, 1].set_xlabel("")

# dfL2["Mean # of Turns"] = dfL2["Number of Players"] * dfL2["Number of Rounds"]
sns.pointplot(x="Goal Length", y="Number of Rounds",  hue="Fixed End Result", data=dfL2,
              ax=axes[1, 0], linewidth=3, markers=["s", "o", "D"], ci=None)
axes[1, 0].legend().remove()
axes[1, 0].set_xlabel('Goal String Length')
axes[1, 0].set_ylabel("Mean # of Rounds", labelpad=22)
 
# dfM2["Mean # of Turns"] = dfM2["Number of Players"] * dfM2["Number of Rounds"]
sns.pointplot(x="Mermaid Count", y="Number of Rounds",  hue="Fixed End Result", data=dfM2,
              ax=axes[1, 1], linewidth=3, markers=["s", "o", "D"], ci=None)
axes[1, 1].set_xlabel("Number of Mermaids in Goal String")
axes[1, 1].legend().remove()
axes[1, 1].set_ylabel("")
handles, labels = axes[1, 1].get_legend_handles_labels()

fig.suptitle('Breakdown of Ending Conditions for Games with Different Goal Strings', fontsize=22)
plt.tight_layout()
fig.subplots_adjust(bottom=0.2, top=0.9)
axes[0, 0].yaxis.set_major_formatter(mtick.PercentFormatter())

labels = ["A player wins the game", "A player had more than 20 cards",
          "Only action cards left in the deck"]
axes[1, 1].legend(handles, labels, loc='upper center', handletextpad=-0.25, columnspacing=0.5,
           ncol=3, bbox_to_anchor=(-0.13, -0.3))
axes[1, 0].set_yticks(np.arange(0, 71, 10))

plt.show()
fig.savefig("./Analysis/Plots/GoalAnalysisGraph_balance_2-6players.png", dpi=290)

