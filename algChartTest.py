import pandas as pd
import numpy as np
from matplotlib import pyplot as plt
import re
import argparse



argParser = argparse.ArgumentParser()
argParser.add_argument("-f","--file",dest="filename",help="csv to make a chart out of",type=str)
args = argParser.parse_args()


match = re.match(r"\w*(?=\d+\.csv)",args.filename.split("/")[-1])
assert match!=None, "can't find param name"
param = match.group()
print(param)


df = pd.read_csv(args.filename)

data = [ [ float(i) for i in str(row).split(";")] for row in df[param] if row != df[param][0]]


df = pd.DataFrame(data,columns=[i for i in str(df[param][0]).split(";")])

# df["number"] = [200,400,600,800,1000]

barWidth = 0.25


figure,ax=plt.subplots()
plt.xticks(df["number"])
ax.bar(x=df['number'],height=df['chaoticAnt'], width=barWidth)
ax.bar(x=df['number']+barWidth,height=df['pickyAnt'], width=barWidth)
ax.legend(labels=df.columns[1:])
ax.set_ylabel("mean levenshtein distance", fontsize=20)
ax.set_xlabel(param, fontsize=20)

plt.show()
