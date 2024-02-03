import pandas as pd
import numpy as np
from matplotlib import pyplot as plt
import re
from pathlib import Path
import os
import math
import argparse


argParser = argparse.ArgumentParser()
argParser.add_argument("-d","--directory",dest="dir",help="directory with csv-s to make a chart out of",type=str)
args = argParser.parse_args()

currPath = Path(__file__).parent
files = os.listdir(currPath / args.dir)
powerbase = 1
while(pow(powerbase,2) < len(files) ):
    powerbase+=1
charts_dim = ( )


figure,ax=plt.subplots(powerbase, math.ceil(len(files)/powerbase))

for chartNum, filename in enumerate(files):
    print(f'{chartNum // powerbase} {chartNum % powerbase}')

    
    match = re.match(r"\w*(?=\d+\.csv)",filename.split("/")[-1])

    assert match!=None, "can't find param name"
    param = match.group()


    df = pd.read_csv(currPath / args.dir / filename)

    data = [ [ float(i) for i in str(row).split(";")] for row in df[param] if row != df[param][0]]

    # print(data)

    df = pd.DataFrame(data,columns=[i for i in str(df[param][0]).split(";")])

    df["number"] = np.arange(len(df.index))

    barWidth = 0.25

    ax[chartNum // powerbase, chartNum % powerbase].bar(x=df['number'],height=df['chaoticAnt'], width=barWidth)
    ax[chartNum // powerbase, chartNum % powerbase].bar(x=df['number']+barWidth,height=df['pickyAnt'], width=barWidth)

    ax[chartNum // powerbase, chartNum % powerbase].legend(labels=df.columns[1:])
    ax[chartNum // powerbase, chartNum % powerbase].set_ylabel("mean levenshtein distance", fontsize=20)
    ax[chartNum // powerbase, chartNum % powerbase].set_xlabel(param, fontsize=20)

for chartNum in range(len(files), pow(powerbase,2)):
    # print(f'{chartNum // powerbase} {chartNum % powerbase}')
    ax[chartNum // powerbase, chartNum % powerbase].axis("off")
    
    

plt.show()
 

    



