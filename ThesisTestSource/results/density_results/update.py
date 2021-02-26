from numpy import genfromtxt
import numpy as np

k_values = [3, 5, 8, 10, 13, 15, 18]

pre = "wulff_real_world_"

data = genfromtxt(f'{pre}2.txt', delimiter=',')
r, c = data.shape

n = np.zeros((r, c + 1))
n[:,c] = 2
n[:,:-1] = data 


for k in k_values:
    data = genfromtxt(f'{pre}{k}.txt', delimiter=',')
    t = np.zeros((r, c + 1))
    t[:,c] = k
    t[:,:-1] = data 
    n = np.vstack((n, t))

np.savetxt(f'{pre}collected.csv', n, delimiter=',')
