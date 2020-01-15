import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("../GameInfo/results.csv")

meanRoundsPerGoal = df[["Goal", "Number of Rounds"]].groupby("Goal").mean().reset_index()


print(meanRoundsPerGoal)
