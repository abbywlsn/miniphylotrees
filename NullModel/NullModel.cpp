#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <assert.h>
#include "Evolve/Systematics.hpp"
#include "config/config.hpp"
#include "config/command_line.hpp"
#include "config/ArgManager.hpp"


//need to go through each generation 1000 times 
//increment generations by 1 
//change populaltion to 10, 100, 1000


EMP_BUILD_CONFIG(NullModelConfig,
  GROUP(MAIN, "Tree Settings"),
  VALUE(MUTATION_RATE, double, 0.05, "This is my mutation rate."),
  VALUE(POP_SIZE, int, 100, "This is my population size."),
  VALUE(GENERATION_COUNT, int, 10, "This is my number of generations."),
  VALUE(RANDOM_SEED, int, 10, "Seed for random number generator"),
);

NullModelConfig config; 

// double config.MUTATION_RATE() = config.MUTATION_RATE(); 
// int config.POP_SIZE() = config.POP_SIZE(); 
 int parentNum; 
// int gen = gen; 
// int TenGens = 10; 

class Organism {
public:
    int genotype = 0;

    Organism() { //default constructor sets genotype to 0
    }

    Organism(int _genotype) { //this constructor sets genotype to the parent genotype
        genotype = _genotype;
    }

    int MutateGenotype(emp::Random &RandNum) {

        double randMutation = RandNum.GetDouble(0, 1);
        //assert(randMutation != 0 && randMutation > 0);

        if (randMutation < config.MUTATION_RATE()) {
            int MutatedGenotype = genotype - RandNum.GetInt(-3, 3);
            genotype = MutatedGenotype;
            //cout << "mutated genotype = " << genotype << endl;
        }
        //cout << "GENOTYPE: " << genotype << endl;
        return genotype;
    }
};

int chooseOrg(std::vector<Organism> &currentGen, emp::Random &randNum){
    parentNum = randNum.GetInt((int) currentGen.size());  //chooses random spot in array for parent
    return parentNum;
}

void switchGens(std::vector<Organism> &currentGen, std::vector<Organism> &childGen, emp::Systematics<Organism, int> &sys){
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
    //NullModelConfig config; 
    auto args = emp::cl::ArgManager(argc, argv); 
    if (args.ProcessConfigOptions(config, std::cout, "NullModelConfig.cfg", "My-macros.h") == false) exit(0);
    if(args.TestUnknown() == false) exit(0); //if there are leftover args throw an error
    std::cout << "==============================" << std::endl;
    std::cout << "|    How am I configured?    |" << std::endl;
    std::cout << "==============================" << std::endl;
    config.Write(std::cout);
    std::cout << "==============================\n" << std::endl; 
    
    emp::Random randNum(config.RANDOM_SEED());

     for(int gen = 1; gen <= 1000; gen++){ 
         for(int run = 1; run <= 1000; run++){ 

            //std::cout << config.POP_SIZE() << std::endl; 
            std::function<int(Organism)> taxonFunc = [](Organism org){return org.genotype;};
            emp::Systematics<Organism, int> sys(taxonFunc); //optional 3rd arg
            sys.SetTrackSynchronous(true);

            //current gen (vector)
            std::vector<Organism> currentGen; //begins with currentGen
            //child gen (vector)
            std::vector<Organism> childGen;

            for (int i = 0; i < config.POP_SIZE(); i++) {
                currentGen.emplace_back(); //currentGen is filled with 10 organism
                sys.AddOrg(currentGen[i], i); //parent is null (removed brackets)
            }

        //    for(int i = 0; i < currentGen.size(); i++){
        //        cout << currentGen[i] . printVect() << " " << endl;
        //    }

            for (int i = 0; i < gen; i++) {
                for(int r = 0; r < config.POP_SIZE(); r++){

                    chooseOrg(currentGen, randNum);
                    sys.SetNextParent(parentNum);
                    //currentGen[parentNum].reproduce(childGen, sys); //fills childGen with 10 Organisms

                    childGen.emplace_back(currentGen[parentNum].genotype); //fills childGen vector with Organisms
                    childGen[r].MutateGenotype(randNum);

                    emp::WorldPosition pos(r, 1);
                    sys.AddOrg(childGen[r], pos); //removed brackets childGen[i], {i, 0}
                }
                //sys.PrintStatus();

                if(i == gen - 1){
                    int phylogenetic_diversity = sys.GetPhylogeneticDiversity(); 
                    std::cout << phylogenetic_diversity << std::endl; 
                    //writeToFile("NullModelResults_1000.csv", phylogenetic_diversity, false);
                    //so we want to record 10, 100, and 1000 organisms 
                    //we want to have 0 through 5000 generations
                }

                for(int j = 0; j < (int) currentGen.size(); j++){
                    sys.RemoveOrg(j);
                }


                switchGens(currentGen,childGen, sys); //puts contents of child vector into current vector and deletes content of child vector
            } 

            //int total_orgs = gen * config.POP_SIZE();
           // std::cout << "generations: " << gen << " / total organisms: " << total_orgs << std::endl;

        }
        //writeToFile("NullModelResults_1000.csv", 0, true);
    }
    
};
