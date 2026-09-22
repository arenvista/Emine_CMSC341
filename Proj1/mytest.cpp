#include "csr.h"

class Tester{

    public:

    private:

};

int main (){
    CSR test;
    int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    test.compress(4,6,array1,24);
    test.dump();

    //cout << endl << test.getAt(2,4) << endl;//returns the value 70
    cout << endl << test.sparseRatio() << endl;

    CSR cCSR;
    int array2[] = {0,0,0,0,100,200,0,0,300};
    cCSR.compress(3,3,array2,9);//initialize object cCSR
    cCSR.dump();
    cout << endl << cCSR.sparseRatio() << endl;


    return 0;
}