// UMBC - CMSC 341 - Fall 2026 - Proj2
#include "show.h"

//default constructor iniatlizes the object and makes it empty
Group::Group(){ 
    m_root = nullptr;
    m_groupID = DEFAULT_ID;
    m_left = nullptr;
    m_right = nullptr;
}

//constructor that takes in different data to create and obhect 
Group::Group(int group, Drone drones[], int population){
    m_groupID = group;
    m_root = nullptr;
    m_left = nullptr;
    m_right = nullptr;
    for (int i = 0; i<population; i++){
        insert(drones[i]);
    }
}

//destructor that calls clear to handel the deletion
Group::~Group(){
    clear();
}

//insert function calls the recursive helper function
void Group::insert(const Drone& drone){
    //checks that id is valid first
    if (drone.getID() >= MINID && drone.getID() <= MAXID){
        //calls the recursive helper function
        m_root = insert(m_root, drone);
    
    }  
    //else{
        //cout << "ID " << drone.getID() << " is not valid " << endl;
    //}
    return; 
}

//helper function that updates the height of that passed in drone
void Group::updateHeight(Drone* drone){
    //checks that drone is valid
    if (drone != nullptr){
        //saves both the height of the left child and right child
        int leftHeight = (height(drone->getLeft()));
        int rightHeight = (height(drone->getRight()));

        //the height of the drone is determined by the bigger child height
        drone->setHeight(1 + (leftHeight > rightHeight ? leftHeight : rightHeight));
        return;
    }
    else{
        //cout << "Unable to complete update, node is nullptr" << endl;
        return;
    }
}

//helper function that calculates the helper function
int Group::balanceFactor(Drone* drone){

    //variables that store the height of the left and right children for calculations
    int left;
    int right;

    //checks if the left child exists
    if(drone->getLeft() == nullptr){
        left = -1;
    }

    //if not then return the height of that child
    else {left = (drone->getLeft()->getHeight());}

    //checks if the right child exits
    if(drone->getRight() == nullptr){
        right= -1;
    }
    //if not then return the height of that child
    else {right = (drone->getRight()->getHeight());}

    //the balance factor is equal to the left height - right height
    int factor = (left - right);

    return factor;
}

//helper function that recursivley balances the avl tree
Drone* Group::rebalanceTree(Drone* drone){

    //checks that drone is valid
    if (drone == nullptr){
        return nullptr;
    }

    //checks the balance factor of the drone
    int factor = balanceFactor(drone);

    //checks if the factor is greater then 1, then left side is heavier
    if (factor > 1){ // if left needs fixing

        //recursive call
        drone->m_left = rebalanceTree(drone->m_left);

        //if the right side of that left child has the imbalance then double rotation is needed
        if(balanceFactor((drone->getLeft())) < 0){
            drone ->setLeft(left(drone->getLeft()));
        }
        //updated heights of the drones
        updateHeight(drone);
        //returns the drone that was rotated into that spot
        return right(drone);
     
    //if the factor is negative then the right side is heavy
    } else if (factor < -1){ // if right needs fixing

        //recursive call
        drone->m_right = rebalanceTree(drone->m_right);

        //chekcs if the left side of the right child has the imbalance if so a double roatation is needed
        if(balanceFactor((drone->getRight())) == 1){
            drone->setRight(right(drone->getRight()));
        }
        //updates the heights of touched drones
        updateHeight(drone);
        //returns the drone tha was rotated into that posistion
        return left(drone);
    
    } else { // finally found a balanced drone
        updateHeight(drone);
        return drone;
    }

    return drone;
}

//heler function to recursivley insert the drone
Drone * Group::insert(Drone* root, const Drone& drone){
        //im lost not sure what to do here
    //chekcs if the avl tree is empty and inserts new drone at the root spot
    if (root == nullptr){
       Drone* newDrone = new Drone(drone.getID(), drone.getType(), drone.getState());
       newDrone->setHeight(0);
       newDrone->setLeft(nullptr);
       newDrone->setRight(nullptr);
       return newDrone;
    
    // LESS THAN ROOT -> GO LEFT to insert the new drone
    } else if (drone.getID() < root->getID()){

        // WITH CHILDREN
        if (root->getLeft()){ 
            root->setLeft(insert(root->m_left, drone));
            updateHeight(root);
            return rebalanceTree(root); 

        // NO CHILDREN, INSERT HERE
        } else { 
            Drone* newDrone = new Drone(drone.getID(), drone.getType(), drone.getState());
            newDrone->setHeight(0);
            newDrone->setLeft(nullptr);
            newDrone->setRight(nullptr);
            root->setLeft(newDrone);
            updateHeight(root);
            return rebalanceTree(root); 
        }

    // GREATER THAN ROOT -> GO RIGHT
    } else if (drone.getID() > root->getID()){ 
        if (root->getRight()){ // WITH CHILDREN
            root->setRight(insert(root->m_right,drone));
            updateHeight(root);
            return rebalanceTree(root);
        
         // NO CHILDREN, INSERT HERE
        } else {
            Drone* newDrone = new Drone(drone.getID(), drone.getType(), drone.getState());
            newDrone->setHeight(0);
            newDrone->setLeft(nullptr);
            newDrone->setRight(nullptr);
            root->setRight(newDrone);
            updateHeight(root);
            return rebalanceTree(root);
        }
    }

    /*
    if (drone.getID() < root->getID()){
       root->setLeft( insert(root->m_left, drone));
       return root;
    }
    
    else if (drone.getID() > root->getID()){
        root->setRight(insert(root->m_right,drone));
    }
    */

    else{
        //cout << "No Duplicate ID's allowed!" << endl;
        return root;
    }

    // updateHeight(root);
    return rebalanceTree(root);
}

//calls the recursive clear function to handel the deletion
void Group::clear(){

    //check to ensure that tree isn't empty
    if(m_root == nullptr){return;}

    //cals the recursive futnction to handel clear()
    clear(m_root);

    //resets the object to empty
    m_root = nullptr;
    m_groupID = 0; 
}

//recursive clear helper function
void Group::clear(Drone* drone){

    if(drone == nullptr){return;}
    if(drone->m_left != nullptr){clear(drone->m_left);}
    if(drone->m_right != nullptr){clear(drone->m_right);}
    delete drone;
}

//remoe function calls the recurisve remove helper function 
void Group::remove(int id){

    m_root = remove(m_root, id);
}

//helper function that find the successor drone
Drone* Group::minDrone(Drone* drone){
    while(drone->getLeft() != nullptr){
        drone = drone->getLeft();
    }
    return drone;
}

//recursive remove helper function
Drone * Group::remove(Drone* drone, int id){
    /*
    IF FOUND
        temp = drone.child (Might be childen, which means you need to do math)
        delete drone;
        return temp;
    IF LESS
        m_left = remove(leftChild, target)
        return drone
    IF GREATER
        m_right = remove(rightChild, target)
        return drone

    EXPLANATION: Works its way down looking for target, then works its way back up returning edges
    (Last to first, skipping second to last (target))
    HARD THING: When there are two children, will have to think about this
    */

    //check to make sure drone is valid 
    if (drone == nullptr){
        return nullptr;
    }

    //if the drone's id is less then the cur, go left
    if(id < drone->getID()){
        
        drone->m_left = remove(drone->m_left, id);
        updateHeight(drone);
        return rebalanceTree(drone);
        
    }

    //if the drone's id is greater then the cur, go right
    else if (id > drone->getID()){
        
        drone->m_right = remove(drone->m_right, id);
        updateHeight(drone);
        return rebalanceTree(drone);
         
    }

    //if drone is found
    if(drone->getID() == id){

        //no children, leaf node, drone can just be deleted
        if (!drone->getRight() && !drone->getLeft()){ 
            delete drone;
            return nullptr;
        }

        //one child
        else if (!drone->getLeft() || !drone->getRight()){
            Drone * child = drone->getLeft() ? drone->getLeft() : drone->getRight(); //temp (kinda)
            delete drone;
            updateHeight(child);
            // rebalance subtree with root (param)
            return rebalanceTree(child); 
        }

        //2 children
        else if(drone->getLeft() && drone->getRight()){
            //find the succesor to replace that drone since it has 2 children
            Drone * child = minDrone(drone->getRight());
            drone->setID(child->getID());
            drone->setType(child->getType());
            drone->setState(child->getState());
            drone->m_right = remove(drone->m_right, child->getID());
            updateHeight(drone);
            // rebalance subtree with root (param)
            return rebalanceTree(drone);
            // return rebalanceTree(drone);
        
        }
    }
    
    updateHeight(drone);
    return rebalanceTree(drone);
}

//function calls the recursive listDrone helper function
void Group::listDrones() const {
    listDrone(m_root);
}

//recurisve helper function
void Group::listDrone(Drone* drone) const{
    if(drone == nullptr){return;}
    listDrone(drone->getLeft());
    cout << drone->getID() << ":" << drone->getTypeStr() << ":" << drone->getStateStr() << endl;
    listDrone(drone->getRight());
}

//sets/changes the state of the chosen drone
bool Group::setState(int id, STATE state){
    //find the drone first using the recurisve helper function
    Drone* drone = findDrone(m_root, id);
    if (drone == nullptr){
        return false;
    }
    drone->m_state = state;
    return true;

}

//calls the recurisve helper function
void Group::removeDead(){

    if (m_root == nullptr){
        return; 
    }

    
    int totalDrones = countDrones(m_root);

    int * deadDroneIds = new int[totalDrones];

    int deadDronesNum = 0;

    fillIDArray(m_root, deadDroneIds, deadDronesNum);

    for (int i = 0; i < deadDronesNum; i++){
        remove(deadDroneIds[i]);
    }
   delete [] deadDroneIds;  
}

//helper function to count the amount of drone nodes in avl tree
int Group::countDrones(Drone * drone){

    if (drone == nullptr){
        return 0;
    }
    //recurisve call
    return 1 + countDrones(drone->m_left) + countDrones(drone->m_right); 
}

void Group::fillIDArray(Drone * drone, int deadIDs[], int &deadDrones){

    if (drone == nullptr){
        return;
    }

    if (drone != nullptr){
        fillIDArray(drone->m_left, deadIDs, deadDrones);

        if(drone->getState() == DEAD){
            deadIDs[deadDrones] = drone->getID();
            deadDrones += 1; 
        }

        fillIDArray(drone->m_right, deadIDs, deadDrones);
    }
}

//calls the recurisve helper function
bool Group::findDrone(int id) const {
    Drone* drone = findDrone(m_root, id);
    if (drone == nullptr){
        return false;
    }
    return true;
}

//recursive find drone helper function
Drone* Group::findDrone(Drone* n, int id) const{
    
    //drone doesn't exits
    if(n == nullptr){
        //cout << "DID NOT FIND DRONE \n";
        return nullptr;}

    //found the drone
    if (n->getID() == id ){
        return n;
    }

    //go left if the id is smaller then cur drone id 
    if (n->getID() > id){
        if (n->getLeft()){
            return findDrone(n->m_left, id);
        }
        else{
            //cout << "DID NOT FIND DRONE \n";
            return nullptr;
        }
    }

    //go right if id is greater then the cur drone id
    if(n->getID() < id){
        if(n->getRight()){
        return findDrone(n->m_right, id);
        }
        else{
            //cout << "DID NOT FIND DRONE \n";
            return nullptr;
        }
    }
    return nullptr;
}

//helper function that recursivley goes through the tree to count 
int Group::traverseInOrderColor(Drone* drone, LIGHTCOLOR color) const{
    //checks if drone is empty
    if (drone == nullptr){
        return 0;
    }
    //stores the count
    int counter = 0; 

    //recursive call to go left
    counter += traverseInOrderColor(drone->m_left, color);

    if (drone->getType() == color){
        counter++;
    }
    //recursive call to go right
    counter += traverseInOrderColor(drone->m_right, color);

    return counter;
}

// calls the recursive traverals to handel the counting (traverseInOrderColor)
int Group::count(LIGHTCOLOR color) const{

    return traverseInOrderColor(m_root, color);
    
}

//the recurisve traverals function that counts the drones with that certain state
int Group::traverseInOrderState(Drone* drone, STATE state) const {

    //stores the count
    int counter = 0;

    //checks if drone is empty
    if (drone == nullptr){
        return 0;
    }
    //rcurisve call to fo left through tree

    counter += traverseInOrderState(drone->m_left, state);

    if (drone->getState() == state){
        counter++;
    }

    //recursive call to right through tree
    counter += traverseInOrderState(drone->m_right, state);

    return counter;
}

//cals the recurisve helper function to handel count (traverseInOrderState)
int Group::count(STATE state) const{
    
    return  traverseInOrderState(m_root, state);
}

//cals the recurisve helper function
void Group::dumpTree() const {
    dump(m_root);
}

//handels printing of tree recurisvley
void Group::dump(Drone* aDrone) const{
    if (aDrone != nullptr){
        cout << "(";
        dump(aDrone->m_left);//first visit the left child
        cout << aDrone->m_id << ":" << aDrone->m_height;//second visit the node itself
        dump(aDrone->m_right);//third visit the right child
        cout << ")";
    }
}

//helper function to handel right rotation
Drone* Group::right(Drone* drone){
    //checks drone is valid
    if (drone == nullptr){
        return nullptr;
    }
    //left child 
    Drone * y = drone->getLeft();
    if (y == nullptr){
        //cout << "rotation is not able to be completed" << endl;
        return nullptr; 
    }
    //right child of left child
    Drone * z = y->getRight();
    y->setRight(drone);
    drone->setLeft(z);

    updateHeight(drone);
    updateHeight(y);

    return y;
}

//helper function to handel left rotation
Drone* Group::left(Drone* drone){
    //checks if drone is valid
     if (drone == nullptr){
        return nullptr;
    }

    //right child of chosen drone
    Drone * y = drone->getRight();
        if (y == nullptr){
        //cout << "rotation is not able to be completed" << endl;
        return nullptr; 
    }
    //left child of the right child
    Drone * z = y->getLeft();
    y->setLeft(drone);
    drone->setRight(z);

    updateHeight(drone);
    updateHeight(y);
    
    return y;
}

//helper function that gets the height of the passed in drone, calls getHeight helper function to check
int Group::height(Drone* drone){
    if (drone == nullptr){
        return -1; //nullptr is -1 and leafnodes are 0
    }
    
    return drone->getHeight();
}

//default constructor, sets object to empty state
Show::Show(){
    m_root = nullptr;
}

//destructor that calls clear to handel deletion
Show::~Show(){
    if(m_root == nullptr){return;}
    clear(m_root);
    m_root = nullptr;
}
/*
This function inserts the drones in the tree. Since, the tree is a Splay tree, 
it needs to reorganize the tree after insertion. A duplicate key is not allowed.
 In the error cases such as invalid ID the function returns false.
  If the operation is successful the function. returns true.
*/
bool Show::insert(int group, Drone drones[], int population){

    //checks id is valid first
    if(group < MINID || group > MAXID){

        return false;
    }
    //creates a new group object
    Group * newGroup = new Group(group, drones, population);

    //if the tree is empty, then the head becomes the new group
    if (m_root == nullptr){
        m_root = newGroup;
        return true;
    }
    //if tree is filled then splay first 
    m_root = splay(m_root, group);

    //if group id already exits then delete that new group
    if (m_root == nullptr || m_root->m_groupID == group){
        delete newGroup;
        return false; 
    }

    //if id smaller then curr node id then the root gets pushed right
    else if (m_root->m_groupID > group){
        newGroup->m_left = m_root->m_left;
        newGroup->m_right = m_root;
        m_root->m_left = nullptr;
        m_root = newGroup;
        return true; 
    }

    //if id bigger then curr node id then the root gets pushed left
    else if (m_root->m_groupID < group){
        newGroup->m_right = m_root->m_right;
        newGroup->m_left = m_root;
        m_root->m_right  = nullptr; 
        m_root = newGroup;
        return true; 
    }

    delete newGroup; 
    return false;
}

//calls the recursive dump function
void Show::dump(bool verbose) const{
	dumpHelper(m_root, verbose);
    cout << endl;
}

//recurisve dump function helper to cout the tree
void Show::dumpHelper(Group* root, bool verbose) const{
    if (root != nullptr){
        {
            cout << "(";
            dumpHelper( root->m_left, verbose );
            if (verbose)
                cout << root->m_groupID << ":" << root->m_root->m_id;
            else
                cout << root->m_groupID;
            dumpHelper( root->m_right, verbose );
            cout << ")";
        }
    }

}

/*
This function finds all drones with a specific state and returns the total number of them.
 In the error cases the function returns zero.
*/

int Show::count(int group, STATE state){
    //checks that tree is filled
    if (m_root == nullptr){
        return 0;
    }

    //splays tree for chosen group
    m_root = splay(m_root, group);
    
    //checks that group was found
    if ( m_root == nullptr || m_root->m_groupID != group){
        return 0;
    }

    return m_root->count(state);
}
/*
This function removes the drone if the all flag is false.
If the flag is true, the function removes all dead drones.
The function returns true if the operation is successful.
In the error cases the function returns false.
*/

bool Show::removeDrone(int group, int drone, bool all){
    //checks that tree is empty
    if (m_root == nullptr){
        return false;
    }

    //splays tree for that group
    m_root = splay(m_root, group);

    //checks that group was found
    if ( m_root == nullptr || m_root->m_groupID != group){
        return false;
    }

    
    if (all){
        m_root->removeDead();
        return true;
    }

    else{
        m_root->remove(drone);
        return true;
    }

    return true;
}

//This function returns the height of the tree. In the error cases the function returns -1.
// return the height of the root of that group
int Show::getGroupHeight(int group){
    //checks that tree is empty or not
    if (m_root == nullptr){
        return -1;
    }

    //splays for that group
    m_root = splay(m_root, group);

    //checks that the group was found
    if ( m_root == nullptr || m_root->m_groupID != group){
        return -1;
    }

    return m_root->height(m_root->m_root);

}

/*

This function tries to change the current state of the drone.
If the operation is successful the function returns true,
otherwise it returns false.
*/

bool Show::setState(int group, int drone, STATE state){
    //checks if tree is empyt or not
    if(m_root == nullptr){
        return false; 
    }

    //splays for the chosen group
    m_root = splay(m_root, group);

    //checks if the drone was found
    if (m_root == nullptr || m_root->m_groupID != group){
        return false; 
    }

    m_root->setState(drone, state);
    return true;
}

//recurisve clear helper fucntion
void Show::clear(Group * group){
    if (group == nullptr){
        return;
    }

    if(group->m_left != nullptr){clear(group->m_left);}
    if(group->m_right != nullptr){clear(group->m_right);}

    delete group;
}

//helper function to handel the left rotation
Group * Show::left(Group * group){

    //checks that group is valid
    if (group == nullptr){
        return nullptr;
    }

    //right child of that group
    Group * y = group->m_right;

    if (y == nullptr){
        //cout << "rotation is not able to be completed" << endl;
        return nullptr; 
    }
    //left child of the right child
    Group * z = y->m_left;
    y->m_left = group;
    group->m_right = z;

    return y;

}

//helper function to handel the right rotation
Group * Show::right(Group * group){

    //checks that the group is valid
    if (group == nullptr){
        return nullptr;
    }

    //left child of chosen group
    Group * y = group->m_left;

    if (y == nullptr){
        //cout << "rotation is not able to be completed" << endl;
        return nullptr; 
    }

    //right child of the left child
    Group * z = y->m_right;
    y->m_right = group;
    group->m_left = z;

    return y;
}

//helper function that handels the splay operation
  Group * Show::splay(Group * group, int id){
    //checks if group doesn't exits or has been found and retunrs it 
    if (group == nullptr || group->m_groupID == id){
        return group; 
    }
    //in left subtree
    if (group->m_groupID > id){
        if (group->m_left == nullptr){
            return group;
        }
        //goes left if the id is less the left child id
        if (group->m_left->m_groupID > id){
            group->m_left->m_left = splay(group->m_left->m_left, id);
            group = right(group);
        }
        //goes right if id is greater then the right child id
        else if (group->m_left->m_groupID < id){
            group->m_left->m_right = splay(group->m_left->m_right, id);
            
            if (group->m_left->m_right != nullptr){
                group->m_left = left(group->m_left);
            }
            
        }
        return (group->m_left == nullptr) ? group : right(group);
    }
    // it is in the right subtree
    else{
        if (group->m_right == nullptr){
            return group; 
        }
        //goes left if id is less then the right child id
        if (group->m_right->m_groupID > id){
            group->m_right->m_left = splay(group->m_right->m_left, id);
            if(group->m_right->m_left != nullptr){
                group->m_right = right(group->m_right);
            }
        }
        //goes right if the id is greater then the right child id
        else if (group->m_right->m_groupID < id){
            group->m_right->m_right = splay(group->m_right->m_right, id);
            group = left(group); 
        }
    return (group->m_right == nullptr) ? group : left(group); 
    }
    return group; 
}


