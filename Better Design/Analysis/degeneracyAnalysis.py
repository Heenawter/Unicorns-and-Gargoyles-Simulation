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

def plotme(df):
    df = df[["Number of Players", "Number of Rounds", "End Result"]]

    winners = df[df["End Result"].str.contains("wins")]
    winners["Fixed End Result"] = "A player wins"
    df = df.join(winners["Fixed End Result"])
    df["Fixed End Result"].fillna(df["End Result"], inplace=True)

    df = df[df["Number of Players"] <= 12]
    df = df[df["Number of Players"] >= 2]
    df = df.groupby(["Number of Players", "Fixed End Result"]).count()
    # df = df.drop(["End Result"], axis=1)
    df = df.reset_index()
    # df["Number of Rounds"] = (df["Number of Rounds"] * (df["Number of Players"] * 30)) / 60
    df = df.rename(columns={"Fixed End Result": "Ending Condition", "Number of Rounds": "Game Count"})

    sns.set_style("darkgrid")
    sns.set(rc={'axes.facecolor': '#f7f7f7', 'figure.facecolor': 'white'})
    # rcParams['axes.titlesize'] = 16
    rcParams['font.family'] = 'Calibri'
    rcParams['font.style'] = 'normal'
    sns.set_context(context="notebook", rc={"axes.titlesize": 18, "xtick.labelsize": 15, "xtick.fontstyle": "bold",
                                        "axes.labelsize": 18, "ytick.labelsize": 15, "legend.fontsize": 15, "axes.margins": 1})
    colors = ["#59b32d", "#0a98d8", "#ee6283"]  # Set your custom color palette
    customPalette = sns.set_palette(sns.color_palette(colors))
    g = sns.catplot(x = "Number of Players", # x variable name
                    y="Game Count",          # y variable name
                    hue="Ending Condition",  # group variable name
                    data = df,               # dataframe to plot
                    kind = "point",
                    legend_out=False, markers=["s", "o", "D"])
    g.fig.set_figheight(4)
    g.fig.set_figwidth(10)
    plt.title("Ending Conditions for Randomized Games with Differing Player Counts",
            fontsize=20,  pad=20)
    L = plt.legend()
    L.get_texts()[0].set_text('A player wins the game')
    L.get_texts()[1].set_text('A player had more than 20 cards')
    L.get_texts()[2].set_text('Only action cards left in the deck')
    plt.tight_layout()
    plt.savefig("./Analysis/Plots/DegeneracyAnalysisGraph.png", dpi=290)
    plt.show()


# df = pd.read_csv("./Analysis/Data/degeneracyTest_goalsControlled_results.csv")
plotme(df)

# df = pd.read_csv("./Analysis/Data/degeneracyTest_results_8bits_2mermaids.csv")
# plotme(df)

# print(df)

def plotmultiple():
    def plot_single(df_file, title, ax):
        df = pd.read_csv(df_file)
        winners = df[df["End Result"].str.contains("wins")]
        winners["Fixed End Result"] = "A player wins"
        df = df.join(winners["Fixed End Result"])
        df["Fixed End Result"].fillna(df["End Result"], inplace=True)

        df = df[df["Number of Players"] <= 12]
        df = df[df["Number of Players"] >= 2]
        df = df.groupby(["Number of Players", "Fixed End Result"]).count()
        # df = df.drop(["End Result"], axis=1)
        df = df.reset_index()
        # df["Number of Rounds"] = (df["Number of Rounds"] * (df["Number of Players"] * 30)) / 60
        df = df.rename(columns={"Fixed End Result": "Ending Condition", "Number of Rounds": "Game Count"})
        g = sns.pointplot(x="Number of Players",  # x variable name
                        y="Game Count",          # y variable name
                        hue="Ending Condition",  # group variable name
                        data=df,               # dataframe to plot
                        ax=ax,
                        legend_out=False, markers=["s", "o", "D"])
        ax.title.set_text(title)
        ax.legend().remove()

    sns.set_style("darkgrid")
    sns.set(rc={'axes.facecolor': '#f7f7f7', 'figure.facecolor': 'white'})
    # rcParams['axes.titlesize'] = 16
    rcParams['font.family'] = 'Calibri'
    rcParams['font.style'] = 'normal'
    sns.set_context(context="notebook", rc={"axes.titlesize": 18, "xtick.labelsize": 15, "xtick.fontstyle": "bold",
                                            "axes.labelsize": 18, "ytick.labelsize": 15, "legend.fontsize": 15, "axes.margins": 1})
    colors = ["#59b32d", "#0a98d8", "#ee6283"]  # Set your custom color palette
    customPalette = sns.set_palette(sns.color_palette(colors))

    fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(10, 5), sharex=True, sharey=True)

    row = 0
    for bits in range(4, 6 + 1):
        col = 0
        for merm in range(2, 3 + 1):
            plot_single("./Analysis/Data/degeneracyTest_results_" + str(bits) + "bits_"  + str(merm) + "mermaids.csv",
                        str(bits) + " bits, " + str(merm) + " mermaids", axes[col, row])
            axes[col, row].set_xlabel("")
            axes[col, row].set_ylabel("")
            axes[col, row].set_yticks(np.arange(0, 1001, 200))

            col += 1
        row += 1

    axes[1, 1].set_xlabel("Number of Players")
    handles, labels = axes[1, 2].get_legend_handles_labels()
    labels = ["A player wins the game", "A player had more than 20 cards",
              "Only action cards left in the deck"]
    plt.tight_layout()

    
    fig.legend(handles, labels, loc='lower center', handletextpad=-0.25, columnspacing=0.5,
                  ncol=3, bbox_to_anchor=(0.5, 0))
    plt.subplots_adjust(left=0.085, bottom=0.225, right=None, top=None, wspace=None, hspace=None)
    fig.text(0.015, 0.55, "Number of Games",
             ha="center", va="center", fontsize=18, rotation=90)

    plt.savefig("./Analysis/Plots/DegeneracyAnalysisGraph_6-8bits_1-3mermaids.png", dpi=290)
    plt.show()

# plotmultiple()

def plotMultiple_v2():
    def plot_single(df_file, title, ax, color):
        df = pd.read_csv(df_file)

        df = df[df["Number of Players"] <= 12]
        df = df[df["Number of Players"] >= 2]
        df = df.groupby(["Number of Players"]).mean()
        # df = df.drop(["End Result"], axis=1)
        df = df.reset_index()
        df = df.rename(columns={"Number of Rounds": "Mean Number of Rounds"})

        g = sns.pointplot(x="Number of Players",  # x variable name
                          y="Mean Number of Rounds",  # y variable name
                          data=df,               # dataframe to plot
                          ax=ax, color=color,
                          label=title)

    
    sns.set_style("darkgrid")
    sns.set(rc={'axes.facecolor': '#f7f7f7', 'figure.facecolor': 'white'})

    fig, axes = plt.subplots(figsize=(12, 8), sharex=True, sharey=True)
    plot_single("./Analysis/Data/degeneracyTest_results_6bits_2mermaids.csv",
                "6 bits, 1 mermaid", axes, "red")
    plot_single("./Analysis/Data/degeneracyTest_results_7bits_2mermaids.csv",
                "7 bits, 1 mermaid", axes, "green")
    plot_single("./Analysis/Data/degeneracyTest_results_8bits_2mermaids.csv",
                "8 bits, 1 mermaid", axes, "blue")
    plt.show()

# plotMultiple_v2()

def plotTiming():
    df = pd.read_csv("./Analysis/Data/degeneracyTest_minMax_results.csv")

    df = df[["Number of Rounds", "Game Time"]].groupby(["Number of Rounds"]).mean()
    print(df)
    fig, axes = plt.subplots(nrows=1, ncols=1, figsize=(10, 5), sharex=True, sharey=True)
    df.plot(ax=axes)
    # axes.set_xticklabels(df.index, rotation=90)
    plt.show()

plotTiming()
