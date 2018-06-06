import math
#fileName = raw_input("Enter the name of the file to open: ");
with open("test.txt", "r") as f:
    coors = []
    for line in f:
        line = line.split()
        if line:
            line = [int(i) for i in line]
            coors.append(line)

def calcDist(a,b):
    d = int(math.sqrt(math.pow((a[1] - b[1]), 2) +
        math.pow((a[2] - b[2]), 2)))
    return d;
dist = []
dist.append(calcDist(coors[0], coors[1]))
dist.append(calcDist(coors[1], coors[2]))
dist.append(calcDist(coors[2], coors[0]))

total = 0
for i in dist:
    total +=i

print(" ".join(map(str,dist)))
