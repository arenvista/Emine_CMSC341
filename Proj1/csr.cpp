// CMSC 341 - Fall 2026 - Project 1
#include "csr.h"
///////////////CSR Class Implementation///////////////
CSR::CSR(){
    /*int* m_values;      //array to store non-zero values
    int* m_col_index;   //array to store column indices
    int* m_row_index;   //array to store row indices 
    int m_nonzeros;     //number of non-zero values
    int m_m;            //number of rows
    int m_n;            //number of columns
    CSR* m_next;        //pointer to the next CSR object in linked list
    */

    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;
    
}
CSR::~CSR(){
    clear();
    
}
CSR::CSR(const CSR & rhs){
    
}
void CSR::clear(){
    
    
}
bool CSR::empty() const{
    
}
void CSR::compress(int m, int n, int array[], int arraySize){
    
}
int CSR::getAt(int row, int  col) const{
    
} 
bool CSR::operator==(const CSR & rhs) const{
    
}
int CSR::sparseRatio(){
    
}
void CSR::dump(){
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

//////////////CSRList Class Implementation///////////////
CSRList::CSRList(){
    
}
CSRList::CSRList(const CSRList & rhs){
    
}
CSRList::~CSRList(){
    
}
bool CSRList::empty() const{
    
}
void CSRList::insertAtHead(const CSR & matrix){
    
}
void CSRList::clear(){
    
}

int CSRList::getAt(int CSRIndex, int row, int col) const{
    
}
bool CSRList::operator== (const CSRList & rhs) const{
    
}
const CSRList& CSRList::operator=(const CSRList & rhs){
    
}
int CSRList::averageSparseRatio(){
    
}
void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}