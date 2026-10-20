#include <iostream>
using namespace std;

void weirdCount(int ii) {
    if (ii == 10) {
        cout << ii << endl;
        return;
    } else {
        cout << "Running weridCount(" << ii + 1 << ")" << endl;
        weirdCount(ii + 1);
        cout << ii << endl;
    }
}

int main() {
    weirdCount(0);
    return 0;
}

/*
Drone * Group::removeDead(Drone * drone){
    /* if (m_root == nullptr){
        return nullptr; 
    }
    //Drone * drone = m_root; 
    if(drone->getState() == DEAD){
        remove(drone->getID());
    } else {
        // If 2 children
            // run func on left
            // run func on right
            // rebalance/return

        if (drone->getLeft() && drone->getRight()){ //2 children
            removeDead(drone->getLeft());
            removeDead(drone->getRight());
            return rebalanceTree(drone);
        }

        // else if 1 left child
            // run func on left
            // rebalance/return
        else if(drone->getLeft() && !drone->getRight()){
            removeDead(drone->getLeft());
            return rebalanceTree(drone);
        }

        // else if 1 right child
            // run func on right
            // rebalance/return
        else if(drone->getRight() && !drone->getLeft()){
            removeDead(drone->getRight());
            return rebalanceTree(drone);
        }

        // else (no children)
            // return
        else{
            return nullptr; 
        }
    }
    return nullptr; 
} */