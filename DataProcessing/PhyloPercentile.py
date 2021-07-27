import numpy as np
import csv
import pandas as pd
# treedata_array = np.genfromtxt("../../../Empirical/source/Evolve/miniphylotrees/SingleGenClade.csv", delimiter=",")
# print(treedata_array)
#
# with open('../../../Empirical/source/Evolve/miniphylotrees/TenSingleGenClade.csv', 'w') as tree_percentiles:
#     for i in range(101):
#         print(i, np.percentile(treedata_array, i))
#
#         tree_percentiles.write(str(i) + "," + " " + str(np.percentile(treedata_array, i)) + '\n')

data_array = []
df = pd.read_csv('../NullModel/NullModel_Data/NullModel_100/100_NoComma.csv', header=None)
df = df.T
newdf = pd.DataFrame(np.sort(df.values, axis=0), index=df.index, columns=df.columns)
df = newdf.T

dfeveryten = df[df.columns[::10]]

print(dfeveryten)

dfeveryten.to_csv('../NullModel/NullModel_Data/NullModel_100/Percentile_100.csv', header=False,
          index=False) 