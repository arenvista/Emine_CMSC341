#include "csr.h"

/*
== Testing CSR Class
- [ ] Test whether the compress(...) function works correctly for a normal case, e.g. it populates the member variables of the class with the expected values for an array of data.
- [ ] Test whether the compress(...) function works correctly for an error case, e.g. the user provides data which is less than the asking matrix size.
- [ ] Test whether the compress(...) function works correctly for an error case, e.g. the user asks for a 0 x 0 matrix but provides data.
- [ ] Test whether the overloaded equality operator works correctly for a normal case.
- [ ] Test whether the overloaded equality operator works correctly for an edge case, i.e. both objects are empty.
- [ ] Test whether the getAt(...) function throws an exception for an error case, i.e. the requested index numbers do not exist in the matrix.

== Testing CSRList Class
- [ ] Test whether the overloaded assignment operator works correctly for a normal case.
- [ ] Test whether the overloaded assignment operator works correctly for an edge case, e.g. the case of assigning an empty object to an object that contains data.
- [ ] Test whether the getAt(...) function throws an exception for an error case, i.e. the list of matrices is empty.
- [ ] Test whether the getAt(...) works correctly for a normal case, it returns the expected answer.

== Testing For Memory Leaks / Memory Errors
Run your test program in valgrind; check that there are no memory leaks or errors.
Note: If valgrind finds memory errors, compile your code with the -g option to enable debugging support and then re-run valgrind with the -s and --track-origins=yes options. valgrind will show you the line numbers where the errors are detected and can usually tell you which line is causing the error.
Never ignore warnings. They are a major source of errors in a program.
*/


class Tester{

    public:
    bool testDefaultConstructorOne();
    bool testDefaultConstructorTwo();
    //IF YOU HAVE TIME GO BACK AND DO THIS ONE
    bool testCopyConstructorOne();
    bool testCompresOne();
    bool testCompresTwo();
    bool testCompresThree();
    bool testOverloadedEqualityOne();
    bool testOverloadedEqualityTwo();
    bool testGetAtOne();
    bool testOverloadedAsignmentOne();
    bool testOverloadedAsignmentTwo();
    bool testGetAtTwo();
    bool testGetAtThree();
    private:

};

int main (){
    Tester tester;
    /*CSR test;
    int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    test.compress(4,6,array1,24);
    test.dump();

    //cout << endl << test.getAt(2,4) << endl;//returns the value 70
    cout << endl << test.sparseRatio() << endl;

    CSR cCSR;
    int array2[] = {0,0,0,0,100,200,0,0,300};
    cCSR.compress(3,3,array2,9);//initialize object cCSR
    cCSR.dump();
    cout << endl << cCSR.sparseRatio() << endl; 
    */

    cout << "Test if default constructor works one" << endl;

    tester.testDefaultConstructorOne() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    //cout << "Test if default constructor works two" << endl;

    //tester.testDefaultConstructorTwo() ? cout << "Passed " << endl
    //                                : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if Compress() works for a normal case" << endl;

    tester.testCompresOne() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if Compress() works for a error case" << endl;

    tester.testCompresTwo() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if Compress() works for another error case" << endl;

    tester.testCompresThree() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if Overloaded Equality Operator works for a normal case" << endl;

    tester.testOverloadedEqualityOne() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if Overloaded Equality Operator works for a error case" << endl;

    tester.testOverloadedEqualityTwo() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if GetAt() works for a error case" << endl;

    tester.testGetAtOne() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if Overloaded Asignment works for normal case" << endl;

    tester.testOverloadedAsignmentOne() ? cout << "Passed" << endl
                                                : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if Overloaded Asignment works for edge case" << endl;

    tester.testOverloadedAsignmentTwo() ? cout << "Passed" << endl
                                               : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if getAtTwo() works for error case" << endl;

    tester.testGetAtTwo() ? cout << "Passed" << endl
                                               : cout << "Fail" << endl;

    cout << endl;

    return 0;
}

/*
    m_values = nullptr; //array to store non-zero values
    m_col_index = nullptr; //array to store column indices
    m_row_index = nullptr; //array to store row indices 
    m_nonzeros = 0; //number of non-zero values
    m_m = 0; //number of rows
    m_n = 0; //number of columns
    m_next = nullptr; //pointer to the next CSR object in linked list
*/
//Tests default constructor for CSR object
bool Tester::testDefaultConstructorOne(){
        CSR object;
    bool  result = true;
    result       = result && (object.m_nonzeros == 0);
    result       = result && (object.m_m == 0);
    result       = result && (object.m_n == 0);
    result       = result && (object.m_values == nullptr);
    result       = result && (object.m_col_index == nullptr);
    result       = result && (object.m_row_index == nullptr);
    result       = result && (object.m_next == nullptr);
    return result;
}
//Test default constructor for CSRList object
bool Tester::testDefaultConstructorTwo(){
    CSRList object;
    bool  result = true;
    result       = result && (object.m_size == 0);
    result       = result && (object.m_head == nullptr);
    return result;
}

bool Tester::testCopyConstructorOne(){
    return true;
}

bool Tester::testCompresOne(){
    CSR object;
    bool result = true;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    object.compress(4,6,array,24);
    int nonZeroes = 8;
    int rows = 4;
    int cols = 6;
    int values[] = {10,20,30,40,50,60,70,80};
    int col_index[] = {0,1,1,3,2,3,4,5};
    int row_index[] ={0,2,4,7,8};

    result = result && (object.m_nonzeros == nonZeroes);
    result = result && (object.m_m == rows);
    result = result && (object.m_n == cols);
    
    for (int i = 0; i < nonZeroes; i ++){
        if (object.m_values[i] != values[i]){
            result = false; 
        }
    }

    for (int i = 0; i < nonZeroes; i ++){
        if (object.m_col_index[i] != col_index[i]){
            result = false; 
        }
    }

    for (int i = 0; i < rows + 1; i ++){
        if (object.m_row_index[i] != row_index[i]){
            result = false; 
        }
    }

    return result;
}

bool Tester::testCompresTwo(){

    CSR object;
    bool result = true;
    int array[] = {10, 0, 5, 0, 0, 7, 0, 0, 9, 0};
    object.compress(4,6,array,10);
    int nonZeroes = 4;
    int rows = 4;
    int cols = 6;
    int values[] = {10,5,7,9};
    int col_index[] = {0,2,5,2};
    int row_index[] ={0,3,4,4,4};

    result = result && (object.m_nonzeros == nonZeroes);
    result = result && (object.m_m == rows);
    result = result && (object.m_n == cols);
    
    for (int i = 0; i < nonZeroes; i ++){
        if (object.m_values[i] != values[i]){
            result = false; 
        }
    }

    for (int i = 0; i < nonZeroes; i ++){
        if (object.m_col_index[i] != col_index[i]){
            result = false; 
        }
    }

    for (int i = 0; i < rows + 1; i ++){
        if (object.m_row_index[i] != row_index[i]){
            result = false; 
        }
    }

    return result;

}

bool Tester::testCompresThree(){

    CSR object;
    bool result = true;
    int array[] = {10, 0, 5, 0, 0, 7, 0, 0, 9, 0};
    object.compress(0,0,array,10);
    int nonZeroes = 0;
    int rows = 0;
    int cols = 0;

    result = result && (object.m_nonzeros == nonZeroes);
    result = result && (object.m_m == rows);
    result = result && (object.m_n == cols);
    result = result && (object.m_values == nullptr);
    result = result && (object.m_col_index == nullptr);
    result = result && (object.m_row_index == nullptr);
    
    return result;

}

bool Tester::testOverloadedEqualityOne(){
    CSR object1;
    CSR object2;
    int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    object1.compress(4,6,array1,24);
    object2.compress(4,6,array1,24);

    bool result = true;

    result = result && (object1.m_nonzeros == object2.m_nonzeros);
    result = result && (object1.m_m == object2.m_m);
    result = result && (object1.m_n == object2.m_n);

    for(int i = 0; i < object1.m_nonzeros; i++){
        if (object1.m_values[i] != object2.m_values[i]){
            result = false;
        }
    }

    for(int i = 0; i < object1.m_nonzeros; i++){
        if (object1.m_col_index[i] != object2.m_col_index[i]){
            result = false;
        }
    }

    for(int i = 0; i < object1.m_m; i++){
        if (object1.m_row_index[i] != object2.m_row_index[i]){
            result = false;
        }
    }

    result = result && (object1 == object2);

    return result;
}

bool Tester::testOverloadedEqualityTwo(){

    CSR object1;
    CSR object2;

    bool result = true;

    result = result && (object1.m_nonzeros == object2.m_nonzeros);
    result = result && (object1.m_m == object2.m_m);
    result = result && (object1.m_n == object2.m_n);

    for(int i = 0; i < object1.m_nonzeros; i++){
        if (object1.m_values[i] != object2.m_values[i]){
            result = false;
        }
    }

    for(int i = 0; i < object1.m_nonzeros; i++){
        if (object1.m_col_index[i] != object2.m_col_index[i]){
            result = false;
        }
    }

    for(int i = 0; i < object1.m_m; i++){
        if (object1.m_row_index[i] != object2.m_row_index[i]){
            result = false;
        }
    }

    result = result && (object1 == object2);

    return result;
    
}

bool Tester::testGetAtOne(){
    CSR test;
    bool result = true;
    int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    test.compress(4,6,array1,24);

    try {
        test.getAt(3,6);
        result = false;
    }
    catch (exception &e){};

    try {
        test.getAt(4,2);
        result = false;
    }
    catch (exception &e){};

    return result;

}

bool Tester::testOverloadedAsignmentOne(){
    bool result = true;

    CSRList list1;
    int array1[] = {2,0,6,0};
    CSR object1;
    object1.compress(2,2,array1,4);
    list1.insertAtHead(object1);
    int array2[] = {4,2,0,0};
    CSR object2;
    object2.compress(2,2,array2,4);
    list1.insertAtHead(object2);

    CSRList list2;
    int array3[] = {3,0,5,0};
    CSR object3;
    object3.compress(2,2,array3,4);
    list2.insertAtHead(object3);

    list2 = list1;

    result = result && (list2.m_size == list1.m_size);

    result = result && (list2.m_head != list1.m_head);
    
    CSR * csr1 = list1.m_head;
    CSR * csr2 = list2.m_head;

    while (csr1 != nullptr && csr2 != nullptr){
        result = result && (csr1->m_nonzeros == csr2->m_nonzeros);
        result = result && (csr1->m_m == csr2->m_m);
        result = result && (csr1->m_n == csr2->m_n);

        for (int i = 0; i < csr1->m_nonzeros; i ++){
            result = result && (csr1->m_values[i] == csr2->m_values[i]);
            result = result && (csr1->m_col_index[i] == csr2->m_col_index[i]);
        }

        for (int i = 0; i < csr1->m_m; i ++){
            result = result && (csr1->m_row_index[i] == csr2->m_row_index[i]);
        }

        csr1 = csr1->m_next;
        csr2 = csr2->m_next;
    }

    result = result && (csr1 == nullptr && csr2 == nullptr);

    return result; 
}

bool Tester::testOverloadedAsignmentTwo(){
    bool result = true;

    CSRList list1;
    int array1[] = {2,0,6,0};
    CSR object1;
    object1.compress(2,2,array1,4);
    list1.insertAtHead(object1);
    int array2[] = {4,2,0,0};
    CSR object2;
    object2.compress(2,2,array2,4);
    list1.insertAtHead(object2);
    int array3[] = {7,9,3,0};
    CSR object3;
    object3.compress(2,2,array3,4);
    list1.insertAtHead(object3);


    CSRList list2;
  

    list2 = list1;

    result = result && (list2.m_size == list1.m_size);

    result = result && (list2.m_head != list1.m_head);
    
    CSR * csr1 = list1.m_head;
    CSR * csr2 = list2.m_head;

    while (csr1 != nullptr && csr2 != nullptr){
        result = result && (csr1->m_nonzeros == csr2->m_nonzeros);
        result = result && (csr1->m_m == csr2->m_m);
        result = result && (csr1->m_n == csr2->m_n);

        for (int i = 0; i < csr1->m_nonzeros; i ++){
            result = result && (csr1->m_values[i] == csr2->m_values[i]);
            result = result && (csr1->m_col_index[i] == csr2->m_col_index[i]);
        }

        for (int i = 0; i < csr1->m_m; i ++){
            result = result && (csr1->m_row_index[i] == csr2->m_row_index[i]);
        }

        csr1 = csr1->m_next;
        csr2 = csr2->m_next;
    }

    result = result && (csr1 == nullptr && csr2 == nullptr);

    return result; 
    
}

bool Tester::testGetAtTwo(){
    bool result = true;
    //create empty list

    CSRList list1;

    try{

        list1.getAt(0,1,1);
        result = false;
    }
    catch (exception &e){};

    
    return result; 
}

bool Tester::testGetAtThree(){
    bool result = true;

    CSRList list1;
    int array1[] = {2,0,6,0};
    CSR object1;
    object1.compress(2,2,array1,4);
    list1.insertAtHead(object1);
    int array2[] = {4,2,0,0};
    CSR object2;
    object2.compress(2,2,array2,4);
    list1.insertAtHead(object2);
    int array3[] = {7,9,3,0};
    CSR object3;
    object3.compress(2,2,array3,4);
    list1.insertAtHead(object3);

    return result; 
    
}



