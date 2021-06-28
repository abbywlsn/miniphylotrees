#include <iostream>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>

//g++ -std=c++17 -g -pthread -Wall -Wno-unused-function -Wno-unused-private-field -I/mnt/c/Users/abbys/boost/boost_1_61_0 main.cpp

bool writeToFile(std::string filename, int field_one, boost::multiprecision::cpp_int field_two){
    std::ofstream file;
    file.open(filename, std::ios_base::app);
    file << field_one << "," << field_two << std::endl;
    file.close();
    return true;
}

 boost::multiprecision::cpp_int catalanNumber(boost::multiprecision::cpp_int n){  
    boost::multiprecision::cpp_int cat = 1; 
    for(boost::multiprecision::cpp_int i = 1; i <= n; i++){ 
      cat *= (4*i-2); 
      cat /= (i+1); 
    }
    return cat; 
  }


  double NumConfigurations(boost::multiprecision::cpp_int n, boost::multiprecision::cpp_int phylo_value){ //n is number of leaf nodes 


    boost::multiprecision::cpp_int p = n - 1; //p is largest number of internal nodes that a tree with n leaf nodes could have 
    boost::multiprecision::cpp_int tot_num_trees = 0;
    boost::multiprecision::cpp_int num_less = 0; 
    boost::multiprecision::cpp_int num_trees = 0;


    for(boost::multiprecision::cpp_int i = 0; i <= p - 1; i++){ 

      num_trees = catalanNumber(p-i)*boost::multiprecision::pow(p-i, (int) i); //num trees is total number of possible trees
 
      //calculate phylo diversity with p-i internal nodes as internal nodes + leaf nodes -1
      std::cout << "NUM_TREES: " << num_trees << std::endl; 
      //std::cout << "COMPARE TO PHYLO NUMBER: " << (p-i+n-1) << std::endl; 
      if(phylo_value >= (p - i + n - 1)){ 
        num_less += num_trees; 
        //std::cout << "NUM_LESS: " << num_less << std::endl; 
      }
      tot_num_trees += num_trees;
      //std::cout << "I value: " << i << "      " << "TOT_NUM_TREES: " << tot_num_trees << std::endl; 
    }

    std::cout << "FINAL NUM LESS: " << num_less << std::endl; 
    std::cout << "FINAL TOT NUM: " << tot_num_trees << std::endl; 
   
    double less = (double) num_less; 
    double total = (double) tot_num_trees; 
    // std::cout << "Less Int: " << less << std::endl; 
    // std::cout << "Total Int: " << total << std::endl; 

    double val = less/total; 
    std::cout << "NumConfigurations Result: " << val << std::endl; 

    return val;     
  }


int PhyloHistogram(boost::multiprecision::cpp_int n){
    boost::multiprecision::cpp_int p = n - 1;
    boost::multiprecision::cpp_int trees = 0;

    for(boost::multiprecision::cpp_int i = 0; i <= p - 1; i++){
        trees = catalanNumber(p-i)*boost::multiprecision::pow(p-i, (int) i); //num trees is total number of possible trees
        boost::multiprecision::cpp_int phylo_diversity = p - i + n - 1;

        std::cout << "NUM TREES: " << trees << "       PHYLO DIVERSITY: " << phylo_diversity << std::endl;
        //writeToFile("PhyloHistogram.csv", (int) phylo_diversity, trees);
    }
    return 0;
}

int main() {
    std::cout << catalanNumber(10) << " <- CATALAN NUMBER" << std::endl; 
    std::cout << PhyloHistogram(10) << " <- PHYLO HIST RAN" << std::endl; 
    std::cout << NumConfigurations(10, 10) << " <- NUM CONFIGURATIONS" << std::endl; 
}