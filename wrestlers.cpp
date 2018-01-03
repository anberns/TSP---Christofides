/******************************************************************************
 * Author: Aaron Berns
 * Date: 11.1.17
 * Description: Implements wrestling rivalry algorithm for question 5. BFS
 * function based on lecture pseudocode. Help with map and queue containers from
 * cpluplus.com. 
 * *****************************************************************************/

#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
using std::ifstream;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::queue;
using std::map;
using std::pair;

// holds edge data
struct edge {
    int vNum;
    int dist;

// holds city data
struct city{
    int num;
    int x;
    int y;
    vector<edge> eList;
};

// performs breadth-first search of array of wrestlers
void BFS(wrestler *array, map<string, int> idMap, int s) {
    string u, v;
    int uIndex, vIndex;
    queue<string> Q;
    array[s].dist = 0;
    Q.push(array[s].name);
    while (Q.size() > 0) {
        u = Q.front();
        Q.pop();
        uIndex = idMap.find(u)->second;
        for (int i = 0; i < array[uIndex].rivals.size(); ++i) {
            v = array[uIndex].rivals[i];
            vIndex = idMap.find(v)->second;
            if (array[vIndex].color == 'w') {
                array[vIndex].color = 'g';
                array[vIndex].dist = array[uIndex].dist + 1; 
                array[vIndex].parent = u;
                Q.push(array[vIndex].name);
            }
        }
        array[uIndex].color = 'b';
    }
}

// checks compatibility of wrestler matchups
bool rivalStatus(wrestler *arr, map<string, int> idMap, int num) {
    bool evenDist;  // flag for even or odd distance of wrestler
    bool testDist;  // flag for rival distance
    string v;
    int vIndex;
    for (int i = 0; i < num; ++i) {
        if (arr[i].dist % 2 == 0) {
            evenDist = true;
        }
        else {
            evenDist = false;
        }
        for (int j = 0; j < arr[i].rivals.size(); ++j) {
            v = arr[i].rivals[j];
            vIndex = idMap.find(v)->second;
            if (arr[vIndex].dist % 2 == 0) {
                testDist = true;
            }
            else {
                testDist = false;
            }
            if (evenDist == testDist) {
                return false;
            }
        }
    }
    return true;
}

int main ()
{
    vector<city> cList;       
    string fileName;
    string wName;
    string wName2;

    /* read in file name
    cout << "Please enter the name of the file to open: ";
    cin >> fileName;
    */

    // open files
    ifstream inFile;
    inFile.open("tsp_example_1.txt");
    
    // read in wrestler name data, initialize wrestler structs
    for (int i = 0; i < numNames; ++i) {
        inFile >> wName;
        idMap.insert(pair<string, int>(wName, i));
        array[i].name = wName;
        array[i].color = 'w';
    }

    // read in rivalries and store rivals as edge list
    inFile >> numRiv;
    int index;
    for (int i = 0; i < numRiv; ++i) {
        inFile >> wName >> wName2;
        index = idMap.find(wName)->second;
        array[index].rivals.push_back(wName2);
        index = idMap.find(wName2)->second;
        array[index].rivals.push_back(wName);
    }

    // BFS to label wrestlers as Babyface or Heel
    BFS(array, idMap, 0);
        
    // check rivalries, print to terminal
    if (rivalStatus(array, idMap, numNames)) {
        cout << "Yes" << endl;
        cout << "Babyfaces: ";
        for (int i = 0; i < numNames; ++i) {
            if (array[i].dist % 2 == 0) {
                cout << array[i].name << " ";
            }
        }
        cout << endl;
        cout << "Heels: ";
        for (int i = 0; i < numNames; ++i) {
            if (array[i].dist % 2 != 0) {
                cout << array[i].name << " ";
            }
        }
        cout << endl;
    }
    else {
        cout << "No" << endl;
    }

    // free array
    delete [] array;

    // close files
    inFile.close();
    return 0;
}


