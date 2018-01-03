#include "population.hpp"
#include <random>
#include <algorithm>

Population::Population(int numChromosomes, int numCities, vector<int> xCoords, vector<int> yCoords) {
    srand(time(NULL));

    popSize = numChromosomes;
    for (int i = 0; i < popSize; i++) {
        chromosomes.push_back(new Chromosome(numCities, xCoords, yCoords));
    }
}


Population::~Population() {
    destroyPop();
}


/* Selects the fittest chromosome from the population */
Chromosome* Population::getFittest() {
    Chromosome* fittest = chromosomes.at(0);
    for (int i = 1; i < chromosomes.size(); i++) {
        if (chromosomes.at(i)->getFitness() < fittest->getFitness()) {
            fittest = chromosomes.at(i);
        }
    }

    return fittest;
}


/* Picks a parent chromosome based on the tournament method */
Chromosome* Population::pickParent() {
    if (fitnesses.empty()) {
        genFitnesses();
    }

    vector<Chromosome*> tnmnt;
    while (tnmnt.size() < 10) {
        int pick = rand() % chromosomes.size();
        tnmnt.push_back(chromosomes.at(pick));
    }

    Chromosome* best = tnmnt.at(0);
    for (int i = 1; i < tnmnt.size(); i++) {
        if (tnmnt.at(i)->getFitness() < best->getFitness()) {
            best = tnmnt.at(i);
        }
    }
    return best;

    /*
    int c1 = rand() % chromosomes.size();
    int c2;
    do {
        c2 = rand() % chromosomes.size();
    } while (c1 == c2);

    if (fitnesses.at(c1) < fitnesses.at(c2)) {
        return chromosomes.at(c1);
    } else {
        return chromosomes.at(c2);
    }
    */
}


/* Discovers the fitnesses of all chromosomes in the population */
void Population::genFitnesses() {
    for (int i = 0; i < chromosomes.size(); i++) {
        fitnesses.push_back(chromosomes.at(i)->getFitness());
    }
}


/* Creates a new generation */
void Population::nextGen() {
    vector<Chromosome*> newPop;
    while (newPop.size() < popSize) {
        Chromosome* parent1 = pickParent();
        Chromosome* parent2;
        do {
            parent2 = pickParent();
        } while (parent2 == parent1);
        vector<Chromosome*> children = parent1->createChildren(parent2);
        newPop.push_back(children[0]);
        newPop.push_back(children[1]);
    }
    
    destroyPop();
    chromosomes = newPop;
}


/* Frees memory of a generation */
void Population::destroyPop() {
    for (int i = 0; i < chromosomes.size(); i++) {
        delete chromosomes.at(i);
    }
    fitnesses.clear();
}

/* Gets the entire population's avg fitness */
long Population::getPopFitness() {
    if (fitnesses.size() < 1) {
        genFitnesses();
    }

    long sum = 0;
    for (int i = 0; i < fitnesses.size(); i++) {
        sum += fitnesses.at(i);
    }

    return sum / fitnesses.size();
}
