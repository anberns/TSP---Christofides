import sys
import math
import time
#code is influenced and inspired by this sites java implementation.
#http://www.technical-recipes.com/2017/applying-the-2-opt-algorithm-to-traveling-salesman-problems-in-java/
#as well as this site
#http://on-demand.gputechconf.com/gtc/2014/presentations/S4534-high-speed-2-opt-tsp-solver.pdf

#Create city class. TO store city data
class City:
    def __init__(city, number, xCoord, yCoord):
        city.cityName = number
        city.x = xCoord
        city.y = yCoord

#http://www.technical-recipes.com/2017/applying-the-2-opt-algorithm-to-traveling-salesman-problems-in-java/
#for finding distance algorithm
#distance between two cities. Gets called all the time
def distance(x1,y1,x2,y2):
    distanceX = x2 - x1
    distanceY = y2 - y1
    twoCitiesDistance = int(round(math.sqrt(distanceX*distanceX + distanceY*distanceY)))
    return twoCitiesDistance


 #shoutout to this sites slideshow for 2opt code. I based mine off of the pseudo code found there.
 ##http://on-demand.gputechconf.com/gtc/2014/presentations/S4534-high-speed-2-opt-tsp-solver.pdf
def twoOPT(cityArray, tour):
    minchange = -1 #minchange variable tracks wether or not change has occured.
    while minchange <0:
        minchange = 0
        swap = False
        #cycles through the tour
       
        for i in range(len(tour)-2):
            for j in range(i+2, len(tour)-1):
                #change gets the result of the calculation (distance between node i to node J + the distance between the node after i and j 
                #then subtract the distance between node i and the node after it + the distance between J and the node after it)
                #if the value in change is less than that in minchange we perform a swap. if value of change is smaller than
                #minchange we have a more efficient route to switch to.
                change = distance(cityArray[tour[i]].x, cityArray[tour[i]].y, cityArray[tour[j]].x, cityArray[tour[j]].y)+ distance(cityArray[tour[i+1]].x, cityArray[tour[i+1]].y, cityArray[tour[j+1]].x, cityArray[tour[j+1]].y) - distance(cityArray[tour[i]].x, cityArray[tour[i]].y, cityArray[tour[i+1]].x, cityArray[tour[i+1]].y)- distance(cityArray[tour[j]].x, cityArray[tour[j]].y, cityArray[tour[j+1]].x, cityArray[tour[j+1]].y)
                if minchange > change: #if true aka 1 we must swap the paths
                    swap = 1
                    minchange = change
                    minI = i
                    minJ = j
            #simple swap algo 
        if swap == 1:
            temp = tour[minJ]
            tour[minJ] = tour[minI+1]
            tour[minI+1] = temp
       
    return tour

#based on the algorithm presented in the assignment prompt.
def tourDistance(cityArray, tour):
        dist = 0
        for i in range(len(cityArray)-1): 
            dist += distance(cityArray[tour[i]].x,cityArray[tour[i]].y,cityArray[tour[i+1]].x, cityArray[tour[i+1]].y)
            #find the distance from the first to last city and add it to total distance for a complete distance measurement of the tour.
        dist += distance(cityArray[tour[0]].x, cityArray[tour[0]].y, cityArray[tour[-1]].x, cityArray[tour[-1]].y) 
        return dist 


def main():
    fileName = sys.argv[1]
    file = fileName + '.txt'
    with open(file) as data:
        cityArray = []
        for line in data:
            line = line.split() # this gets fixes issues with blank
            if line:            #if there is a value in the line, put it in the array
                cityInfo = []
                for value in line:
                    cityInfo.append(value) # put each value into city info
                    #the city identifyer, x coord and y coord
                city = City(int(cityInfo[0]), int(cityInfo[1]), int(cityInfo[2])) #create a new object and apply the details we recieve from txt file
                cityArray.append(city)  #append the object to the array of cities

    start = time.perf_counter() 

    #create tours for possible start points
    #builds a tour of cities array based on nearest neighbor.
    for p in range(len(cityArray)):  

        tourArray = [p] 
        x = 0
        while x != len(cityArray)-1: 
            i = tourArray[-1]
            lastDistance = float('inf')
            for j in cityArray:
                #if city is not in the tour, find the distance to the city
                #if the distance is less than or equal to the most recent distance
                #make that distance most recent and make j.cityname the current city to travel from next
                #this makes sure we only travel to the next nearest neighbor.
                if j.cityName not in tourArray:
                    d = distance(cityArray[i].x, cityArray[i].y, j.x, j.y)
                    if d <= lastDistance:
                        lastDistance = d
                        currentCity = j.cityName 
            tourArray.append(currentCity)
            x += 1
 #call twoOPT as tour parameter for function because that gives us the optimal tour to work with
    totalDistance = tourDistance(cityArray, twoOPT(cityArray, tourArray))

    end = time.perf_counter()
    timeElapsed = end - start 
    print(timeElapsed) #prints total time elapsed so we can track it for comp

    #for file output, we just print a list from the optimal tour of the city array
    #starting with the total distance on line 0
    file = fileName + '.txt' + '.tour'
    with open(file, 'w') as outPutFile:
        outPutFile.write("%s" % totalDistance)
        outPutFile.write("\n")
        for val in twoOPT(cityArray, tourArray):
            num = str(val)
            outPutFile.write(num + "\n")

if __name__ == "__main__":
    main()