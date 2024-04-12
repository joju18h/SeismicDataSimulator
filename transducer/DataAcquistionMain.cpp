#include "DataAcquisition.h"
#include <iostream>


using namespace std;

int main(){
    int retVal = 0;
    DataAcquisitionUnit dataAcquisitionUnit;
    retVal = dataAcquisitionUnit.run();
    return retVal;
}