#include "chromosome.hpp"
#include <random>
#include <time.h>
#include <cstdlib>
#include <math.h>
#include <iostream>

using std::rand;
using std::srand;
using std::vector;
using std::cout;
using std::endl;

/*
 * Constructor that randomly generates a new chromosome
 */
Chromosome::Chromosome(int numCities, vector<int> xCoords, vector<int> yCoords) {
    numGenes = numCities;
    cityX = xCoords;
    cityY = yCoords;
    for (int i = 0; i < numCities; i++) {
        genes.push_back(i);
        cityIdxs.insert(std::pair<int, int>(i, i));
    }
    // Shuffle genes
    for (int i = numCities - 1; i >= 0; i--) {
        int idx = rand() % numCities;
        swapGenes(i, idx);
    }
}


/*
 * Constructor that creates a chromosome from an array representation.
 */
Chromosome::Chromosome(vector<int> refGenes, vector<int>xCoords, vector<int>yCoords) {
    genes = refGenes;
    cityX = xCoords;
    cityY = yCoords;
    numGenes = genes.size();
}


Chromosome::Chromosome(const Chromosome &c2) {
    genes = c2.genes;
    cityX = c2.cityX;
    cityY = c2.cityY;
    numGenes = c2.numGenes;
}


Chromosome::~Chromosome() {
}


/*
 * Creates a new chromosome from using another parent chromosome.
 * PUT THIS IN POPULATION.CPP
 */
vector<Chromosome*> Chromosome::createChildren(Chromosome* parent2) {
    /* Credit for randomization code:
     * https://stackoverflow.com/questions/1340729/how-do-you-generate-a-random-double-uniformly-distributed-between-0-and-1-from-c
     */
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<> dist(0, 1);

    double prob = dist(engine);

    Chromosome* child1;
    Chromosome* child2;

    if (prob <= pCrossover) {
        int xoverPt = rand() % numGenes + 1;
        child1 = crossover(parent2, xoverPt);
        child2 = parent2->crossover(this, xoverPt);
    } else {
        child1 = new Chromosome(*this);
        child2 = new Chromosome(*parent2);
    }

    double probMutC1 = dist(engine);
    double probMutC2 = dist(engine);
    if (probMutC1 <= pMutate) {
        child1->mutate();
    }
    if (probMutC2 <= pMutate) {
        child2->mutate();
    }

    vector<Chromosome*> children;
    children.push_back(child1);
    children.push_back(child2);
    return children;
}


/*
 * Uses the PMX method to exchange genes between parents
 */
Chromosome* Chromosome::crossover(Chromosome* parent2, int xoverPt) {
    Chromosome* child = new Chromosome(*this);
    vector<int> p2Genes = parent2->getGenes();
    /* If a crossover occurs then at least one gene must be swapped so we set
     * the minimum crossover point to be 1
     */

    for (int i = xoverPt; i < numGenes; i++) {
        int allele = p2Genes[i];
        int pos = cityIdxs[allele];
        child->swapGenes(i, pos);
    }

    return child;
}


void Chromosome::mutate() {
    int gene1 = rand() % numGenes;
    int gene2;
    do {
        gene2 = rand() % numGenes;
    } while (gene2 == gene1);

    swapGenes(gene1, gene2);
}


/*
 * Returns a fitness value for the chromosome
 */
int Chromosome::getFitness(){
    int dist = 0;
    // Get distance between last and first city
    int x1 = cityX[genes[numGenes - 1]];
    int x2 = cityX[genes[0]];
    int y1 = cityY[genes[numGenes - 1]];
    int y2 = cityY[genes[0]];
    dist += round(sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2)));

    // Get distances for rest of route
    for (int i = 1; i < numGenes; i++) {
        x1 = cityX[genes[i - 1]];
        x2 = cityX[genes[i]];
        y1 = cityY[genes[i - 1]];
        y2 = cityY[genes[i]];
        dist += round(sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2)));
    }

    return dist;
}


/*
 * Returns a copy of the chromosome's value
 */
vector<int> Chromosome::getGenes() {
    return genes;
}


/*
 * Swaps two genes in the chromosome and updates their known positions
 */
void Chromosome::swapGenes(int p1, int p2) {
    int temp = genes[p1];
    genes[p1] = genes[p2];
    genes[p2] = temp;
    // Update locations of cities in vector
    cityIdxs[genes[p1]] = p1;
    cityIdxs[genes[p2]] = p2;
}
