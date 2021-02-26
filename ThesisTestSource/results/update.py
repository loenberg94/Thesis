from numpy import genfromtxt
import numpy as np

k_values = [3, 5, 8, 13]

data = genfromtxt('thorup_complete_2.txt', delimiter=',')
r, c = data.shape

n = np.zeros((r, c + 1))
n[:,c] = 2
n[:,:-1] = data 

n.

for k in k_values:
    data = genfromtxt(f'thorup_complete_{k}.txt', delimiter=',')
    t = np.zeros((r, c + 1))
    t[:,c] = k
    t[:,:-1] = data 
    n = np.vstack((n, t))

np.savetxt('thorup_complete_collected.csv', n, delimiter=',')
