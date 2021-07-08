#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include "Evolve/Systematics.hpp"
#include "config/config.hpp"
#include "config/command_line.hpp"
#include "config/ArgManager.hpp"

//for i in {1..5}; do make DiversityTrees && ./a.out; done

//g++ -std=c++17 -I../../ main.cpp -o main.o && ./main.o
//for i in {1..5}; do g++ -std=c++17 -I../../ main.cpp -o main.o && ./main.o; done
//for i in {1..100}; do ./main.o; done

/*This file creates trees with mutations and pressure for diversity (optional) for the systematics normalization project.
 * This program generates trees with a depth of 10 to 100 generations. Mutation occurs when a random number generted between
 * 0 and 1 is less than 0.05. When a mutation occurs, another random number is generated between -3 and 3. Whatever number is chosen
 * is then subtracted from the original genotype value. If the user chooses to use pressure for diversity, they must use the
 * ChooseOrgDiversity() function and the CalcFitness() function. The ChooseOrg() function should be left unused in this scenario.
 */
/*This file creates trees with mutations and pressure for diversity (optional) for the systematics normalization project.
 * This program generates a random tree with a depth of
 * 100 generations. This can be changed in the gen variable. This tree has 10 organisms per generation and uses a clade
 * as the method of tracking in systematics.h. The program returns phylogenetic diversity after each generation and prints the
 * final phylogenetic diversity to a csv file of your choice. You will have to uncomment the writeToFile function and give it a
 * file path to use this function.
 * You can also use this tree to test the systematics.h FindPhyloData() function. You can uncomment the line sys.FindPhyloData() to
 * use this function of the program. */


using namespace std;

EMP_BUILD_CONFIG(PressureForDiversityConfig,
  GROUP(MAIN, "Tree Settings"),
  VALUE(MUTATION_RATE, double, 0.05, "This is my mutation rate."),
  VALUE(POP_SIZE, int, 10, "This is my population size."),
  VALUE(GENERATION_COUNT, int, 10, "This is my number of generations."),
);

PressureForDiversityConfig config; 

// int numOrgs = 1000; 
 int parentNum;
 //int gen = 10;
// double mutRate = 0.05;
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

        if (randMutation < config.MUTATION_RATE()) {
            int MutatedGenotype = genotype - RandNum.GetInt(-3, 3);
            genotype = MutatedGenotype;
        }
        return genotype;
    }
};

// int chooseOrg(vector<Organism> &currentGen, emp::Random &randNum){
//     parentNum = randNum.GetInt(size(currentGen));  //chooses random spot in array for parent
//     return parentNum;
// }


void calcFitness(vector<Organism> &currentGen, vector<double> &fitnessVect, emp::Random &randNum) {
    fitnessVect.resize(0);

    vector<int> fitnessCalc;

    fitnessCalc.reserve(currentGen.size());
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
    emp::IndexMap fitness_index(fitnessVect.size());
    for (size_t id = 0; id < fitnessVect.size(); id++){
        fitness_index.Adjust(id, fitnessVect[id]);
    }
    const double fit_pos = randNum.GetDouble(fitness_index.GetWeight());
    size_t parent_id = fitness_index.Index(fit_pos);
    parentNum = parent_id;
    return parentNum;
}

void switchGens(vector<Organism> &currentGen, vector<Organism> &childGen, emp::Systematics<Organism, int> &sys){
    currentGen.swap(childGen);
    childGen.clear();
    sys.Update();
}

bool writeToFile(std::string filename, int field_one, bool skip_line){
    if(skip_line == true){ 
        std::ofstream file;
        file.open(filename, std::ios_base::app);
        file << " " << std::endl;
        file.close();
    }else{ 
        std::ofstream file;
        file.open(filename, std::ios_base::app);
        file << field_one << ",";
        file.close();
    }
   return true;
}

int main(int argc, char * argv[]) {

    auto args = emp::cl::ArgManager(argc, argv); 
    if (args.ProcessConfigOptions(config, std::cout, "PressureForDiversityConfig.cfg", "My-macros.h") == false) exit(0);
    if(args.TestUnknown() == false) exit(0); //if there are leftover args throw an error
    std::cout << "==============================" << std::endl;
    std::cout << "|    How am I configured?    |" << std::endl;
    std::cout << "==============================" << std::endl;
    config.Write(std::cout);
    std::cout << "==============================\n" << std::endl; 

    emp::Random randNum;

    for(int gen = 1; gen <= 5000; gen++){ 
        for(int run = 1; run <= 1000; run++){ 
            function<int(Organism)> taxonFunc = [](Organism org){return org.genotype;};
            emp::Systematics<Organism, int> sys(taxonFunc); //optional 3rd arg
            sys.SetTrackSynchronous(true);

            //current gen (vector)
            vector<Organism> currentGen; //begins with currentGen
            //child gen (vector)
            vector<Organism> childGen;

            vector<double> fitnessVect;

            for (int i = 0; i < config.POP_SIZE(); i++) {
                currentGen.emplace_back(); //currentGen is filled with 10 organism
                sys.AddOrg(currentGen[i], i); //parent is null (removed brackets)
            }

            for (int i = 0; i < gen; i++) {
                //cout << "generation: " << i << endl;
                calcFitness(currentGen, fitnessVect,randNum);

                for(int r = 0; r < config.POP_SIZE(); r++){

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
                
                if(i == gen - 1){
                    //sys.FindPhyloData();
                    cout << "Gen = " << gen << endl;
                    int phylogenetic_diversity = sys.GetPhylogeneticDiversity(); 
                    writeToFile("PressureForDiversityResults_10.csv", phylogenetic_diversity, false);
                    //TenGens = TenGens + 10;
                }

                for(int j = 0; j < (int) currentGen.size(); j++){
                    sys.RemoveOrg(j);
                }
                switchGens(currentGen,childGen, sys); //puts contents of child vector into current vector and deletes content of child vector
            }

            int total_orgs = gen * config.POP_SIZE();

            cout << "generations: " << gen << " / total organisms: " << total_orgs << endl;
                    
        }
         writeToFile("PressureForDiversityResults_10.csv", 0, true);
    }
    
};