// CMSC 341 - Fall 2026 - Project 3
#ifndef ER_H
#define ER_H

#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;

class Grader; // forward declaration (for grading purposes)
class Tester; // forward declaration (for test functions)
class Hospital; // forward declaration
class Patient;  // forward declaration
class Region;   // forward declaration
#define ROOTINDEX 1
#define EMPTY Patient() // This is an empty object (invalid patient)
#define DEFAULTPATIENTID  0
enum HEAPTYPE {MINHEAP, MAXHEAP, NOTYPE};
enum STRUCTURE {SKEW, LEFTIST, NOSTRUCT};
// Priority function pointer type
typedef int (*prifn_t)(const Patient&);

// parameters, min and max values
const int HOSPITALMINID = 100001;
const int HOSPITALMAXID = 999999;
const int MINPATIENTID = 100001;
const int MAXPATIENTID = 999999;
const int MINTEMP = 35; // Body temperature, celsius
const int MAXTEMP = 42;
const int MINOX = 70;   // Level of oxygen saturation (SpO2)
const int MAXOX = 101;
const int MINRR = 10;   // Respiratory Rate, per minute
const int MAXRR = 40;
const int MINBP = 70;   // Blood Pressure
const int MAXBP = 160;
const int MINOPINION = 1;   // Nurse opinion, between 1 - 10
const int MAXOPINION = 10;  // 1 is highest priotity
//
// patient class
//
class Patient {
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // contains test functions
    friend class Hospital;
    Patient() {
        // This is an empty object since patient ID is zero
        m_patient = 0; m_temperature = 37; m_oxygen = 100;
        m_RR = 20;m_BP = 100;m_opinion=10;
        m_right = nullptr;   // right child
        m_left = nullptr;    // left child
        m_npl = 0;
    }
    Patient(int patient, int temp, int ox, int rr, int bp, int op) {
        if ( (temp < MINTEMP || temp > MAXTEMP) || 
        (ox < MINOX || ox > MAXOX) || (rr < MINRR || rr > MAXRR) || 
        (bp < MINBP || bp > MAXBP) || (op < 1 || op > 10)){
            // create an empty object
            m_patient = patient; m_temperature = 37; m_oxygen = 100;
            m_RR = 20; m_BP = 100;m_opinion=10;
        }
        else{
            m_patient = patient; m_temperature = temp; m_oxygen = ox;
            m_RR = rr; m_BP = bp;m_opinion=op;
        }
        m_right = nullptr;   // right child
        m_left = nullptr;    // left child
        m_npl = 0;
    }
    void setNPL(int npl) {m_npl = npl;}
    int getNPL() const {return m_npl;}
    int getPatient() const {return m_patient;}
    int getTemperature() const {return m_temperature;}
    int getOxygen() const {return m_oxygen;}
    int getRR() const {return m_RR;}
    int getBP() const {return m_BP;}
    int getOpinion() const {return m_opinion;}
    void setPatient(int patient) {m_patient=patient;}
    void setTemperature(int val) {m_temperature=val;}
    void setOxygen(int val) {m_oxygen=val;}
    void setRR(int val) {m_RR=val;}
    void setBP(int val) {m_BP=val;}
    void setOpinion(int val) {m_opinion=val;}
    // Overloaded assignment operator
    const Patient & operator=(const Patient& rhs){
        if (this != &rhs){
            m_patient = rhs.m_patient;
            m_temperature = rhs.m_temperature;
            m_oxygen = rhs.m_oxygen;
            m_RR = rhs.m_RR;
            m_BP = rhs.m_BP;
            m_opinion = rhs.m_opinion;
        }
        return *this;
    }
    // Overloaded equality operator
    bool operator==(const Patient & rhs) const {
        return ((m_patient == rhs.m_patient) &&
                (m_temperature == rhs.m_temperature) &&
                (m_oxygen == rhs.m_oxygen) &&
                (m_RR == rhs.m_RR) &&
                (m_BP == rhs.m_BP) &&
                (m_opinion == rhs.m_opinion));
    }
    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Patient& patient);

    private:
    int m_patient;     // Patient's ID, unique
    int m_temperature; // Body temperature, celsius
    int m_oxygen;      // Level of oxygen saturation (SpO2), percentage
    int m_RR;          // Respiratory Rate, per minute
    int m_BP;          // Blood Pressure
    int m_opinion;     // Nurse opinion, 1 - 10

    Patient * m_right;   // right child
    Patient * m_left;    // left child
    int m_npl;           // null path length for leftist heap
};

class Hospital {
    // stores the skew/leftist heap, minheap/maxheap
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // contains student test functions
    friend class Region; 
    Hospital();
    Hospital(prifn_t priFn, HEAPTYPE heapType, 
            STRUCTURE structure, int regPrior,
            int id);
    ~Hospital();
    Hospital(const Hospital& rhs);
    Hospital& operator=(const Hospital& rhs);
    bool insertPatient(const Patient& input);
    Patient getNextPatient();
    void mergeWithQueue(Hospital& rhs);
    void clear();
    int numPatients() const;
    // Print the queue using preorder traversal.  Although the first patient
    // printed should have the highest priority, the remaining patients will
    // not necessarily be in priority order.
    void printPatientQueue() const;
    prifn_t getPriorityFn() const;
    // Set a new priority function.  Must rebuild the heap!!!
    void setPriorityFn(prifn_t priFn, HEAPTYPE heapType);
    HEAPTYPE getHeapType() const;
    STRUCTURE getStructure() const;
    // Set a new data structure (skew/leftist). Must rebuild the heap!!!
    void setStructure(STRUCTURE structure);
    void dump() const;  // For debugging purposes.

    private:
    int m_ID;               // unique identifier
    Patient * m_heap;       // Pointer to root of skew heap
    int m_size;             // Current size of the heap
    prifn_t m_priorFunc;    // Function to compute priority
    HEAPTYPE m_heapType;    // either a MINHEAP or a MAXHEAP
    STRUCTURE m_structure;  // skew heap or leftist heap
    int m_regPrior;         // this holds the priority of the hospital

    void dump(Patient *pos) const; // helper function for dump

    /******************************************
    * Private function declarations go here! *
    ******************************************/
    void clearHelper(Patient * patient);
    Patient * mergeHelper(Patient * a, Patient * b);
    bool higherPrior(Patient * lhs, Patient * rhs, HEAPTYPE type);
    void swapNodes(Patient * &lhs, Patient * &rhs);
    Patient * copyHelper(Patient * rhsRoot);
    void preOrderTraversal(Patient * root) const;
    Patient * heapify(Patient * root);

};
class Region{
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // for testing purposes
    Region(int size);
    ~Region();
    bool addHospital(Hospital & hospital); // enqueue function
    bool getHospital(Hospital & hospital); // dequeue function
    bool getPatient(Patient & patient);
    bool getNthHospital(Hospital & hospital, int n);
    void dump(); // For debugging purposes
    // change priority function for the Nth highest priority hospital
    bool setPriorityFn(prifn_t priFn, HEAPTYPE heapType, int n);
    // change structure for the Nth highest  priority hospital
    bool setStructure(STRUCTURE structure, int n);
    private:
    Hospital * m_heap;          // Array to hold the heap
    int m_capacity;             // size of array
    int m_size;                 // Current size of the heap

    void dump(int index);       // helper function for dump

    /******************************************
     * Private function declarations go here! *
     ******************************************/
    void enqueHelper(int index);
    void dequeueHelper(int index);
    

};
#endif


