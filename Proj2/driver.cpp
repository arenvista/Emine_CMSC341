// UMBC - CMSC 341 - Fall 2026 - Proj2
#include "show.h"
using namespace std;
#include <vector>
#include <random>
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
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
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{ // Tester class to implement test functions
    public:
};
int main(){
    Random idGen(MINID,MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2);
    {
        Group group;
        int groupSize = 10;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<groupSize;i++){
            ID = idGen.getRandNum();
            if (i == groupSize / 2) tempID = ID;//we store this ID for later use
            Drone drone(ID,
                        static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                        static_cast<STATE>(stateGen.getRandNum()));
            group.insert(drone);
        }
        cout << "\nDump after inserting " << groupSize << " nodes:\n\n";
        group.dumpTree();
        cout << "\n\nList of Drones after inserting " << groupSize << " nodes:\n";
        group.listDrones();
        cout << endl;

        group.remove(tempID);
        cout << "\nDump after removig the node with ID: " << tempID << "\n\n";
        group.dumpTree();
        cout << "\n\nList of Drones after removing the node with ID: " << tempID << "\n";
        group.listDrones();
        cout << endl;
    }
    
    {
        cout<< "\nConstructing a show tree ..." << endl;
        Show aShow;
        
        for (int i=0; i<10; i++){
            Drone drones[10];
            int groupSize = 1;
            for(int i=0;i<groupSize;i++){
                Drone drone(idGen.getRandNum(),
                            static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                            static_cast<STATE>(stateGen.getRandNum()));
                drones[i] = drone;
            }
            aShow.insert(idGen.getRandNum(), drones, 10);
        }
        cout << "\nDump of the show tree:\n" << endl;
        aShow.dump();
    }
    
    return 0;
}
