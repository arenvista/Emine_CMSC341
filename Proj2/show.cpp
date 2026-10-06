// UMBC - CMSC 341 - Fall 2026 - Proj2
#include "show.h"

Group::Group(){ 
    m_root = nullptr;
    m_groupID = DEFAULT_ID;
    m_left = nullptr;
    m_right = nullptr;
}
Group::Group(int group, Drone drones[], int population){
    m_groupID = group;
    m_root = nullptr;
    m_left = nullptr;
    m_right = nullptr;
    for (int i = 0; i<population; i++){
        insert(drones[i]);
    }
}
Group::~Group(){
    clear();
}
void Group::insert(const Drone& drone){
    if (drone.getID() >= MINID && drone.getID() <= MAXID){

        m_root = insert(m_root, drone);
    
    }  
    else{
        cout << "ID is not valid " << endl;
    }
}

void Group::clear(){
    if(m_root == nullptr){return;}
    clear(m_root);
    m_root = nullptr;
    m_left = nullptr;
    m_right = nullptr;
    m_groupID = 0; 
}

void Group::clear(Drone* drone){
    if(drone == nullptr){return;}
    if(drone->m_left != nullptr){clear(drone->m_left);}
    if(drone->m_right != nullptr){clear(drone->m_right);}
    delete drone;
}

Drone* Group::findDrone(Drone* n, int id) const{
    if (n->getID() == id ){
        return n;
    }
    if (n->getID() < id){
        return findDrone(n->m_left, id);
    }
    if(n->getID() > id){
        return findDrone(n->m_right, id);
    }
    cout << "DID NOT FIND DRONE \n";
    return nullptr;
}

void Group::remove(int id){
    Drone * drone = findDrone(m_root, id);

}

void Group::listDrones() const {
    
}

bool Group::setState(int id, STATE state){
    Drone* drone = findDrone(m_root, id);
    if (drone == nullptr){
        return false;
    }
    drone->m_state = state;
    return true;

}

void Group::removeDead(){
    
}

bool Group::findDrone(int id) const {
    Drone* drone = findDrone(m_root, id);
    if (drone == nullptr){
        return false;
    }
    return true;
}

int Group::count(LIGHTCOLOR color) const{
    
}

int Group::count(STATE state) const{
    
}

void Group::dumpTree() const {dump(m_root);}
void Group::dump(Drone* aDrone) const{
    if (aDrone != nullptr){
        cout << "(";
        dump(aDrone->m_left);//first visit the left child
        cout << aDrone->m_id << ":" << aDrone->m_height;//second visit the node itself
        dump(aDrone->m_right);//third visit the right child
        cout << ")";
    }
}

Drone* Group::insert(Drone* drone, const Drone& newDrone){
        //im lost not sure what to do here
    if (drone == nullptr){
        return new Drone(newDrone.getID(), newDrone.getType(), newDrone.getState());
    }
    if (newDrone.getID() < drone->getID()){
        drone->setLeft(insert(drone->getLeft(), newDrone));
    }
    if (newDrone.getID() > drone->getID()){
        drone->setRight(insert(drone->getRight(),newDrone));
    }
    else{
        cout << "No Duplicate ID's allowed!" << endl;
        return drone;
    }
    updateHeight(drone);
    return rebalanceDrone(drone);
}


void Group::updateHeight(Drone* drone){
    
}


int Group::balanceFactor(Drone* drone){
    int left;
    int right;
    if(drone->getLeft() == nullptr){
        left = 0;
    }
    else {left = (drone->getLeft()->getHeight());}
    if(drone->getRight() == nullptr){
        right= 0;
    }
    else {right = (drone->getRight()->getHeight());}
    int factor = (left - right);

    return factor;
}

Drone* Group::rebalanceDrone(Drone* drone){
    int factor = balanceFactor(drone);

    if (factor > 1){
        
    }

    if (factor < -1){

    }

    return drone;
}

Drone* Group::right(Drone* drone){
    
}

Drone* Group::left(Drone* drone){
    
}

Show::Show(){

}

Show::~Show(){
    
}

bool Show::insert(int group, Drone drones[], int population){

}

void Show::dump(bool verbose) const{
	dumpHelper(m_root, verbose);
    cout << endl;
}

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
int Show::count(int group, STATE state){
    
}

bool Show::removeDrone(int group, int drone, bool all){
    
}
int Show::getGroupHeight(int group){
    
}

bool Show::setState(int group, int drone, STATE state){
    
}


