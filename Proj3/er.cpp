// CMSC 341 - Fall 2026 - Project 3
#include "er.h"
Hospital::Hospital(){ // empty object
  m_ID = 0;
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = nullptr;
  m_heapType = NOTYPE;
  m_structure = NOSTRUCT;
  m_regPrior = 0;
}

Hospital::Hospital(prifn_t priFn, HEAPTYPE heapType,STRUCTURE structure, int regPrior, int id){
  m_ID = id;
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
  m_regPrior = regPrior; 
}

Hospital::~Hospital(){
  clear();
}
void Hospital::clear() {
  if(m_heap == nullptr){
      return;
  }
  clearHelper(m_heap);
  m_ID = 0;
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = nullptr;
  m_heapType = NOTYPE;
  m_structure = NOSTRUCT;
  m_regPrior = 0;

}

//helper function that recursivley traverses to delete every node/patient
void Hospital::clearHelper(Patient * patient){
  if(patient == nullptr){
    return;
  }
  //recursive call
  clearHelper(patient->m_left);
  clearHelper(patient->m_right);

  delete patient;
}

/* The copy constructor must make a deep copy of the rhs object.
It must function correctly if rhs is empty.
This function creates an exact same copy of rhs.*/
Hospital::Hospital(const Hospital& rhs){
  m_heap = copyHelper(rhs.m_heap);

  m_size = rhs.m_size;
  m_ID = rhs.m_ID;
  m_structure = rhs.m_structure;
  m_heapType = rhs.m_heapType;
  m_priorFunc = rhs.m_priorFunc;
  m_regPrior = rhs.m_regPrior;

}

/*The assignment operator creates an exact same copy of rhs.
You should not call the copy constructor in the
implementation of the assignment operator.*/
Hospital& Hospital::operator=(const Hospital& rhs) {
  if (this == &rhs){
    return * this;
  }

  if (this->m_heap != nullptr){
    clear();
  }

  m_heap = copyHelper(rhs.m_heap);

  m_size = rhs.m_size;
  m_ID = rhs.m_ID;
  m_structure = rhs.m_structure;
  m_heapType = rhs.m_heapType;
  m_priorFunc = rhs.m_priorFunc;
  m_regPrior = rhs.m_regPrior;

  return * this;
}

/*
This function merges the host queue with the rhs; it leaves rhs empty;
it transfers all nodes from rhs to the current heap.
Two heaps can only be merged if they have the same priority function
and they are of the same data structure. If the user attempts to merge queues
with different priority functions, or two different data structures a domain_error
exception should be thrown. This function requires protection against self-merging.
Merging a heap with itself is not allowed.
*/

void Hospital::mergeWithQueue(Hospital& rhs) {

  if(this == &rhs){
    return;
  }

  if (rhs.m_heap == nullptr){
    return;
  }

  if(m_heap == nullptr){

    if(m_priorFunc != nullptr && (m_priorFunc != rhs.m_priorFunc && m_heapType != rhs.m_heapType 
      && m_structure != rhs.m_structure)){
        throw domain_error("These heaps are not compatible");
      }

    if(m_priorFunc == nullptr){
      m_priorFunc = rhs.m_priorFunc;
      m_heapType = rhs.m_heapType;
      m_structure = rhs.m_structure;
 
    }

    m_heap = rhs.m_heap;
    m_size = rhs.m_size;
    rhs.m_heap = nullptr;
    rhs.m_size = 0;
    return;
  }

  if (m_priorFunc != rhs.m_priorFunc && m_heapType != rhs.m_heapType
     && m_structure != rhs.m_structure){
    throw domain_error("These heaps are not compatible");
  }

  m_heap = mergeHelper(m_heap, rhs.m_heap);
  m_size = rhs.m_size + m_size;
  rhs.m_heap = nullptr;
  rhs.m_size = 0;
}

Patient * Hospital::mergeHelper(Patient * thisRoot, Patient * rhsRoot){
  //base cases
  if (thisRoot == nullptr){
    return rhsRoot;
  }

  if (rhsRoot == nullptr){
    return thisRoot;
  }

  if (higherPrior(thisRoot, rhsRoot, m_heapType)){
    swapNodes(thisRoot, rhsRoot);
  }
  //recusrive call, goes down the right side of the tree
  thisRoot->m_right = mergeHelper(thisRoot->m_right, rhsRoot);

  if (m_structure == SKEW){
    //swaps the children to handel the skew property
    Patient * leftChild = thisRoot->m_left;
    thisRoot->m_left = thisRoot->m_right;
    thisRoot->m_right = leftChild;
    return thisRoot;
  }

  int rNPL = -1;
  int lNPL = -1;
  //gets the NPL for both children if they aren't nullptrs
  if (thisRoot->m_left != nullptr){
    lNPL = thisRoot->m_left->getNPL();
  }

  if(thisRoot->m_right != nullptr){
    rNPL = thisRoot->m_right->getNPL();
  }
  //for leftits property only need to switch if the rNPL is bigger then lNPL
  if (rNPL > lNPL){
    Patient * leftChild = thisRoot->m_left;
    thisRoot->m_left = thisRoot->m_right;
    thisRoot->m_right = leftChild;

    int tempNPL = lNPL;
    lNPL = rNPL;
    rNPL = tempNPL;
  }

  thisRoot->setNPL(rNPL + 1);
  return thisRoot;
}

/*
Inserts a Patient object into the queue.
It must maintain the min-heap or the max-heap property depending on the settings.
Moreover, if the selected data structure is leftist heap,
it needs to maintain a correct value of Null Path Length (NPL) in the node.
If the Patient object does not provide a valid priority value the object is not inserted
and the function returns false, otherwise the function returns true.
*/
bool Hospital::insertPatient(const Patient& patient) {

  if (patient.m_patient > MAXPATIENTID || patient.m_patient < MINPATIENTID){
    return false;
  }

  int patientPrior =  m_priorFunc(patient);

  if (patientPrior <= 0){
    return false;
  }

  Patient * newPatient = new Patient(patient);
  newPatient->m_left = nullptr;
  newPatient->m_right = nullptr;
  newPatient->m_npl = 0;
  m_heap = mergeHelper(m_heap, newPatient);
  m_size += m_size + 1;
  return true;
    
}

//It returns the current number of patients in the queue.
int Hospital::numPatients() const{
  return m_size;
}

//This function returns the current priority function
prifn_t Hospital::getPriorityFn() const {
  return m_priorFunc;
}

/*
This function extracts (removes the node) and returns the highest
priority patient from the queue.
It must maintain the min-heap or max-heap property.
The function throws an out_of_range exception
if the queue is empty when the function is called.
*/
Patient Hospital::getNextPatient() {

  if (m_heap == nullptr){
    throw out_of_range("Que is empty");
  }

  int nextPatientId = m_heap->m_patient;
  int nextPatientTemp = m_heap->m_temperature;
  int nextPatientOx = m_heap->m_oxygen;
  int nextPatientRr = m_heap->m_RR;
  int nextPatientBp = m_heap->m_BP;
  int nextPatientOp = m_heap->m_opinion;

  Patient * leftchild = m_heap->m_left;
  Patient * rightchild = m_heap->m_right;

  delete m_heap;
  m_heap=mergeHelper(leftchild, rightchild);
  m_size = m_size - 1;

  return Patient(nextPatientId, nextPatientTemp, nextPatientOx, nextPatientRr, nextPatientBp,
  nextPatientOp);
}

/*
This function sets a new priority function and its corresponding heap type (min-heap or max-heap).
It must rebuild the heap! To rebuild the heap this application does not re-allocate memory
to the objects; it reuses the current memory.
Note: it is the responsibility of the user to pass compatible arguments priFn and heapType.
The class implementation does not need to check the compatibility
*/
void Hospital::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {

  if (m_heap == nullptr){
    m_priorFunc = priFn;
    m_heapType = heapType;
    return;
  }
  
  Patient * tempRoot = m_heap;

  m_heap = nullptr;

  m_priorFunc = priFn;
  m_heapType = heapType;
  m_heap = heapify(tempRoot);
    
}

/*
This function sets the data structure, i.e. it is either SKEW or LEFTIST.
It must rebuild a heap with the new structure using the nodes in the current data structure.
Note: rebuild means transferring nodes from the current data structure to the new data structure.
Rebuild operation does not re-create the nodes.
*/
void Hospital::setStructure(STRUCTURE structure){
    
  if (m_heap == nullptr){
    m_structure = structure;
    return;
  }
  
  Patient * tempRoot = m_heap;

  m_heap = nullptr;

  m_structure = structure;
  m_heap = heapify(tempRoot);
}

HEAPTYPE Hospital::getHeapType() const {
  return m_heapType;
}
STRUCTURE Hospital::getStructure() const {
  return m_structure;
}

/*
It prints the contents of the queue using preorder traversal.
Although the first patient printed should have the highest priority,
the remaining patients will not necessarily be in priority patient.
Please refer to the sample output file (driver.txt) for the format of this function's output.
*/
void Hospital::printPatientQueue() const {

  cout << "Contents of the queue:" << endl;
  preOrderTraversal(m_heap);
  
}

void Hospital::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    cout << "Hospital " << m_regPrior << ": => ";
    dump(m_heap);
  }
  cout << endl;
}
void Hospital::dump(Patient *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(*pos) << ":" << pos->m_patient;
    else
        cout << m_priorFunc(*pos) << ":" << pos->m_patient << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout << "Patient ID: " << patient.getPatient() 
        << ", blood pressure: " << patient.getBP()
        << ", temperature: " << patient.getTemperature();
  return sout;
}

Patient * Hospital::heapify(Patient * root){

  if (root == nullptr ){
      return root;
  }

  Patient * left = root->m_left;
  Patient * right = root->m_right;

  root->m_left = nullptr;
  root->m_right = nullptr;
  root->m_npl = 0;


  left = heapify(left);
  right = heapify(right);

  return mergeHelper(root, mergeHelper(left, right));
}

void Hospital::preOrderTraversal(Patient * root) const{
  if (root == nullptr){
    return;
  }

  cout << "[ " << m_priorFunc(*root) << " ]" << " Patient ID: "
  << root->m_patient << " blood Pressure: " << root->m_BP << " temperature: "
  << root->m_temperature << endl;
  preOrderTraversal(root->m_left);
  preOrderTraversal(root->m_right);

}

bool Hospital::higherPrior(Patient * lhs, Patient * rhs, HEAPTYPE type){
  if (type == MINHEAP){
    if (m_priorFunc(*rhs) < m_priorFunc(*lhs)){
        return true;
    }
  }

  if (type == MAXHEAP){
    if ((m_priorFunc(*rhs)) > (m_priorFunc(*lhs))){
      return true;
    }
  }

  return false;
}

void Hospital::swapNodes(Patient * &lhs, Patient * &rhs){
  Patient * temp = lhs;
  lhs = rhs;
  rhs = temp;
}

Patient * Hospital::copyHelper( Patient * rhsRoot){

  if (rhsRoot == nullptr){
    return rhsRoot;
  }

  Patient * newPatient = new Patient(rhsRoot->m_patient, rhsRoot->m_temperature,
     rhsRoot->m_oxygen, rhsRoot->m_RR, rhsRoot->m_BP, rhsRoot->m_opinion);
  newPatient->m_left = copyHelper(rhsRoot->m_left);
  newPatient->m_right = copyHelper(rhsRoot->m_right);
  return newPatient;
}

//////////////////////////////////////////////////////////////

Region::Region(int size){
  m_capacity = size;
  m_size = 0;
  m_heap = new Hospital[size];
  
}

Region::~Region(){
  delete []m_heap;
  m_heap = nullptr;
  m_capacity = 0;
  m_size = 0;
}

/*
This function enqueues the Hospital object.
If enqueue operation is successful it returns true. 
The empty Hospital objects are allowed in this queue.
Every Hospital has a priority value stored in the member variable m_regPrior.
*/

bool Region::addHospital(Hospital & hospital){
  if (m_size >= m_capacity){
    return false;
  }

  m_size = m_size + 1;
  m_heap[m_size] = hospital;
  enqueHelper(m_size);
  return true;
}

/*
This function dequeues the Hospital with highest priority.
If dequeue operation is successful it returns true.
 The Hospital is returned in the parameter hospital.
*/

bool Region::getHospital(Hospital & hospital){

  if (m_size == 0){
    return false;
  }

  hospital = m_heap[ROOTINDEX];
  m_heap[ROOTINDEX] = m_heap[m_size];
  m_size = m_size - 1;

  if(m_size > 0){
    dequeueHelper(ROOTINDEX);
  }

  return true;

}

/*
This function dequeues the Nth highest priority Hospital object. 
After dequeue operation the rest of the queue must be preserved with the correct state.
If dequeue operation is successful it returns true.
*/

bool Region::getNthHospital(Hospital & hospital, int n){

  if (n < ROOTINDEX || n > m_size){
    return false;
  }

  Hospital* minVals = new Hospital[n]; // New array of length n (cause were looking for the last one)
  
  for (int i = 0; i < n; i++){
    getHospital(minVals[i]);
  }
  //dump();
  for (int i = 0; i < n-1; i++){
    addHospital(minVals[i]);
    //dump();
  }

  hospital = minVals[n-1];
  delete []minVals;
  return true;

  /*minVals[0] = m_heap[ROOTINDEX];
  for (int ii = 2; ii <= m_size; ii++) {
    bool breaker = false;
    for (int jj = 0; jj <= (n - 1); jj++) {
      if (!breaker && m_heap[ii].m_regPrior < minVals[jj].m_regPrior) {
        minVals[jj] = m_heap[ii];
        breaker = true;
      }
    }
  }*/
  // 
  // rotate accordingly

  
}

void Region::dump(){
    dump(ROOTINDEX);
    cout << endl;
}

void Region::dump(int index){
  if (index <= m_size){
    cout << "(";
    dump(index*2);
    cout << m_heap[index].m_regPrior;
    dump(index*2 + 1);
    cout << ")";
  }
}

/*
This function converts the type of heap in the Nth highest priority Hospital.
If the operation is successful it returns true.
*/

bool Region::setPriorityFn(prifn_t priFn, HEAPTYPE heapType, int n){

  if (n < ROOTINDEX || n > m_size){
    return false;
  }

  Hospital  temp; 

  if (!(getNthHospital(temp, n))){
    return false;
  }
  temp.dump();
  temp.setPriorityFn(priFn, heapType);
  addHospital(temp);
  return true;
}

/*
This function converts the structure of the heap in the Nth highest priority Hospital.
If the operation is successful it returns true.
*/

bool Region::setStructure(STRUCTURE structure, int n){

  if (n < ROOTINDEX || n > m_size){
    return false;
  }

  Hospital  temp; 

  if (!(getNthHospital(temp, n))){
    return false;
  }

  temp.setStructure(structure);
  addHospital(temp);
  return true;
}

/*
This function dequeues the highest priority Patient from the highest priority Hospital
and returns it in the parameter patient.
After dequeue operation the state of the rest of objects must be preserved.
If dequeue operation is successful it returns true.
If there are empty Hospital objects with higher priority that the one
having Patient objects the function removes and discards the empty objects.
*/

bool Region::getPatient(Patient & patient){
 return true;
}

void Region::enqueHelper(int index){

  while (index > ROOTINDEX){

    int parentIndex = (index / 2);

    if (m_heap[parentIndex].m_regPrior > m_heap[index].m_regPrior){
      Hospital  temp = m_heap[parentIndex];
      m_heap[parentIndex] = m_heap[index];
      m_heap[index] = temp;
      index = parentIndex;
    }

    else{
      return;
    }
  
  }
}

void Region::dequeueHelper(int index){
  while (index*2 <= m_size){

    int leftChild = index*2;
    int rightChild = leftChild + 1;
    int smallerChild = leftChild;

    if (rightChild <= m_size && m_heap[rightChild].m_regPrior < m_heap[leftChild].m_regPrior){
      smallerChild = rightChild;

    }

    if (m_heap[index].m_regPrior > m_heap[smallerChild].m_regPrior){
      Hospital  temp = m_heap[index];
      m_heap[index] = m_heap[smallerChild];
      m_heap[smallerChild] = temp;
      index = smallerChild;
    }
    else{return;}
  }
}