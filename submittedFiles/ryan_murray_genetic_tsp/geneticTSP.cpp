#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "population.hpp"

using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;
using std::vector;

int main(int argc, char** argv) {
    if (argc < 1) {
        cout << "Error: Missing file argument" << endl;
    }
    string filename = argv[1];
    ifstream file;
    file.open(filename, ifstream::in);

    vector<int> cities;
    vector<int> xCoords;
    vector<int> yCoords;

    int city;
    int xCoord;
    int yCoord;
    while (file >> city) {
        file >> xCoord >> yCoord;
        
        cities.push_back(city);
        xCoords.push_back(xCoord);
        yCoords.push_back(yCoord);
    }

    int numCities = cities.size();

    Population pop = Population(50, numCities, xCoords, yCoords);
    for (int i = 0; i < 1000; i++) {
        cout << "Current avg fitness: " << pop.getPopFitness() << endl;
        pop.nextGen();
    }

    string outFilename = filename + ".tour";
    ofstream outFile;
    outFile.open(outFilename, ofstream::out | ofstream::app);

    Chromosome* best = pop.getFittest();
    vector<int> bestGenes = best->getGenes();
    cout << "Best fitness: " << best->getFitness() << endl;
    outFile << best->getFitness() << endl;
    cout << "Genes: ";
    for (int i = 0; i < bestGenes.size(); i++) {
        cout << bestGenes.at(i) << " ";
        outFile << bestGenes.at(i) << endl;
    }
    cout << endl;

    outFile.close();

    /*
    for (int i = 0; i < cities.size(); i++) {
        cout << "City " << cities[i];
        cout <<  ": x = " << xCoords[i];
        cout << ", y = " << yCoords[i] << endl;
    }
    */
}
