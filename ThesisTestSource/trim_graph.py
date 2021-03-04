from random import randrange
from numpy import genfromtxt
import numpy as np

f = 'complete_graph'

data = genfromtxt(f, delimiter = ' ')

out = []
for r in data:
    if np.random.randint(0,101) < 90:
        out.append(r)

np.savetxt('very_dense_graph', np.array(out).astype(int), fmt='%i', delimiter=' ')