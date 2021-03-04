from random import randrange
from numpy import genfromtxt
import numpy as np

f = 'very_sparse_graph'

data = genfromtxt(f, delimiter = ' ')
unique = np.unique(np.delete(data, 2, 1), axis=0)

r,_ = unique.shape

r_unique = np.random.randint(1, 1000, size=(r,3))
r_unique[:,:-1] = unique

np.savetxt(f'{f}_unique', r_unique.astype(int), fmt='%i', delimiter=' ')