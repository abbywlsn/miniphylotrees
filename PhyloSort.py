from sklearn import preprocessing
import numpy as np
import csv

with open('NullModel_Data/NullModel_10/NullModelResults_10.csv', 'r', newline='') as csv_file:
    reader = csv.reader(line.replace('  ', ',') for line in csv_file)
    my_list = list(reader)
    x = my_list
    data_array = []

    for a_list in my_list:
        floatlist = [float(i) for i in a_list]
        floatlist.sort()
        data_array.append(floatlist)

        # every_10th_element = floatlist[::10]
        # data_array.append(every_10th_element)

    normalized = preprocessing.normalize(data_array)

    print("Normalized Data = ", normalized)

    with open('NullModel_Data/NullModel_10/NullModelSorted.csv',
              'w') as tree_percentiles:

            tree_percentiles.write(normalized + '\n')