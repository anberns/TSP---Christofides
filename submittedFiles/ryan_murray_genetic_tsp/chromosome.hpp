#include <vector>
#include <map>

using std::vector;

class Chromosome {
    private:
        int numGenes = 0;
        vector<int> genes; 
        vector<int> cityX;
        vector<int> cityY;
        double pCrossover = 0.8;
        double pMutate = 0.005;
        std::map<int, int> cityIdxs;

        Chromosome* crossover(Chromosome*, int);
        void mutate();
        void swapGenes(int, int);
    public:
        Chromosome(int, vector<int>, vector<int>);
        Chromosome(vector<int>, vector<int>, vector<int>);
        Chromosome(const Chromosome &c2);
        ~Chromosome();
        vector<Chromosome*> createChildren(Chromosome*);
        int getFitness();
        vector<int> getGenes();
};
