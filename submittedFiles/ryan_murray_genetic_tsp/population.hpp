#include "chromosome.hpp"

class Population {
    private:
        int numIterations = 0;
        int popSize = 0;
        vector<Chromosome*> chromosomes;

        vector<int> fitnesses;
        void genFitnesses();
        Chromosome* pickParent();
        void destroyPop();

    public:
        Population(int, int, vector<int>, vector<int>);
        ~Population();
        Chromosome* getFittest();
        void nextGen();
        long getPopFitness();
};
