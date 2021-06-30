#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include "Evolve/Systematics.hpp"

//for i in {i..10}; do make DiversityTrees && ./a.out done

//g++ -std=c++17 -I../../ main.cpp -o main.o && ./main.o
//for i in {1..5}; do g++ -std=c++17 -I../../ main.cpp -o main.o && ./main.o; done
//for i in {1..100}; do ./main.o; done

/*This file creates trees with mutations and pressure for diversity (optional) for the systematics normalization project.
 * This program generates trees with a depth of 10 to 100 generations. Mutation occurs when a random number generted between
 * 0 and 1 is less than 0.05. When a mutation occurs, another random number is generated between -3 and 3. Whatever number is chosen
 * is then subtracted from the original genotype value. If the user chooses to use pressure for diversity, they must use the
 * ChooseOrgDiversity() function and the CalcFitness() function. The ChooseOrg() function should be left unused in this scenario.
 *
/*This file creates trees with mutations and pressure for diversity (optional) for the systematics normalization project.
 * This program generates a random tree with a depth of
 * 100 generations. This can be changed in the numGens variable. This tree has 10 organisms per generation and uses a clade
 * as the method of tracking in systematics.h. The program returns phylogenetic diversity after each generation and prints the
 * final phylogenetic diversity to a csv file of your choice. You will have to uncomment the writeToFile function and give it a
 * file path to use this function.
 * You can also use this tree to test the systematics.h FindPhyloData() function. You can uncomment the line sys.FindPhyloData() to
 * use this function of the program. */


using namespace std;

int numOrgs = 10;
int parentNum;
int numGens = 10;
double mutRate = 0.05;
int TenGens = 10;

class Organism {
public:
    //int clade;
    int genotype = 0;

    Organism() { //default constructor sets genotype to 0
    }

    Organism(int _genotype) { //this constructor sets genotype to the parent genotype
        genotype = _genotype;
    }

    int MutateGenotype(emp::Random &RandNum) {

        double randMutation = RandNum.GetDouble(0, 1);
        //cout << "random mutation rate = " << randMutation << endl;
        //cout << "GENOTYPE BEFORE MUTATION: " << genotype << endl;

        if (randMutation < mutRate) {
            int MutatedGenotype = genotype - RandNum.GetInt(-3, 3);
            genotype = MutatedGenotype;
            //cout << "mutated genotype = " << genotype << endl;
        } else {
            //cout << "not mutated genotype = " << genotype << endl;
        }
        //cout << "GENOTYPE: " << genotype << endl;

        return genotype;
    }
};

int chooseOrg(vector<Organism> &currentGen, emp::Random &randNum){

    parentNum = randNum.GetInt(size(currentGen));  //chooses random spot in array for parent
    //cout << "parent chosen is in spot " << parentNum << " in currentGen array which is " << size(currentGen) << " long" << endl;
    return parentNum;
}


void calcFitness(vector<Organism> &currentGen, vector<double> &fitnessVect, emp::Random &randNum) {
    fitnessVect.resize(0);

    vector<int> fitnessCalc;

    fitnessCalc.reserve(currentGen.size());
    for (int i = 0; i < currentGen.size(); i++) {
        fitnessCalc.push_back(currentGen[i].genotype);
    }

    map<int, int> CountMap;

    for (int j = 0; j < fitnessCalc.size(); j++) {
        if (emp::Has(CountMap, fitnessCalc[j])) {
            CountMap[fitnessCalc[j]]++;
        } else {
            CountMap[fitnessCalc[j]] = 1;

        }
    }

    for(int k = 0; k < fitnessCalc.size(); k++){
        fitnessVect.push_back(1.0/CountMap[fitnessCalc[k]]);
    }
}


int chooseOrgDiversity(vector<double> &fitnessVect, emp::Random &randNum){
    emp::IndexMap fitness_index(fitnessVect.size());

    for (size_t id = 0; id < fitnessVect.size(); id++){
        fitness_index.Adjust(id, fitnessVect[id]);
    }

    const double fit_pos = randNum.GetDouble(fitness_index.GetWeight());
    size_t parent_id = fitness_index.Index(fit_pos);

    //cout << "FITNESS VECTOR VALUES: " << endl;

    for(int pos = 0; pos < fitnessVect.size(); pos++){
        //cout << fitnessVect[pos] << " " << endl;
    }

    parentNum = parent_id;

    //cout << "PARENT NUM AFTER CHOOSEORGDIVERSITY: " << parentNum << endl;
    //cout << "fitness val at parent_id: " << fitnessVect[parent_id] << endl;

    return parentNum;
}


void switchGens(vector<Organism> &currentGen, vector<Organism> &childGen, emp::Systematics<Organism, int> &sys){
    currentGen.swap(childGen);
    childGen.clear();
    sys.Update();
}

bool writeToFile(string filename, double field_one);


int main() {
    emp::Random randNum;

    //emp::Random randMut;

    function<int(Organism)> taxonFunc = [](Organism org){return org.genotype;};

    //function<int(int)> square = [](int squaredNum){return (squaredNum*squaredNum);};

    emp::Systematics<Organism, int> sys(taxonFunc); //optional 3rd arg
    sys.SetTrackSynchronous(true);

    //current gen (vector)
    vector<Organism> currentGen; //begins with currentGen
    //child gen (vector)
    vector<Organism> childGen;

    vector<double> fitnessVect;


    for (int i = 0; i < numOrgs; i++) {
        currentGen.emplace_back(); //currentGen is filled with 10 organism
        sys.AddOrg(currentGen[i], i); //parent is null (removed brackets)
    }

//    for(int i = 0; i < currentGen.size(); i++){
//        cout << currentGen[i] . printVect() << " " << endl;
//    }

    for (int i = 0; i < numGens; i++) {
        cout << "generation: " << i << endl;
        //assert(currentGen.size() == 10);

        calcFitness(currentGen, fitnessVect,randNum);


        for(int r = 0; r < numOrgs; r++){

            //chooseOrg(currentGen, randNum);
            chooseOrgDiversity(fitnessVect, randNum); //chooses the parent of the next generation
            //cout << "parent: " << parentNum << endl;
            sys.SetNextParent(parentNum);
            //currentGen[parentNum].reproduce(childGen, sys); //fills childGen with 10 Organisms

            childGen.emplace_back(currentGen[parentNum].genotype); //fills childGen vector with Organisms
            childGen[r].MutateGenotype(randNum);

            emp::WorldPosition pos(r, 1);
            sys.AddOrg(childGen[r], pos); //removed brackets childGen[i], {i, 0}
            //cout << "size of child population: " << size(childGen) << endl;
        }
        //sys.PrintStatus();
        cout << "phylogenetic diversity: " << sys.GetPhylogeneticDiversity() << endl;

        if(i == TenGens - 1){
            //sys.FindPhyloData();
            cout << "Ten Gens = " << TenGens << endl;

            int phylogenetic_diversity = sys.GetPhylogeneticDiversity(); 
            cout << "PHYLO DIVERSITY: " << phylogenetic_diversity << endl; 
            int NumLeaf = sys.GetNumLeafNodes(); 
            cout << "NUM LEAF: " << NumLeaf << endl; 
            //sys.NumConfigurations(NumLeaf, phylogenetic_diversity);
            writeToFile("NumConfigResults.csv", sys.NumConfigurations(NumLeaf, phylogenetic_diversity));

            //TenGens = TenGens + 10;
            //sys.FindPhyloMultipleGens(TenGens);
            //sys.GetPhylogeneticDiversity(TenGens, "TensChooseOrgGenotype.csv");
            //cout << "Phylogenetic Diversity Return: " << sys.GetPhyloNormalize() << endl;

            //writeToFile("ChooseOrgDiversityGenotype1000.csv", sys.GetPhylogeneticDiversity())
            TenGens = TenGens + 10;
        }

        for(int j = 0; j < currentGen.size(); j++){
            sys.RemoveOrg(j);
        }


        switchGens(currentGen,childGen, sys); //puts contents of child vector into current vector and deletes content of child vector
    }

    int total_orgs = numGens * numOrgs;

    cout << "generations: " << numGens << " / total organisms: " << total_orgs << endl;
};

bool writeToFile(string filename, double field_one){
    ofstream file;
    file.open(filename, ios_base::app);
    file << field_one << ",";
    file.close();

    return true;
}