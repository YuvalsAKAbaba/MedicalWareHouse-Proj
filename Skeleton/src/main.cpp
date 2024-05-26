#include "MedicalWarehouse.h"
#include "Action.h"
#include <iostream>

using namespace std;

MedicalWarehouse * backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: MedicalWarehouse <config_path>" << std::endl;
        return 0;
    }    
    MedicalWarehouse medWareHouse(argv[1]);
    medWareHouse.start();    
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }        
}