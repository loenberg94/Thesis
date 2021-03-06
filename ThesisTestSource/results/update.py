from numpy import genfromtxt
import numpy as np

oracles = [
    #"thorup",
    "wulff"
]

files = {
    #"complete_graph",
    #"super_dense_graph",
    #"very_dense_graph",
    #"dense_graph",
    #"semi_dense_graph",
    #"sparse_graph",
    #"very_sparse_graph",
    "real_world"
}

k_values = [
    3, 
    5, 
    8, 
    10, 
    13, 
    15, 
    18
    ]

for oracle in oracles:
    for graph in files:
        data = genfromtxt(f'{oracle}_{graph}_2.txt', delimiter=',')
        r, c = data.shape

        n = data

        for k in k_values:
            data = genfromtxt(f'{oracle}_{graph}_{k}.txt', delimiter=',')
            n = np.vstack((n, data))

        np.savetxt(f'{oracle}_{graph}_collected.csv', n, delimiter=',')
