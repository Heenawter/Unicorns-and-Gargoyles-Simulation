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


df = pd.read_csv("./Analysis/Data/consistencyTest.csv")
df = df[["Seed", "End Result", "Number of Rounds"]].groupby(["Seed", "End Result"]).count()

print(df.unstack().fillna(0))

# sns.barplot(data=df.unstack())
# plt.show()

