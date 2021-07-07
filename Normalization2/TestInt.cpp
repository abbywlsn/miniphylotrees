#include <iostream>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include "boost/multiprecision/cpp_bin_float.hpp"
typedef boost::multiprecision::number<boost::multiprecision::cpp_bin_float<1000> > cpp_bin_float_1000;

//g++ -std=c++17 -g -pthread -Wall -Wno-unused-function -Wno-unused-private-field -I/mnt/c/Users/abbys/boost/boost_1_61_0 main.cpp

bool writeToFileInt(std::string filename, int field_one, boost::multiprecision::cpp_int field_two){
    std::ofstream file;
    file.open(filename, std::ios_base::app);
    file << field_one << "," << field_two << std::endl;
    file.close();
    return true;
}

 boost::multiprecision::cpp_int catalanNumberInt(boost::multiprecision::cpp_int n){  
    boost::multiprecision::cpp_int cat = 1; 
    for(boost::multiprecision::cpp_int i = 1; i <= n; i++){ 
      cat *= (4*i-2); 
      cat /= (i+1); 
    }
    return cat; 
  }


  double NumConfigurationsInt(boost::multiprecision::cpp_int n, boost::multiprecision::cpp_int phylo_value){ //n is number of leaf nodes 


    boost::multiprecision::cpp_int p = n - 1; //p is largest number of internal nodes that a tree with n leaf nodes could have 
    boost::multiprecision::cpp_int tot_num_trees = 0;
    boost::multiprecision::cpp_int num_less = 0; 
    boost::multiprecision::cpp_int num_trees = 0;


    for(boost::multiprecision::cpp_int i = 0; i <= p - 1; i++){ 

      num_trees = catalanNumberInt(p-i)*boost::multiprecision::pow(p-i, (int) i); //num trees is total number of possible trees
 
      //calculate phylo diversity with p-i internal nodes as internal nodes + leaf nodes -1
      //std::cout << "NUM_TREES: " << num_trees << std::endl; 
      //std::cout << "COMPARE TO PHYLO NUMBER: " << (p-i+n-1) << std::endl; 
      if(phylo_value >= (p - i + n - 1)){ 
        num_less += num_trees; 
        //std::cout << "NUM_LESS: " << num_less << std::endl; 
      }
      tot_num_trees += num_trees;
      //std::cout << "I value: " << i << "      " << "TOT_NUM_TREES: " << tot_num_trees << std::endl; 
    }

    std::cout << "INT final num less: " << num_less << std::endl; 
    std::cout << "INT final tot num: " << tot_num_trees << std::endl; 
   
    // double less = (double) num_less; 
    // double total = (double) tot_num_trees; 
    // // std::cout << "Less Int: " << less << std::endl; 
    // // std::cout << "Total Int: " << total << std::endl; 

    // double val = less/total; 
    // std::cout << "NumConfigurations Result: " << val << std::endl; 

    return 100;     
  }


int PhyloHistogram(boost::multiprecision::cpp_int n){ //n is num of leaf nodes
    boost::multiprecision::cpp_int p = n - 1;
    boost::multiprecision::cpp_int trees = 0;

    for(boost::multiprecision::cpp_int i = 0; i <= p - 1; i++){
        trees = catalanNumberInt(p-i)*boost::multiprecision::pow(p-i, (int) i); //num trees is total number of possible trees
        boost::multiprecision::cpp_int phylo_diversity = p - i + n - 1;

        std::cout << "NUM TREES: " << trees << " PHYLO DIVERSITY: " << phylo_diversity << std::endl;
        writeToFileInt("SpecificHist148.csv", (int) phylo_diversity, trees);
    }
    return 0;
}
// bool writeToFileFloat(std::string filename, int field_one, cpp_bin_float_1000 field_two){
//     std::ofstream file;
//     file.open(filename, std::ios_base::app);
//     file << field_one << "," << field_two << std::endl;
//     file.close();
//     return true;
// }

//  cpp_bin_float_1000 catalanNumberFloat(cpp_bin_float_1000 n){  
//     cpp_bin_float_1000 cat = 1; 
//     for(cpp_bin_float_1000 i = 1; i <= n; i++){ 
//       cat *= (4*i-2); 
//       cat /= (i+1); 
//     }
//     return cat; 
//   }


//   double NumConfigurationsFloat(cpp_bin_float_1000 n, cpp_bin_float_1000 phylo_value){ //n is number of leaf nodes 


//     cpp_bin_float_1000 p = n - 1; //p is largest number of internal nodes that a tree with n leaf nodes could have 
//     cpp_bin_float_1000 tot_num_trees = 0;
//     cpp_bin_float_1000 num_less = 0; 
//     cpp_bin_float_1000 num_trees = 0;


//     for(cpp_bin_float_1000 i = 0; i <= p - 1; i++){ 

//       num_trees = catalanNumberFloat(p-i)*boost::multiprecision::pow(p-i, (int) i); //num trees is total number of possible trees
 
//       //calculate phylo diversity with p-i internal nodes as internal nodes + leaf nodes -1
//       //std::cout << "NUM_TREES: " << num_trees << std::endl; 
//       //std::cout << "COMPARE TO PHYLO NUMBER: " << (p-i+n-1) << std::endl; 
//       if(phylo_value >= (p - i + n - 1)){ 
//         num_less += num_trees; 
//         //std::cout << "NUM_LESS: " << num_less << std::endl; 
//       }
//       tot_num_trees += num_trees;
//       //std::cout << "I value: " << i << "      " << "TOT_NUM_TREES: " << tot_num_trees << std::endl; 
//     }

//     std::cout << "FLOAT final num less: " << num_less << std::endl; 
//     std::cout << "FLOAT final tot num: " << tot_num_trees << std::endl; 
   
//     // double less = (double) num_less; 
//     // double total = (double) tot_num_trees; 
//     // // std::cout << "Less Int: " << less << std::endl; 
//     // // std::cout << "Total Int: " << total << std::endl; 

//     // double val = less/total; 
//     // std::cout << "NumConfigurations Result: " << val << std::endl; 

//     return 100;     
//   }

int main() {
  PhyloHistogram(148);  
    
}