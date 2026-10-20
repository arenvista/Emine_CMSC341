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

    bool testAVLBalanceAfterInsertions(); //done
    bool testBSTPropAfterInsertions(); //done
    bool testSplayTreeInsertMovesRoot(); //done
    bool testAVLBalanceAfterRemoval(); //done
    bool testBSTPropAfterRemovals(); //done
    bool testRemoveMultipleDrones(); //done
    bool testFindMultipleDrones(); //done
    bool testShowCountAndState();  //done
    bool testShowSetState();      //done
    bool testBSTPropAfterMultipleSplayOperations(); //done
    bool testAVLBalanceAfterRemoveDead(); //if i have time left
    bool testBSTAfterRemoveDead(); // if I have time left
    bool testGroupRejectsInvalidAndDuplicateInsert();//done
    bool testShowRejectsInvalidAndDuplicateInsert();//done

    private:
    int getActualHeight(Drone * drone);
    bool isAVLBalanced(Drone * drone);
    int countAVLNodes(Drone * drone);
    bool isAVLStillBST(Drone * drone, int minId, int maxId);
    void fillIDArray(int idArray[], int minId, int maxId, int groupSize);
    bool checkIDUniquness(int idArray[], int groupSize, int id);
    bool checkSplayStillBST(Group * group, int minId, int maxId);
    int countSplayNodes(Group * group);
    
    
};

int main(){
    
    Tester tester;
    
    cout << "Test whether the AVL trees are balanced after 100 insertions" << endl;

    tester.testAVLBalanceAfterInsertions() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;
    
    cout << "Test whether the BST property of the AVL trees after 100 insertions." << endl;

    tester.testBSTPropAfterInsertions() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test whether the AVL trees are balanced after multiple removals." << endl;

    tester.testAVLBalanceAfterRemoval() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test whether the BST property is preserved after multiple removals from the AVL trees." << endl;

    tester.testBSTPropAfterRemovals() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test whether the function Group::remove works correctly for 50 drone removals" << endl;

    tester.testRemoveMultipleDrones() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test whether the function Group::findDroneworks correctly for 50 drone searches" << endl;

    tester.testFindMultipleDrones() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test whether the Splay tree performs the splay operations when inserting." << endl;

    tester.testSplayTreeInsertMovesRoot() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test whether the function Show::count(...) returns a correct value for a specific state" << endl;

    tester.testShowCountAndState() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test whether the function Show::setState(...) works correctly" << endl;

    tester.testShowSetState() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test whether the BST property is preserved in the Splay tree after multiple Splay operations." << endl;

    tester.testBSTPropAfterMultipleSplayOperations() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Tests that Group object rejects invalid IDs" << endl;

    tester.testGroupRejectsInvalidAndDuplicateInsert() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Tests that Show object rejects invalid IDs" << endl;

    tester.testShowRejectsInvalidAndDuplicateInsert() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    
    cout << "Tests that group can remove dead correctly" << endl;

    tester.testAVLBalanceAfterRemoveDead() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;
    
    return 0;
}
//helper function to get the height of passed in drone
int Tester::getActualHeight(Drone * drone){
    
    if (drone == nullptr){
        return -1;
    }
    //recursive calls to go left and right through tree
    int leftHeight = getActualHeight(drone->m_left);
    int rightHeight = getActualHeight(drone->m_right);

    //pick the bigger height
    if (leftHeight > rightHeight){
        return 1 + leftHeight;
    }

    return 1 + rightHeight; 
}

//helper function that checks if the AVL tree is height balanced at all nodes
bool Tester::isAVLBalanced(Drone * drone){

    if (drone == nullptr){
        return true; 
    }

    int leftHeight = getActualHeight(drone->m_left);
    int rightHeight = getActualHeight(drone->m_right);
    int balanceFactor = (leftHeight - rightHeight);

    if (balanceFactor < -1){
       balanceFactor = (balanceFactor * -1); 
    }

    if (balanceFactor > 1){
        
        return false;
    }
    
    if (getActualHeight(drone) != drone->getHeight()){
      return false;
    }
    //checks the tree recursivley by going through both the left and right subtrees too
    if (isAVLBalanced(drone->m_left) == false || isAVLBalanced(drone->m_right) == false){
        return false; 
    }
    return true;
}

//helper function to count the nodes in the AVL tree, uses recursion
int Tester::countAVLNodes(Drone * drone){
    if (drone == nullptr){
        return 0; 
    }

    return 1 + countAVLNodes(drone->m_left) + countAVLNodes(drone->m_right);
}

//helper function that checks if the AVL tree still is a binary search tree
bool Tester::isAVLStillBST(Drone * drone, int minId, int maxId){

    if (drone == nullptr){
        return true;
    }

    if (drone->m_id < minId || drone->m_id > maxId){
        return false; 
    }
    //goes through the left subtree recursivley
    if (!(isAVLStillBST(drone->m_left, minId, drone->m_id))){
        return false;
    }
    //goes through right subtree recurisvley
    if (!(isAVLStillBST(drone->m_right, drone->m_id, MAXID))){
        return false; 
    }

    return true;
}

//helper function that creates an array of all the ids used that were generated by random class generator
void Tester::fillIDArray(int idArray[], int minId, int maxId, int groupSize){

    Random idGen(minId,maxId);


    int usedNums = 0;
    int count = 0; 

    while( usedNums < groupSize){

        int id = idGen.getRandNum();
        if (checkIDUniquness(idArray, count, id)){
            idArray[count] = id;
            usedNums += 1; 
            count += 1; 
        }
    }
}

//checks that id was not used before when inserting a new one in the array
bool Tester::checkIDUniquness(int idArray[], int count, int id){
    for (int i = 0; i < count; i++){
        if (idArray[i] == id){
            return false; 
        }
    }
    return true; 
}

//helper function that checks if the splay tree is still a binary search tree
bool Tester::checkSplayStillBST(Group * group, int minId, int maxId){
        if (group == nullptr){
        return true;
    }

    if (group->m_groupID < minId || group->m_groupID > maxId){
        return false; 
    }
    //recurisvley goes through left subtree
    if (!(checkSplayStillBST(group->m_left, minId, group->m_groupID))){
        return false;
    }
    //recurivley goes through right subtree
    if (!(checkSplayStillBST(group->m_right, group->m_groupID, MAXID))){
        return false; 
    }

    return true;
}

int Tester::countSplayNodes(Group * group){
    if (group == nullptr){
        return 0; 
    }

    return 1 + countSplayNodes(group->m_left) + countSplayNodes(group->m_right);
}

/*Test whether the AVL trees are balanced after a decent number of insertions,
 e.g. inserting the provided database. 
(Note: this requires visiting all nodes and checking the height values are correct.)*/

bool Tester::testAVLBalanceAfterInsertions(){
    //creates a new group
    Group group;
    int groupSize = 100;
    Random idGen(MINID,MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2);
    //fills the group (avl tree) with the drones 
    for (int i = 0; i < groupSize; i++) {

        stateGen.getRandNum();

        Drone drone(idGen.getRandNum(),
                    static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                    static_cast<STATE>(stateGen.getRandNum()));
        group.insert(drone);
    }
     
    //calls helper functions to check if it is height balanced and if it has the correct amount of nodes
    if (isAVLBalanced(group.m_root) == false){
       return false; 
    }

    if (countAVLNodes(group.m_root) != groupSize){
       return false;
    }

    return true; 
 }

// Test whether the BST property of the AVL trees is preserved after all insertions.

bool Tester::testBSTPropAfterInsertions(){
    //creates new group
    Group group;
    int groupSize = 100;
    Random idGen(MINID,MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2);
    //inserts the new drones into the group
    for (int i = 0; i < groupSize; i++) {

        stateGen.getRandNum();

        Drone drone(idGen.getRandNum(),
                    static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                    static_cast<STATE>(stateGen.getRandNum()));
        group.insert(drone);
    }
    //checks after all the insertions if the AVL tree is still a binary search tree by calling helper function
    if(!isAVLStillBST(group.m_root, MINID, MAXID)){
        return false; 
    }

    return true;
}

/*
Test whether the Splay tree performs the splay operations when inserting. 
For example, we can insert multiple nodes in the splay tree and after every insertion 
we check whether the inserted node is at root and the tree preserves the BST property.
*/

bool Tester::testSplayTreeInsertMovesRoot(){
    //create a splay tree
    Show aShow;
    //array that stores all the ids of the groups in the splay tree
    int groupIDS[25];
    Random idGen(MINID, MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2); 
    //fill that groups id array to keep track of what id numbers were used
    fillIDArray(groupIDS, MINID, MAXID, 25); 
        
        //go through each group and fill it 
        for (int i = 0; i < 25; i++){
            //another array used to insert the drones into that certain group
            Drone drones[10];
            
            for(int j = 0; j < 10; j++){
                Drone drone(idGen.getRandNum(),
                            static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                            static_cast<STATE>(stateGen.getRandNum()));
                drones[j] = drone;
            }

            bool inserted = aShow.insert(groupIDS[i], drones, 10);

            //checks that group was inserted
            if (!inserted){
                return false;
            }
            //checks that root is no longer nullptr if insertion happened
            if (aShow.m_root == nullptr){
                return false;
            }
            //checks if for each group insertion that the new head is that insert group
            if (aShow.m_root->m_groupID !=  groupIDS[i]){
                return false;
            } 

            //checks that tree is maintain bst property after each insertion of a group
            if (!checkSplayStillBST(aShow.m_root, MINID, MAXID)){
                return false; 
            }
        }

    return true; 
}

/*
Test whether the AVL trees are balanced after multiple removals. 
For example, insert database, then remove half of the drones, and check the AVL property.
*/

bool Tester::testAVLBalanceAfterRemoval(){
    //creates  group
    Group group;
    int groupSize = 100;
    Random typeGen(0,2);
    Random stateGen(0,2);

    //array to store chosen ids to keep track 
    int idArray[100]; 

    //fills that array
    fillIDArray(idArray, MINID, MAXID, groupSize);

    for (int i = 0; i < groupSize; i++) {

        stateGen.getRandNum();

        Drone drone(idArray[i],
                    static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                    static_cast<STATE>(stateGen.getRandNum()));
        group.insert(drone);
    }
    //remove half of the drones
    for (int i = 0; i < 50; i++){
        group.remove(idArray[i]);
    }
    //after deltions check si avl is still height balanced
    if (!(isAVLBalanced(group.m_root))){
        return false; 
    }
    //checks that the amount od drones is equalt to the total - deleted number of drones
    if (countAVLNodes(group.m_root) != 50){
        return false; 
    }

    return true; 
}

//Test whether the BST property is preserved after multiple removals from the AVL trees.

bool Tester::testBSTPropAfterRemovals(){
    //creates a group
    Group group;
    int groupSize = 100;
    Random typeGen(0,2);
    Random stateGen(0,2);
    
    //array to store chosen ids for the drones
    int idArray[100]; 

    //genrate the id numbers randomly using helper function
    fillIDArray(idArray, MINID, MAXID, groupSize);

    //create and insert each drone
    for (int i = 0; i < groupSize; i++) {

        stateGen.getRandNum();

        Drone drone(idArray[i],
                    static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                    static_cast<STATE>(stateGen.getRandNum()));
        group.insert(drone);
    }

    //remove half of the drones
    for (int i = 0; i < 50; i++){
        group.remove(idArray[i]);
    }

    //check that the tree still keeps the bst properties
    if (!(isAVLStillBST(group.m_root, MINID, MAXID))){
        return false; 
    }

    //check that the right amount of drones are left
    if (countAVLNodes(group.m_root) != 50){
        return false; 
    }

    
    return true; 
}

/*
Test whether the function Group::remove(...) works correctly for multiple drone removals, 
e.g. 50 removals.
*/

bool Tester::testRemoveMultipleDrones(){
    //creates new group object
    Group group;
    int groupSize = 100;
    Random typeGen(0,2);
    Random stateGen(0,2);

    //array to store chosen id numbers
    int idArray[100]; 

    //fills array randomly with id numbers
    fillIDArray(idArray, MINID, MAXID, groupSize);

    //creates and inserts each drone
    for (int i = 0; i < groupSize; i++) {

        stateGen.getRandNum();

        Drone drone(idArray[i],
                    static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                    static_cast<STATE>(stateGen.getRandNum()));
        group.insert(drone);
    }
    
    //removes 50 drones
    for (int i = 0; i < 50; i++){

        //checks that drone is in there first, if it's not there something went wrong
        if (!(group.findDrone(idArray[i]))){
            return false; 
        }

        //remove drone
        group.remove(idArray[i]);

        //if drone is found after removal something went wrong
        if ((group.findDrone(idArray[i]))){
            return false; 
        }
    }
    //check that the right amount of drones are left
    if(countAVLNodes(group.m_root) != 50){
        return false; 
    }

    return true; 
}

/*
Test whether the function Group::findDrone(...) works correctly for multiple drone searches, 
e.g. 50 searches.
*/

bool Tester::testFindMultipleDrones(){
    //creates new group
    Group group;
    int groupSize = 100;
    Random typeGen(0,2);
    Random stateGen(0,2);

    //array that stores all of the chosen ids
    int idArray[100]; 
    //helper function fills the array with random id numbers using generator
    fillIDArray(idArray, MINID, MAXID, groupSize);

    //create and insert new drones
    for (int i = 0; i < groupSize; i++) {

        stateGen.getRandNum();

        Drone drone(idArray[i],
                    static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                    static_cast<STATE>(stateGen.getRandNum()));
        group.insert(drone);
    }
    
    //looks for 50 drones that should exits
    for (int i = 0; i < 50; i++){
        if (!(group.findDrone(idArray[i]))){
            return false; 
        }
    }

    return true; 
}

/*
Test whether the function Show::count(...) returns a correct value for a specific state, 
i.e. it returns a correct value and it performs the splay operation.
*/

bool Tester::testShowCountAndState(){
    //creates a show object
    Show aShow;
    //array that stores ids of each group in show object
    int groupIDS[10];
    Random idGen(MINID, MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2); 
    //fills the array with random ids
    fillIDArray(groupIDS, MINID, MAXID, 10);
    //chosen group 
    int targetIndex = 3; 
    //chosen group id
    int targetGroupID = groupIDS[targetIndex];
    //counter
    int expectedStateCount = 0; 
    //chosen state to count
    STATE targetState = static_cast<STATE>(0);
        
        for (int i = 0; i < 10; i++){
            Drone drones[10];
            
            for(int j = 0; j < 5; j++){
                STATE state = static_cast<STATE>(stateGen.getRandNum()); 
                Drone drone(idGen.getRandNum(),
                            static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                            state);
                drones[j] = drone;
                //increases count if for the chosen group if the chosen state drone was inserted
                if (groupIDS[i] == targetGroupID && state == targetState){
                    expectedStateCount += 1; 
                }
            }

            if (aShow.insert(groupIDS[i], drones, 5)  == false){
                return false; 
            }
        }
        //counts the drones in that chosen group with that state
        int actualCount = aShow.count(targetGroupID, targetState);
        //checks that the count of actual = the expected
        if (actualCount != expectedStateCount){
            return false; 
        }

        //checks that splay opertaion happened
        if ( aShow.m_root == nullptr || aShow.m_root->m_groupID != targetGroupID){
            return false; 
        }

    return true; 
    
}

/*
Test whether the function Show::setState(...) works correctly, i.e.
it returns a correct value and it performs the splay operation.
*/

bool Tester::testShowSetState(){
//creates a show obhect
Show aShow;
//array to store random id numbers for the groups
int groupIDS[10];
Random idGen(MINID, MAXID);
Random typeGen(0,2);
Random stateGen(0,2); 
//fils that id array for the groups randomly
fillIDArray(groupIDS, MINID, MAXID, 10);

//2d array to keepy track of all of the drones among all groups
int droneIDs[10][5];

    
    for (int i = 0; i < 10; i++){
        Drone drones[10];
        
        for(int j = 0; j < 5; j++){
            int curDroneID = idGen.getRandNum();
            //stores that drone's id
            droneIDs[i][j] = curDroneID; 
            Drone drone(curDroneID,
                        static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                        static_cast<STATE>(stateGen.getRandNum()));
            drones[j] = drone;

        }

        if (aShow.insert(groupIDS[i], drones, 5) == false){
            return false; 
        }

    }
    //randomly pick a group and a drones
    Random pickGroup(0,9);
    int targetIndex = pickGroup.getRandNum(); 
    int targetGroupID = groupIDS[targetIndex];
    Random pickDrone(0,4);
    int targetDroneID = droneIDs[targetIndex][pickDrone.getRandNum()];
    STATE newState = static_cast<STATE>(2); //change drone to dead state
    //set the state of that drone
    if (aShow.setState(targetGroupID, targetDroneID, newState) == false){
        return false; 
    }
    //check that splay happened
    if (aShow.m_root == nullptr || aShow.m_root->m_groupID != targetGroupID){
        return false; 
    }

    return true; 

}

/*
Test whether the BST property is preserved in the Splay tree after multiple Splay operations.
*/

bool Tester::testBSTPropAfterMultipleSplayOperations(){
    //creates a show obhect
    Show aShow;
    //array to keep track of ids
    int groupIDS[15];
    Random idGen(MINID, MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2); 
    //fill array with group ids randomly
    fillIDArray(groupIDS, MINID, MAXID, 15); 
        
        for (int i = 0; i < 15; i++){
            Drone drones[10];
            
            for(int j = 0; j < 10; j++){
                Drone drone(idGen.getRandNum(),
                            static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                            static_cast<STATE>(stateGen.getRandNum()));
                drones[j] = drone;
            }

            bool inserted = aShow.insert(groupIDS[i], drones, 10);
            //checs that each group was inserted
            if (!inserted){
                return false;
            }
            //checks that splay function happened
            if (aShow.m_root == nullptr){
                return false;
            }
            //checks that splay function happened
            if (aShow.m_root->m_groupID !=  groupIDS[i]){
                return false;
            } 
            //checks that tree is still mantaing bst properties
            if (!checkSplayStillBST(aShow.m_root, MINID, MAXID)){
                return false; 
            }
        }
    //generators to pick random group and state for splay operations 
    Random pickGroup(0,14);
    Random pickState(0,2);
    //iterating 15 times to do alot of actions that require splay()
    for (int i = 0; i < 15; i++){
        
        int chosenGroupID = groupIDS[pickGroup.getRandNum()];

        STATE chosenState = static_cast<STATE>(pickState.getRandNum());

        aShow.count(chosenGroupID, chosenState);
        //checks splay was done
        if (aShow.m_root == nullptr || aShow.m_root->m_groupID != chosenGroupID){
            return false; 
        }
        //checks that bst property is mantained after each splay operation for that iteration
        if (checkSplayStillBST(aShow.m_root, MINID, MAXID) == false){
            return false; 
        }
    }

    return true; 
}

bool Tester::testAVLBalanceAfterRemoveDead(){
    //creates a new group
    Group group;
    int groupSize = 100;
    Random idGen(MINID,MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2);
    
    int deadCount = 0;

    //fills the group (avl tree) with the drones 
    for (int i = 0; i < groupSize; i++) {

        stateGen.getRandNum();
        STATE curState = static_cast<STATE>(stateGen.getRandNum());

        Drone drone(idGen.getRandNum(),
                    static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                    curState);

        if (curState == DEAD){
            deadCount += 1;
        }

        group.insert(drone);
    }
     
    //calls helper functions to check if it is height balanced and if it has the correct amount of nodes
    if (isAVLBalanced(group.m_root) == false){
       return false; 
    }

    if (countAVLNodes(group.m_root) != groupSize){
       return false;
    }

    group.removeDead();

    if (isAVLBalanced(group.m_root) == false){
       return false; 
    }

    if (isAVLStillBST(group.m_root, MINID, MAXID) == false){
        return false;
    }

    if (countAVLNodes(group.m_root) != (groupSize - deadCount)){
       return false;
    }

    return true; 
}


bool Tester::testGroupRejectsInvalidAndDuplicateInsert(){
    Group group;
    Random validIDGen(MINID, MAXID);
    int validID = validIDGen.getRandNum();

    Drone validDrone(validID, DEFAULT_LIGHT, DEFAULT_STATE);
    Drone duplicateDrone(validID, DEFAULT_LIGHT, DEFAULT_STATE);
    Drone lowInvalidDrone(MINID - 1, DEFAULT_LIGHT, DEFAULT_STATE);
    Drone highInvalidDrone(MAXID + 1, DEFAULT_LIGHT, DEFAULT_STATE);

    group.insert(validDrone);
    group.insert(duplicateDrone);
    group.insert(lowInvalidDrone);
    group.insert(highInvalidDrone);

    if (countAVLNodes(group.m_root) != 1){
        return false; 
    }

    if (group.findDrone(validID) == false){
        return false;
    }

    return true; 
}

bool Tester::testShowRejectsInvalidAndDuplicateInsert(){

    Show show;
    Random IDGen(MINID, MAXID);
    Random typeGen(0,2);
    Random stateGen(0,2);
    int validID = IDGen.getRandNum();

    int groupSize = 5;
    Drone drones[5];

    for (int i = 0; i < 5; i++){
        Drone drone(IDGen.getRandNum(),
                            static_cast<LIGHTCOLOR>(typeGen.getRandNum()),
                            static_cast<STATE>(stateGen.getRandNum()));
        drones[i] = drone;

    }

    if (show.insert(validID, drones, groupSize) == false){
        return false;
    }

    if (show.insert(validID, drones, groupSize) == true){
        return false;
    }

    if (show.insert(MINID - 1, drones, groupSize) == true){
        return false;
    }

    if (show.insert(MAXID + 1, drones, groupSize) == true){
        return false;
    }

    if (countSplayNodes(show.m_root) != 1){
        return false;
    }
    return true;
}

    