import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from itertools import chain
import math

def getSeeds(df):
    uniqueSeeds = df[df["Player Type"] == "troll"]["Seed"].value_counts()
    f = open("./Analysis/Data/allSeeds.txt", "w")
    for seed in uniqueSeeds.index:
        f.write(str(seed) + "\n")
    f.close()

def runAnalysis(df):

    uniqueGoals = df[df["Player Type"] == "troll"]["Goal"].value_counts()
    # print("Unique Goals:", uniqueGoals)

    df = df[["Player Type", "Function", "Time"]].groupby(["Player Type", "Function"]).describe()
    
    print(df)

def analyseTaxCrab():
    slothFile = open("./Analysis/Data/FixedSeed_Results_Greedy.txt", "r")
    slothCardsRemoved = {}
    for line in slothFile:
        index = line.find("remove")
        if index != -1: # substring found
            removed = line[index + 7:index + 8]
            if(removed in slothCardsRemoved.keys()):
                slothCardsRemoved[removed] += 1
            else:
                slothCardsRemoved[removed] = 1
    slothFile.close()

    trollFile = open("./Analysis/Data/FixedSeed_Results_Troll.txt", "r")
    trollCardsRemoved = {}
    for line in trollFile:
        index = line.find("remove")
        if index != -1:  # substring found
            removed = line[index + 7:index + 8]
            if(removed in trollCardsRemoved.keys()):
                trollCardsRemoved[removed] += 1
            else:
                trollCardsRemoved[removed] = 1
    trollFile.close()

    print(slothCardsRemoved)
    total = 0
    for value in slothCardsRemoved.values():
        total += value
    print("Total: ", total)

    print(trollCardsRemoved)
    total = 0
    for value in trollCardsRemoved.values():
        total += value
    print("Total: ", total)


df = pd.read_csv("./Analysis/Data/speedTest_troll_taxCrab_results_optimized.csv")
print(df.describe())

df = pd.read_csv("./Analysis/Data/speedTest_troll_taxCrab_results_unoptimized.csv")
print(df.describe())

# runAnalysis(df)
# getSeeds(df)
# analyseTaxCrab()
