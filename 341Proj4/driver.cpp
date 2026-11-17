// CMSC 341 - Fall 2026 - Project 4
#include "dealer.h"
#include <random>
#include <vector>
enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL };
class Random {
  public:
    Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50,
           int stdev = 20)
        : m_min(min), m_max(max), m_type(type) {
        if (type == NORMAL) {
            // the case of NORMAL to generate integer numbers with normal
            // distribution
            m_generator = std::mt19937(m_device());
            // the data set will have the mean of 50 (default) and standard
            // deviation of 20 (default) the mean and standard deviation can
            // change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean, stdev);
        } else if (type == UNIFORMINT) {
            // the case of UNIFORMINT to generate integer numbers
            //  Using a fixed seed value generates always the same sequence
            //  of pseudorandom numbers, e.g. reproducing scientific experiments
            //  here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10); // 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min, max);
        } else { // the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10); // 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,
                                                               (double)max);
        }
    }
    void setSeed(int seedNum) {
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor
        // call this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum() {
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if (m_type == NORMAL) {
            // returns a random number in a set with normal distribution
            // we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max)
                result = m_normdist(m_generator);
        } else if (m_type == UNIFORMINT) {
            // this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum() {
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

  private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;     // normal distribution
    std::uniform_int_distribution<> m_unidist; // integer uniform distribution
    std::uniform_real_distribution<double>
        m_uniReal; // real uniform distribution
};
class Tester {};

unsigned int hashCode(const string str);

string carModels[5] = {"challenger", "stratos", "gt500", "miura", "x101"};
string dealers[5] = {"super car", "mega car", "car world", "car joint",
                     "shack of cars"};

int main() {

    vector<Car> dataList;
    Random RndID(MINID, MAXID);
    Random RndCar(0, 4); // selects one from the carModels array
    Random RndQuant(MINQUANTITY, MAXQUANTITY);
    CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
    bool result = true;
    for (int i = 0; i < 49; i++) {
        // generating random data
        Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuant.getRandNum(),
                          RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the CarDB object
        if (!cardb.insert(dataObj))
            cout << "Did not insert " << &dataObj << endl;
    }
    // try to delete some data node
    cout << "Removing data node " << &dataList[5] << endl;
    cardb.remove(dataList[5]);
    cout << "Removing data node " << &dataList[15] << endl;
    cardb.remove(dataList[15]);

    // dumping the data points to the standard output
    cardb.dump();

    // checking whether all data points are there
    cout << endl << "Checking whether all data exist in the DB:" << endl;
    for (vector<Car>::iterator it = dataList.begin(); it != dataList.end();
         it++) {
        Car anObj = cardb.getCar((*it).getModel(), (*it).getDealer());
        bool foundIt = (*it == anObj);
        result = result && foundIt;
        if (!foundIt) {
            cout << "\tData point " << (*it).getModel() << "("
                 << (*it).getDealer() << ")" << " is missing!" << endl;
        }
    }
    if (result)
        cout << "\tAll data points exist!\n";

    return 0;
}
unsigned int hashCode(const string str) {
    unsigned int val = 0;
    const unsigned int thirtyThree = 33; // magic number from textbook
    for (int i = 0; i < str.length(); i++)
        val = val * thirtyThree + str[i];
    return val;
}
