from random import randrange

f = open("complete_graph", "w")

for i in range(0, 999):
    for j in range(i + 1, 1000):
        w = randrange(1, 100)
        f.write(f"{i} {j} {w}\n")
    
f.close()