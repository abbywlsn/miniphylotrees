//TEST TREES

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <assert.h>
//#include "/mnt/c/Users/abbys/Empirical/include/emp/Evolve/Systematics.hpp"
#include "Evolve/Systematics.hpp"

using namespace std;

int numOrgs = 600;
int parentNum;
int numGens = 10;
double mutRate = 0.05;
int TenGens = 10; //TenGens is a variable that is incremented by 10 after each round in order to check the correct number of generations during normalization 


//assert that diversity is the same every time (check)
//make sure that phylo diversity is calculated correctly (check)
//make sure that systematics is classifying phylo value correctly (returns good percentile)
//make test that runs this code and asserts that final value is 5 and in 91 percentile

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
        assert(randMutation != 0 && randMutation > 0);
        //cout << "random mutation rate = " << randMutation << endl;
        //cout << "GENOTYPE BEFORE MUTATION: " << genotype << endl;

        if (randMutation < mutRate) {
            int MutatedGenotype = genotype - RandNum.GetInt(-3, 3);
            genotype = MutatedGenotype;
            //cout << "mutated genotype = " << genotype << endl;
        }
        //cout << "GENOTYPE: " << genotype << endl;
        return genotype;
    }
};



int chooseOrg(vector<Organism> &currentGen, emp::Random &randNum){

    parentNum = randNum.GetInt((int) currentGen.size());  //chooses random spot in array for parent
    //assert(parentNum != 0);
    //cout << "parent chosen is in spot " << parentNum << " in currentGen array which is " << size(currentGen) << " long" << endl;
    return parentNum;
}

//int chooseOrgSet(vector<Organism> &currentGen, emp::Random &randNum){
//    parentNum = randNum.GetInt(currentGen.size());  //chooses random spot in array for parent
//    //cout << "parent chosen is in spot " << parentNum << " in currentGen array which is " << size(currentGen) << " long" << endl;
//    return parentNum;
//}


void calcFitness(vector<Organism> &currentGen, vector<double> &fitnessVect, emp::Random &randNum) {
    fitnessVect.resize(0);

    vector<int> fitnessCalc;

    fitnessCalc.reserve((int) currentGen.size());
    for (int i = 0; i < (int) currentGen.size(); i++) {
        fitnessCalc.push_back(currentGen[i].genotype);
    }

    map<int, int> CountMap;

    for (int j = 0; j < (int) fitnessCalc.size(); j++) {
        if (emp::Has(CountMap, fitnessCalc[j])) {
            CountMap[fitnessCalc[j]]++;
        } else {
            CountMap[fitnessCalc[j]] = 1;
        }
    }
    for(int k = 0; k < (int) fitnessCalc.size(); k++){
        fitnessVect.push_back(1.0/CountMap[fitnessCalc[k]]);
    }
}


int chooseOrgDiversity(vector<double> &fitnessVect, emp::Random &randNum){
   emp::IndexMap fitness_index((int) fitnessVect.size());

   for (size_t id = 0; id < fitnessVect.size(); id++){
       fitness_index.Adjust(id, fitnessVect[id]);
   }

   const double fit_pos = randNum.GetDouble(fitness_index.GetWeight());
   size_t parent_id = fitness_index.Index(fit_pos);

//    for(int pos = 0; pos < fitnessVect.size(); pos++){
//        //cout << fitnessVect[pos] << " " << endl;
//    }

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

//bool writeToFile(string filename, int field_one);
bool writeToFileThree(string filename, int field_one, int field_two, double field_three);



int main() {
    //emp::Random randNum(1);
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
        //cout << "generation: " << i << endl;
        //assert(currentGen.size() == 10);

        calcFitness(currentGen, fitnessVect, randNum);


        for(int r = 0; r < numOrgs; r++){

            chooseOrg(currentGen, randNum);
            //chooseOrg(); //chooses the parent of the next generation
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
        //cout << "phylogenetic diversity: " << sys.GetPhylogeneticDiversity() << endl;

        if(i == TenGens - 1){
            //sys.FindPhyloData();
            //cout << "Ten Gens = " << TenGens << endl;
            //sys.FindPhyloMultipleGens(TenGens);
            //sys.GetPhylogeneticDiversity(TenGens, "/mnt/c/Users/abbys/miniphylotrees/GenTrees/TensChooseOrgGenotype.csv");
            int phylogenetic_diversity = sys.GetPhylogeneticDiversity(true); 
            //cout << "PHYLO DIVERSITY: " << phylogenetic_diversity << endl; 
            //int NumLeaf = sys.GetNumLeafNodes(); 
            //cout << "NUM LEAF: " << NumLeaf << endl; 
            //cout << sys.NumConfigurations(NumLeaf, phylogenetic_diversity) << endl;
            //writeToFileThree("MiniPhyloTreesResults.csv", NumLeaf, phylogenetic_diversity, sys.NumConfigurations(NumLeaf, phylogenetic_diversity)); 
            writeToFile("ChooseOrgDiversityGenotype1000.csv", sys.GetPhylogeneticDiversity());
            //so we want to record 10, 100, and 1000 organisms 
            //we want to have 0 through 5000 generations


            //TenGens = TenGens + 10;
        }

        for(int j = 0; j < (int) currentGen.size(); j++){
            sys.RemoveOrg(j);
        }


        switchGens(currentGen,childGen, sys); //puts contents of child vector into current vector and deletes content of child vector
        //systime++;
    }

    int total_orgs = numGens * numOrgs;

    cout << "generations: " << numGens << " / total organisms: " << total_orgs << endl;
};

bool writeToFile(string filename, int field_one){
   ofstream file;
   file.open(filename, ios_base::app);
   file << field_one << ",";
   file.close();

   return true;
}

  bool writeToFileThree(string filename, int field_one, int field_two, double field_three){
    ofstream file;
    file.open(filename, std::ios_base::app);
    //this will be number of nodes, not normalized diversity, and num cconfig result
    file << field_one << "," << field_two << "," << field_three << endl;
    file.close();

    return true;
}