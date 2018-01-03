/******************************************************************************
 * Author: Aaron Berns
 * Date: 11.20.17
 * Description: Implements Christofides' approximation algorithm for TSP
 * *****************************************************************************/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <ios>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <ctime>
#include <cstdlib>
#include <limits>
using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::map;
using std::fixed;
using std::showpoint;

// holds edge data
struct edge {
    int vNum;
    int dist;
};

// holds city data
struct city{
    int num;
    int x;
    int y;
    vector<edge> eList;
    int key;
    int p;
    bool inQ;
    int deg;
    int match;
    int matchEdge;
};

// time function from cs261
double getMilliseconds() {
    return 1000.0 * clock() / CLOCKS_PER_SEC;
}

// finds MST by Prim's method 
void MSTPrim(vector<city> &G) {
    
    // build Q to hold vertex numbers of G
    vector<int> Q;
    int temp;

    // initialze data
    for (int i = 0; i < G.size(); ++i) {
        G[i].inQ = true;
        G[i].key = std::numeric_limits<int>::max();
        Q.push_back(G[i].num);
    }

    // initialize root
    G[0].key = 0;
    G[0].p = 0;

    // form tree
    int minIndex, minKey;
    while (Q.size() != 0) {

        // find min
        minIndex = 0;
        minKey = std::numeric_limits<int>::max();
        for (int i = 0; i < Q.size(); ++i) {
            if (G[Q[i]].key < minKey) {
                minIndex = i;
                minKey = G[Q[i]].key;
            }
        }
        
        for (int i = 0; i < G[Q[minIndex]].eList.size(); ++i) {

            // if v in Q and edge weight < key
            if (G[G[Q[minIndex]].eList[i].vNum].inQ == true && G[Q[minIndex]].eList[i].dist < G[G[Q[minIndex]].eList[i].vNum].key) {

                // p[v] = u
                G[G[Q[minIndex]].eList[i].vNum].p = G[Q[minIndex]].num;

                // key[v] = w(u,v)
                G[G[Q[minIndex]].eList[i].vNum].key = G[Q[minIndex]].eList[i].dist;
            }
        }
        G[Q[minIndex]].inQ = false;
        Q.erase(Q.begin() + minIndex);
    }
}

// find vertices with odd degree 
void findOddDeg(vector<city> &G) {
    // initialize deg
    for (int i = 0; i < G.size(); ++i) {
        G[i].deg = 1;
    }

    // sum degs of children for each p
    for (int i = 0; i < G.size(); ++i) {
        G[G[i].p].deg++;
    }

    // remove 0 p of itself
    G[0].deg -= 2;
}

// minimum weight matching 
void match(vector<city> &G) {
        
    // create vector of odd degree vertices
    vector<int> odds;
    for (int i = 0; i < G.size(); ++i) {
        if (G[i].deg % 2 != 0) {
            odds.push_back(G[i].num);    
            G[i].match = -1;
        }
        else {
            G[i].match = -2;
        }
    }

    // re-order odd vertices by max possible matching edge weight in descending
    // order
    int min;
    vector<edge> oddsMinMax;
    struct edge temp;
    for (int i = 0; i < odds.size(); ++i) {
        min = -1;
        temp.vNum = odds[i];
        if (G[odds[i]].match == -1) {
            for (int j = 0; j < G[odds[i]].eList.size(); ++j) {
                if (G[G[odds[i]].eList[j].vNum].match == -1 && G[odds[i]].p != G[odds[i]].eList[j].vNum && G[G[odds[i]].eList[j].vNum].p != G[odds[i]].num && G[odds[i]].eList[j].dist > min) {
                    min = G[odds[i]].eList[j].dist;  
                }
            }
        }
        temp.dist = min;
        oddsMinMax.push_back(temp);;
    }

    // reload odds with new order
    odds.clear();
    int relMax;
    int maxV;
    int maxIndex;
    while (oddsMinMax.size() != 0) {
        relMax = -1;
        for (int j = 0; j < oddsMinMax.size(); ++j) {
            if (oddsMinMax[j].dist > relMax) {
                relMax = oddsMinMax[j].dist;
                maxV = oddsMinMax[j].vNum;
                maxIndex = j;
            }
        }
        odds.push_back(maxV);
        oddsMinMax.erase(oddsMinMax.begin() + maxIndex);
    }

    // match odd degree vertices
    int minMatch; 
    int possMatch;
    for (int i = 0; i < odds.size(); ++i) {
        minMatch = std::numeric_limits<int>::max();
        if (G[odds[i]].match == -1) {
            for (int j = 0; j < G[odds[i]].eList.size(); ++j) {
                if (G[G[odds[i]].eList[j].vNum].match == -1 && G[odds[i]].p != G[odds[i]].eList[j].vNum && G[G[odds[i]].eList[j].vNum].p != G[odds[i]].num && G[odds[i]].eList[j].dist < minMatch) {
                    minMatch = G[odds[i]].eList[j].dist;  
                    possMatch = G[odds[i]].eList[j].vNum;
                }
            }
            G[odds[i]].match = possMatch;
            G[odds[i]].matchEdge = minMatch;
            G[possMatch].match = G[odds[i]].num;
            G[possMatch].matchEdge = minMatch;
        }
    }
}

// Eulerian multi-graph from MST and matching 
vector<city> multiGraph(vector<city> E) {

    // clear out edge lists
    for (int i = 0; i < E.size(); ++i) {
        E[i].eList.clear();
    }

    // add edges from MST
    struct edge newEdge;
    for (int i = 1; i < E.size(); ++i) {
        newEdge.vNum = E[i].p;
        newEdge.dist = E[i].key;
        E[i].eList.push_back(newEdge);
        newEdge.vNum = E[i].num;
        E[E[i].p].eList.push_back(newEdge);
    }

    // add edges from matching
    for (int i = 0; i < E.size(); ++i) {
        if (E[i].match != -2) {
            newEdge.vNum = E[i].match;
            newEdge.dist = E[i].matchEdge;
            E[i].eList.push_back(newEdge);
        }
    }
    
    return E;
}

// Euler tour, help from "http://www.graph-magics.com/articles/euler.php" 
vector<int> eulerTour(vector<city> &G, int s) {

    // stack and circuit
    vector<int> eStack;
    vector<int> eCircuit;
    int vCurrent = s;
    int vNext;

    // form tour
    while (G[vCurrent].eList.size() != 0 || eStack.size() != 0) {
        
        // vCurrent has neighbors
        if (G[vCurrent].eList.size() != 0) {
            eStack.push_back(vCurrent);
            vNext = G[vCurrent].eList[G[vCurrent].eList.size() -1].vNum;
            G[vCurrent].eList.pop_back();

            // pop edge from adjacent vertex's edge list
            for (int i = 0; i < G[vNext].eList.size(); ++i) {
                if (G[vNext].eList[i].vNum == vCurrent) {
                    G[vNext].eList.erase(G[vNext].eList.begin() + i);
                }
            }
            vCurrent = vNext;
        }

        // no neighbors
        else {
            eCircuit.push_back(vCurrent);
            vCurrent = eStack.back();
            eStack.pop_back();
        }
    }
    
    return eCircuit;
}

// shortcut euler tour by removing duplicate edges 
// help from "https://stackoverflow.com/questions/8353554/how-to-implement-the-shortcutting-step-in-the-christofides-algorithm"
vector<int> shortcut(vector<int> tour) {

    vector<int> tsp;
    map<int, bool> dupMap;

    // add vertices, ignoring those already visited
    for (int i = 0; i < tour.size(); ++i) {
        if (dupMap.find(tour[i]) == dupMap.end()) {
            dupMap.insert(std::pair<int,bool>(tour[i], true));
            tsp.push_back(tour[i]);
        }
    }

    // add end
    tsp.push_back(tour[0]);

    return tsp;
}

// add edges along route O(n^2)
int calcLength(vector<city> &G, vector<int> route) {
    
    int rLength = 0;

    // find edges in tour and add distances
    for (int i = 0; i < route.size() -1; ++i) {
        for (int j = 0; j < G[route[i]].eList.size(); ++j) {
            if (G[route[i]].eList[j].vNum == route[i+1]) {
                rLength += G[route[i]].eList[j].dist;
                
            }
        }
    }

    for (int j = 0; j < G[route[route.size() -1]].eList.size(); ++j) {
        if (G[route[route.size() -1]].eList[j].vNum == route[0]) {
            rLength += G[route[route.size() -1]].eList[j].dist;
        }
    }
    
    return rLength;
}

int main ()
{
    // seed random generator
    unsigned seed = time(0);
    srand(seed);

    vector<city> cGraph;       
    struct city inputCity;
    string fileName;
    int value;
    string line;
    struct edge e;
    double dDist;
    int tourBeg = 0;


    // read in file name
    cout << "Please enter the name of the file to open: ";
    cin >> fileName;

    // open files
    ifstream inFile;
    inFile.open(fileName);
 
    // rename and open output file
    fileName += ".tour";
    //fileName = "out.txt";
    ofstream outFile;
    outFile.open(fileName, std::ios::out | std::ios::app);
    
    while (!inFile.eof()) {

        // read entire file
        getline(inFile, line); 
        
        // read and store line by line
        istringstream is(line);
        is >> inputCity.num;
        is >> inputCity.x;
        is >> inputCity.y;

        // push city in to cGraph vector
        cGraph.push_back(inputCity);
    }

    // remove duplicate value in last position if space at end of input
    // file creates one
    if (cGraph[cGraph.size() -1].num == cGraph[cGraph.size() -2].num) {
        cGraph.pop_back();
    }

    //get initial time
    double t0 = getMilliseconds();

    // create graph by calculating edges, storing in edge list
    for (int i = 0; i < cGraph.size(); ++i) {
        for (int j = i+1; j < cGraph.size(); ++j) {
            dDist = round(sqrt(((cGraph[i].x - cGraph[j].x) * (cGraph[i].x - cGraph[j].x)) + ((cGraph[i].y - cGraph[j].y) * (cGraph[i].y - cGraph[j].y))));
            e.dist = dDist;
            e.vNum = j;
            cGraph[i].eList.push_back(e);
            e.vNum = i;
            cGraph[j].eList.push_back(e);
        }
    }

    // find MST using Prim's algorithm:
    MSTPrim(cGraph);

    // find vertices with odd degree
    findOddDeg(cGraph);

    // minimum weight matching subgraph
    match(cGraph);

    // construct Eulerian multi-graph from MST and M
    vector<city> eGraph = multiGraph(cGraph);

    // Euler tour
    vector<int> tour = eulerTour(eGraph, tourBeg);

    // shortcut
    vector<int> tspRoute = shortcut(tour);

    // calculate length of route
    int rLength = calcLength(cGraph, tspRoute);

    // get time
    double t1 = getMilliseconds();
    
    // output time and distance to console
    cout << showpoint << fixed << "Time: " << t1 - t0 << " milliseconds" << endl;
    cout << "Distance: " << rLength << endl;

    //write to output file
    outFile << rLength << endl;
    for (int i = 0; i < tspRoute.size() -1; ++i) {
        outFile << tspRoute[i] << endl;
    }
    outFile << endl;

    // close files
    inFile.close();
    outFile.close();

    return 0;
}
