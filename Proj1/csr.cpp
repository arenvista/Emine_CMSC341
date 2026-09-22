// CMSC 341 - Fall 2026 - Project 1
#include "csr.h"
///////////////CSR Class Implementation///////////////
CSR::CSR(){
    m_values = nullptr; //array to store non-zero values
    m_col_index = nullptr; //array to store column indices
    m_row_index = nullptr; //array to store row indices 
    m_nonzeros = 0; //number of non-zero values
    m_m = 0; //number of rows
    m_n = 0; //number of columns
    m_next = nullptr; //pointer to the next CSR object in linked list
    
}
CSR::~CSR(){
    clear();
    
}
CSR::CSR(const CSR & rhs){

    m_nonzeros = rhs.m_nonzeros;
    m_m = rhs.m_m;
    m_n = rhs.m_n;

    if (rhs.m_values == nullptr){
        m_values = nullptr;
        m_col_index = nullptr;
        m_row_index = nullptr;
        return;
    }

    m_values = new int[m_nonzeros];
    m_col_index = new int[m_nonzeros];
    m_row_index = new int[m_m + 1];

    for (int i = 0; i < m_nonzeros; i++){
        m_values[i] = rhs.m_values[i];
        m_col_index[i] = rhs.m_col_index[i];
    }

    for (int i = 0; i <= m_m; i++){
        m_row_index[i] = rhs.m_row_index[i];
    }
    
    //NOT SURE ABOUT THIS!!!
    m_next = nullptr;
    
    return;
}
void CSR::clear(){
    delete[] m_values;
    delete[] m_col_index;
    delete[] m_row_index;

    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;

    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;

}
bool CSR::empty() const{
    if (m_col_index == nullptr && m_row_index == nullptr){
        return true;
    }

    return false;
}
void CSR::compress(int m, int n, int array[], int arraySize){
    /*Since the function can be re-called on the same object 
    the function needs to re-create new memory for the new sparse matrix
    that is passed to the function*/

    //If a 0X0 matrix then create an empty object
    if (m == 0 || n == 0){
        return;
    }

    int total = (m*n);
    int min = (arraySize < total) ? arraySize : total;

    for (int i = 0; i < min; i++){
        if (array[i] != 0){
            m_nonzeros += 1;
        }
    }

    m_values = new int[m_nonzeros];
    m_col_index = new int[m_nonzeros];
    m_row_index = new int[m + 1];
    m_m = m;
    m_n = n;

    int nonZeroCount = 0;
    int currRow = 0;
    m_row_index[0] = 0;
    currRow ++ ;


    for (int i = 0; i < min; i++){
        int columnIndex = i % n;

        if (array[i] != 0){
            m_values[nonZeroCount] = array[i];
            m_col_index[nonZeroCount] = columnIndex;
            nonZeroCount ++;
        }

        if (columnIndex == n - 1 ){
                m_row_index[currRow] = nonZeroCount;
                currRow ++;
        }
    }

    while (currRow < m_m + 1){
        m_row_index[currRow] = nonZeroCount;
        currRow ++;
    }

}
int CSR::getAt(int row, int  col) const{
    if(m_values == nullptr){
        throw::runtime_error("Matrix not iniatlized correctly");
    }

    if( row >= m_m || row < 0){
        throw::runtime_error("Row index is not in range");
    }

    if( col >= m_n || col < 0){
        throw::runtime_error("Col index is not in range");
    }

    int row_start = m_row_index[row];
    int row_end   = m_row_index[row + 1];

    for (int i = row_start; i < row_end; i++){
        if(m_col_index[i] == col){
            return m_values[i];
        }
    }
    
    return 0;
    
} 
bool CSR::operator==(const CSR & rhs) const{
    if (m_n != rhs.m_n && m_m != rhs.m_m && m_nonzeros != rhs.m_nonzeros){
        return false; 
    }

    for (int i = 0; i < m_nonzeros; i++){
        if (m_col_index[i] != rhs.m_col_index[i]){
            return false;
        }

        if (m_values[i] != rhs.m_values[i]){
            return false;
        }
    }

    for (int i = 0; i < m_m; i++){
        if (m_row_index[i] != rhs.m_row_index[i]){
            return false; 
        }
    }
    return true;
}
int CSR::sparseRatio(){
    int numTotal = (m_m * m_n);
    //cout << numTotal << endl;
    int numZeros = (numTotal - m_nonzeros);
    //cout << numZeros << endl;
    double sparseRatio = ((double)numZeros/numTotal);
    //cout << sparseRatio << endl;
    int finalRatio = (int)(sparseRatio * 100);
    //cout << finalRatio << endl;
    return finalRatio;
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
    m_head = nullptr;
    m_size = 0;
}
CSRList::CSRList(const CSRList & rhs){
    if (rhs.m_head == nullptr){
        m_head = nullptr;
        return;
    }
    m_head = new CSR(*rhs.m_head);
    CSR* curNew = m_head;
    CSR* curRhs = rhs.m_head->m_next;

    while (curRhs != nullptr){
        curNew->m_next = new CSR(*curRhs);
        curNew = curNew->m_next;
        curRhs = curRhs->m_next;
    }


}
CSRList::~CSRList(){
     clear();
}
bool CSRList::empty() const{
    if (m_head == nullptr){
        return true;
    }
    return false; 
}
void CSRList::insertAtHead(const CSR & matrix){
    CSR* newCSR = new CSR(matrix);
    if (m_head == nullptr){
        m_head = newCSR;
        return; 
    }

    newCSR->m_next = m_head;
    m_head = newCSR;
    m_size ++;
}
void CSRList::clear(){
    CSR* temp = m_head;
    CSR* next;
    while( temp->m_next != nullptr){
        next = temp->m_next;
        delete temp;
        temp = next;
    }
    delete temp;
    m_head = nullptr;
    m_size = 0;
}

int CSRList::getAt(int CSRIndex, int row, int col) const{
    CSR* current = m_head;
    int numResult;
    // IS LINKED LIST 0 INDEXED?!?!?!? IF SO FIXXXXXX
    if(CSRIndex > m_size){
        //cout << CSRIndex << endl;
        //cout << m_size << endl;
        throw::runtime_error("Object is not in the list!");
    }

    for (int i = 0; i < CSRIndex; i++){
        current = current->m_next;
    }
    numResult = current->getAt(row, col);
    return numResult;
}

bool CSRList::operator== (const CSRList & rhs) const{
    //checks if they are the same size 
    if (m_size != rhs.m_size){
        return false;
    }

    CSR* curRhs = rhs.m_head;
    CSR* curNew = m_head;

    while ( curNew != nullptr && curRhs != nullptr){
        if (!(*curNew == *curRhs)){
            return false;
        }
        curNew = curNew->m_next;
        curRhs = curRhs->m_next;
    }

    return true;
}
const CSRList& CSRList::operator=(const CSRList & rhs){
    if (this == &rhs){
        return *this;
    }

    clear();

    if (rhs.m_head == nullptr){
        m_head = nullptr;
        return *this;
    }

    m_head = new CSR(*rhs.m_head);
    CSR* curRhs = rhs.m_head->m_next;
    CSR* curNew = m_head;

    while (curRhs != nullptr){
        curNew->m_next = new CSR(*curRhs);
        curRhs = curRhs->m_next;
        curNew = curNew->m_next;
    }

    return *this;
}
int CSRList::averageSparseRatio(){
    CSR* curCsr = m_head;

    if (curCsr == nullptr){
        return 0;
    }

    int totalRatio = 0;
    int count = 0;

    while( curCsr != nullptr){
        totalRatio += curCsr->sparseRatio();
        count ++;
        curCsr = curCsr->m_next;
    }

    double avgRatio = ((double)totalRatio / count);
    int finalRatio = (int)(avgRatio * 100);
    return finalRatio; 

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