// CMSC 341 - Fall 2026 - Project 3
#include "er.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = mt19937(10);// 10 is the fixed seed value
            m_unidist = uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = mt19937(10);// 10 is the fixed seed value
            m_uniReal = uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = mt19937(10);// 10 is the fixed seed value
        m_unidist = uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random patient, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random patient, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    random_device m_device;
    mt19937 m_generator;
    normal_distribution<> m_normdist;//normal distribution
    uniform_int_distribution<> m_unidist;//integer uniform distribution
    uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

// the followings are sample priority functions to be used by Hospital class
// Priority functions compute an integer priority for a patient.
int priorityFn1(const Patient &patient);// works with a MAXHEAP
int priorityFn2(const Patient &patient);// works with a MINHEAP

class Tester{
    public:

    bool testInsertMIN();//done
    bool testInsertMAX();//done
    bool testRemovalMIN();//done
    bool testRemovalMAX();//done
    bool testNPLVals();//done
    bool testLeftistProper();//done
    bool testChangingPriorFunc();//done
    bool testMergeOnEmptyQue();//done
    bool testHospitalCCNormal();//done
    bool testHospitalCCEdge();//done
    bool testHospitalAONormal();//done
    bool testHospitalAOEdge();//done
    bool testEmptyDequeue();//do
    bool testIncompatibleMerge();//do

    private:
    int countNodes(Patient * root);
    bool isHeapOrdered(Patient * root, prifn_t priFn, HEAPTYPE heapType);
    bool idUsed(int id[], int size, int idNum);
    void uniqueIDGen(int id[], int size, int min, int max);
    bool insertPatients(Hospital & hospital, int size);

    bool testNPLVals(Patient * root);
    int calcNPL(Patient * root);
    bool testLeftist(Patient * root);
    void collectNodes(Patient * nodes[], Patient * root, int & numNodes);
    bool doesPointerExist(Patient * nodes[], Patient * pointer, int & numNodes);
    bool testTreeEquality(Patient * treeOne, Patient * treeTwo);


};

int main(){

    Tester tester;
    
    cout << "Test insertion for a normal case of min-heap." << endl;

    tester.testInsertMIN() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test insertion for a normal case of max-heap." << endl;

    tester.testInsertMAX() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test removal for a normal case of min-heap." << endl;

    tester.testRemovalMIN() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test removal for a normal case of max-heap." << endl;

    tester.testRemovalMAX() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test all nodes in a leftist heap have the correct NPL value." << endl;

    tester.testNPLVals() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test a leftist heap preserves the property of such a heap" << endl;

    tester.testLeftistProper() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test changing the priority function" << endl;

    tester.testChangingPriorFunc() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test merge of an empty queue (an edge case) with a normal queue" << endl;

    tester.testMergeOnEmptyQue() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test the Hospital class copy constructor for a normal case" << endl;

    tester.testHospitalCCNormal() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test the Hospital class copy constructor for a edge case" << endl;

    tester.testHospitalCCEdge() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test the Hospital class asignment operator for a normal case" << endl;

    tester.testHospitalAONormal() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test the Hospital class asignment operator for a edge case" << endl;

    tester.testHospitalAOEdge() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    return 0;
}

int priorityFn1(const Patient &patient) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [115-242]
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    //the larger value means the higher priority
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient &patient) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [71-111]
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    //the smaller value means the higher priority
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}

bool Tester::idUsed(int id[], int size, int idNum){

    for (int i = 0; i < size; i++){

        if (id[i] == idNum){
            return false; 
        }

    }
    return true;
}

void Tester::uniqueIDGen(int id[], int size, int min, int max){
    Random idGenerator(min, max);

    int usedID = 0;

    while (usedID < size){
        int idNum = idGenerator.getRandNum();

        if (idUsed(id, usedID, idNum) != false){
            id[usedID] = idNum;
            usedID ++;
        }
    }

}

bool Tester::insertPatients(Hospital & hospital, int size){
    int * patientIds = new int[size];
    uniqueIDGen(patientIds, size, MINPATIENTID, MAXPATIENTID);

    Random BPGen(MINBP,MAXBP);
    Random RRGen(MINRR,MAXRR);
    Random oxygenGen(MINOX,MAXOX);
    Random opinionGen(MINOPINION,MAXOPINION);
    Random temperatureGen(MINTEMP,MAXTEMP);

    for (int i = 0; i < size; i++){
        Patient newPatient(patientIds[i], temperatureGen.getRandNum(),
         oxygenGen.getRandNum(), RRGen.getRandNum(), BPGen.getRandNum(), opinionGen.getRandNum());
        
        if (hospital.insertPatient(newPatient) != true){
            delete [] patientIds;
            return false;

        }
    }

    delete [] patientIds;
    return true;
}

int Tester::countNodes(Patient * root){
    if (root == nullptr){
        return 0; 
    }

    return 1 + countNodes(root->m_left) + countNodes(root->m_right);
    
}

bool Tester::isHeapOrdered(Patient * root, prifn_t priFn, HEAPTYPE heapType){

    if (root == nullptr){
        return true; 
    }

    int parentPrio = priFn(*root);

    if (root->m_left != nullptr){

        int leftPrio = priFn(*(root->m_left));

        if( (heapType == MINHEAP && parentPrio > leftPrio) || (heapType == MAXHEAP && parentPrio < leftPrio )){
            return false; 
        }
    }

    if (root->m_right != nullptr){

        int rightPrio = priFn(*(root->m_right));

        if( (heapType == MINHEAP && parentPrio > rightPrio) || (heapType == MAXHEAP && parentPrio < rightPrio )){
            return false; 
        }
    }

    return true; 
}

int Tester::calcNPL(Patient * root){

    if (root == nullptr){
        return -1;
    }

    int leftNPL = calcNPL(root->m_left);

    int rightNPL = calcNPL(root->m_right);

    if (rightNPL < leftNPL){
        return rightNPL + 1;
    }

    return leftNPL + 1; 
}

bool Tester::testNPLVals(Patient * root){

    if (root == nullptr){
        return true;
    }

    if  (root->m_npl != calcNPL(root)){
        return false;
    }

    if (testNPLVals(root->m_left) == false){
        return false; 
    }

    
    if (testNPLVals(root->m_right) == false){
        return false; 
    }

    return true; 

}

bool Tester::testLeftist(Patient * root){
    if (root == nullptr){
        return true; 
    }

    int leftNPL = -1;
    int rightNPL = -1;

    if (root->m_left != nullptr){
        leftNPL = root->m_left->m_npl;
    }

    if (root->m_right != nullptr){
        rightNPL = root->m_right->m_npl;
    }

    if (rightNPL > leftNPL){
        return false; 
    }

    if (testLeftist(root->m_left) != true){
        return false; 
    }
    if (testLeftist(root->m_right) != true){
        return false; 
    }

    return true; 
}

void Tester::collectNodes(Patient * nodes[], Patient * root, int & numNodes){
    
    if (root != nullptr){
        nodes[numNodes] =  root;
        numNodes++;

        collectNodes(nodes, root->m_left, numNodes);
        collectNodes(nodes, root->m_right, numNodes);
    }
}

bool Tester::doesPointerExist(Patient * nodesBefore[], Patient * pointerNode, int & numNodes){

    for (int i = 0; i < numNodes; i++){
        if (nodesBefore[i] == pointerNode){
            return true; 
        }
    }

    return false; 
}

bool Tester::testTreeEquality(Patient * treeOne, Patient * treeTwo){

    if (treeOne == nullptr){
        if (treeTwo != nullptr){
            return false;
        }

        return true; 
    }

    if ((treeOne->m_BP != treeTwo->m_BP) || (treeOne->m_npl != treeTwo->m_npl) || (treeOne->m_opinion != treeTwo->m_opinion) ||
        (treeOne->m_oxygen != treeTwo->m_oxygen) || (treeOne->m_patient!= treeTwo->m_patient) ||
        (treeOne->m_RR != treeTwo->m_RR) || (treeOne->m_temperature != treeTwo->m_temperature)){
        return false;
    }

    if (testTreeEquality(treeOne->m_left, treeTwo->m_left) != true){
        return false;
    }

    if (testTreeEquality(treeOne->m_right, treeTwo->m_right) != true){
        return false;
    }

    return true;

}


/*
Test insertion for a normal case of min-heap.
After a decent number of insertion (e.g. 300 nodes) we traverse the tree
and check that the heap property is satisfied at every node
*/

bool Tester::testInsertMIN(){
    int size = 300;
    Hospital hospital(priorityFn2, MINHEAP, SKEW, 10, 100001);


    if (insertPatients(hospital, size) != true){
        return false; 
    }

    if (hospital.m_size != 300){
        return false; 
    }

    if (countNodes(hospital.m_heap) != 300){
        return false; 
    }

    if (isHeapOrdered(hospital.m_heap, hospital.getPriorityFn(), hospital.m_heapType) != true){
        return false; 
    }

    return true; 
}

/*
Test insertion for a normal case of max-heap.
After a decent number of insertion (e.g. 300 nodes) we traverse the tree
and check that the heap property is satisfied at every node.
*/

bool Tester::testInsertMAX(){
    int size = 300;
    Hospital hospital(priorityFn1, MAXHEAP, SKEW, 10, 100001);


    if (insertPatients(hospital, size) != true){
        return false; 
    }

    if (hospital.m_size != 300){
        return false; 
    }

    if (countNodes(hospital.m_heap) != 300){
        return false; 
    }

    if (isHeapOrdered(hospital.m_heap, hospital.getPriorityFn(), hospital.m_heapType) != true){
        return false; 
    }

    return true; 
}

/*
Test removal for a normal case of min-heap.
After a decent number of insertion (e.g. 300 nodes)
we check whether all removals happen in the correct patient.
*/

bool Tester::testRemovalMIN(){
    int size = 300;
    Hospital hospital(priorityFn2, MINHEAP, SKEW, 10, 100001);


    if (insertPatients(hospital, size) != true){
        return false; 
    }

    Patient curPatient = hospital.getNextPatient();
    int prevPatientPrio = priorityFn2(curPatient);

    for (int i = 1; i < size; i++ ){

        curPatient = hospital.getNextPatient();
        int curPatientPrio = priorityFn2(curPatient);

        if(curPatientPrio < prevPatientPrio){
            return false;
        }

        prevPatientPrio = curPatientPrio;
    }

    if (hospital.m_size != 0){
        return false; 
    }

    return true; 

}

/*
Test removal for a normal case of max-heap.
After a decent number of insertion (e.g. 300 nodes)
we check whether all removals happen in the correct patient.
*/

bool Tester::testRemovalMAX(){
    int size = 300;
    Hospital hospital(priorityFn1, MAXHEAP, SKEW, 10, 100001);


    if (insertPatients(hospital, size) != true){
        return false; 
    }

    Patient curPatient = hospital.getNextPatient();
    int prevPatientPrio = priorityFn1(curPatient);

    for (int i = 1; i < size; i++ ){

        curPatient = hospital.getNextPatient();
        int curPatientPrio = priorityFn1(curPatient);

        if(curPatientPrio > prevPatientPrio){
            return false;
        }

        prevPatientPrio = curPatientPrio;
    }

    if (hospital.m_size != 0){
        return false; 
    }

    return true; 
}

/*
Test all nodes in a leftist heap have the correct NPL value.
*/


bool Tester::testNPLVals(){

    int size = 300;
    Hospital hospital(priorityFn2, MINHEAP, LEFTIST, 10, 100001);


    if (insertPatients(hospital, size) != true){
        return false; 
    }

    if (hospital.m_size != 300){
        return false; 
    }

    if (countNodes(hospital.m_heap) != 300){
        return false; 
    }

    if (testNPLVals(hospital.m_heap) != true){
        return false; 
    }

    return true;
}

/*
Test a leftist heap preserves the property of such a heap,
i.e. at every node the NPL value of the left child is greater
than or equal to the NPL value of the right child. 
*/

bool Tester::testLeftistProper(){

    int size = 300;
    Hospital hospital(priorityFn2, MINHEAP, LEFTIST, 10, 100001);


    if (insertPatients(hospital, size) != true){
        return false; 
    }

    if (hospital.m_size != 300){
        return false; 
    }

    if (countNodes(hospital.m_heap) != 300){
        return false; 
    }

    if(testLeftist(hospital.m_heap) != true){
        return false; 
    }

    return true;
}

/*
Test whether after changing the priority
function a correct heap is rebuilt with the same data (nodes) and the different priority function.
*/

bool Tester::testChangingPriorFunc(){
    int size = 150;
    Hospital hospital(priorityFn2, MINHEAP, LEFTIST, 10, 100001);


    if (insertPatients(hospital, size) != true){
        return false; 
    }

    Patient * nodesBefore[150];
    int numNodesBefore = 0;
    collectNodes(nodesBefore, hospital.m_heap, numNodesBefore);

    if (numNodesBefore != 150){
        return false; 
    }

    hospital.setPriorityFn(priorityFn1, MAXHEAP);


    Patient * nodesAfter[150];
    int numNodesAfter = 0;
    collectNodes(nodesAfter, hospital.m_heap, numNodesAfter);

   
    if (numNodesAfter != 150){
      return false; 
    }

   
    if (hospital.getPriorityFn() != priorityFn1){
        return false;
    }

    
    if (hospital.getHeapType() != MAXHEAP){
        return false;
    }
    if (hospital.m_size != 150){
        return false; 
    }

    if (isHeapOrdered(hospital.m_heap, priorityFn1, MAXHEAP) != true){
        return false; 
    }

    for (int i = 0; i < numNodesAfter; i++){
        if (doesPointerExist(nodesAfter, nodesBefore[i], numNodesAfter) == false){
            return false; 
        }
    }

    return true;
}

/*
Test merge of an empty queue (an edge case) with a normal queue.
This is a call to the function Hospital::mergeWithQueue(Hospital& rhs)
where rhs is a normally populated queue.
*/

bool Tester::testMergeOnEmptyQue(){
    int size = 150;
    Hospital hospitalFilled(priorityFn2, MINHEAP, LEFTIST, 10, 100001);


    if (insertPatients(hospitalFilled, size) != true){
        return false; 
    }
    
    int sizeEmpty = 0;
    Hospital hospitalEmpty(priorityFn2, MINHEAP, LEFTIST, 10, 100002);


    if (insertPatients(hospitalEmpty, sizeEmpty) != true){
        return false; 
    }

    hospitalEmpty.mergeWithQueue(hospitalFilled);

    if (hospitalEmpty.m_size != 150){
        return false; 
    }

    if (hospitalFilled.m_size != 0){
        return false;
    }

        if (hospitalFilled.m_heap != nullptr){
        return false;
    }
    
    if (countNodes(hospitalEmpty.m_heap) != 150){
        return false; 
    }

    if (countNodes(hospitalFilled.m_heap) != 0){
        return false;
    }

    if (isHeapOrdered(hospitalEmpty.m_heap, hospitalEmpty.getPriorityFn(), hospitalEmpty.m_heapType) != true){
        return false; 
    }

    return true; 
    
}

/*
Test the Hospital class copy constructor for a normal case.
*/

bool Tester::testHospitalCCNormal(){

    int size = 250;
    Hospital hospitalOne(priorityFn2, MINHEAP, SKEW, 7, 100002);


    if (insertPatients(hospitalOne, size) != true){
        return false; 
    }

    Hospital hospitalTwo(hospitalOne);

  
    if (hospitalOne.m_size != hospitalTwo.m_size){
        return false; 
    }

    if (hospitalOne.m_heapType != hospitalTwo.m_heapType){
        return false; 
    }

    if (hospitalOne.m_structure != hospitalTwo.m_structure){
        return false; 
    }

    if (countNodes(hospitalTwo.m_heap) != countNodes(hospitalOne.m_heap)){
        return false; 
    }


    if (isHeapOrdered(hospitalTwo.m_heap, hospitalTwo.getPriorityFn(), hospitalTwo.m_heapType) != true){
        return false; 
    }

    if (isHeapOrdered(hospitalOne.m_heap, hospitalOne.getPriorityFn(), hospitalOne.m_heapType) != true){
        return false; 
    }


    if (testTreeEquality(hospitalOne.m_heap, hospitalTwo.m_heap) != true){
        return false; 
    }

    return true;
}

/*
Test the Hospital class copy constructor for an edge case.
*/

bool Tester::testHospitalCCEdge(){

    Hospital hospitalOne;

    Hospital hospitalTwo(hospitalOne);

  
    if (hospitalTwo.m_size != 0){
        return false; 
    }

    if (hospitalTwo.m_heap != nullptr){
        return false; 
    }

    return true;
}

/*
Test the Hospital class assignment operator for a normal case.
*/

bool Tester::testHospitalAONormal(){
    int size = 250;
    Hospital hospitalOne(priorityFn2, MINHEAP, SKEW, 7, 100001);


    if (insertPatients(hospitalOne, size) != true){
        return false; 
    }

    int sizeTwo = 150;
    Hospital hospitalTwo(priorityFn2, MINHEAP, SKEW, 7, 100002);

    if (insertPatients(hospitalTwo, sizeTwo) != true){
        return false; 
    }

    hospitalTwo = hospitalOne;

    if (hospitalOne.m_size != hospitalTwo.m_size){
        return false; 
    }

    if (hospitalOne.m_heapType != hospitalTwo.m_heapType){
        return false; 
    }

    if (hospitalOne.m_structure != hospitalTwo.m_structure){
        return false; 
    }

    if (countNodes(hospitalTwo.m_heap) != countNodes(hospitalOne.m_heap)){
        return false; 
    }


    if (isHeapOrdered(hospitalTwo.m_heap, hospitalTwo.getPriorityFn(), hospitalTwo.m_heapType) != true){
        return false; 
    }

    if (isHeapOrdered(hospitalOne.m_heap, hospitalOne.getPriorityFn(), hospitalOne.m_heapType) != true){
        return false; 
    }


    if (testTreeEquality(hospitalOne.m_heap, hospitalTwo.m_heap) != true){
        return false; 
    }

    return true;
}

/*
Test the Hospital class assignment operator for an edge case.
*/

bool Tester::testHospitalAOEdge(){

    int size = 150;
    Hospital hospitalOne(priorityFn2, MINHEAP, SKEW, 7, 100001);


    if (insertPatients(hospitalOne, size) != true){
        return false; 
    }

    Hospital hospitalTwo;

    hospitalOne = hospitalTwo;

    if (hospitalOne.m_size != 0){
        return false; 
    }

    if (hospitalOne.m_heap != nullptr){
        return false; 
    }

    return true;
}

/*
Test that attempting to dequeue an empty queue throws an out_of_range exception.
*/

bool Tester::testEmptyDequeue(){
    return true;
}

/*
Test that attempting to merge queues with different priority functions throws a domain_error exception.
*/

bool Tester::testIncompatibleMerge(){
    return true;
}