// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"

class Tester {
public:
    bool testDefaultConstructor();

    bool testOptionalConstructorOne();

    bool testOptionalConstructorTwo();

    bool testOptionalConstructorThree();

    bool testrecreateCardOne();

    bool testrecreateCardTwo();

    bool testrecreateCardThree();

    bool testInitCardOne();

    bool testInitCardTwo();

    bool testInitCardThree();

    bool testPlayOne();

    bool testPlayTwo();

    bool testPlayThree();

    bool testPlayFour();

    bool testAssignmentNormal();

    bool testAssignmentEdge();

    bool testSelfAssignment();

private:
    /**********************************************
     * If we need helper functions to be reused in *
     *   test functions they can be declared here!
     **********************************************/
};

int main() {
    Tester tester;

    cout << "Test if default constructor works" << endl;
    tester.testDefaultConstructor() ? cout << "Passed " << endl
                                    : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if optional constructor works for normal case" << endl;
    tester.testOptionalConstructorOne() ? cout << "Passed" << endl
                                        : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if optional constructor works for edge case" << endl;
    tester.testOptionalConstructorTwo() ? cout << "Passed" << endl
                                        : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if recreateCard works for normal case" << endl;
    tester.testrecreateCardOne() ? cout << "Passed" << endl
                                 : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if recreateCard works for edge case" << endl;
    tester.testrecreateCardTwo() ? cout << "Passed" << endl
                                 : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if recreateCard works for error case" << endl;
    tester.testrecreateCardThree() ? cout << "Passed" << endl
                                   : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if testInitCardOne works for normal case" << endl;
    tester.testInitCardOne() ? cout << "Passed" << endl
                             : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if testInitCardTwo works for edge case" << endl;
    tester.testInitCardTwo() ? cout << "Passed" << endl
                             : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if testInitCardThree works for error case" << endl;
    tester.testInitCardThree() ? cout << "Passed" << endl
                               : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if testPlayOne works for normal case" << endl;
    tester.testPlayOne() ? cout << "Passed" << endl : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if testPlayTwo works for edge case" << endl;
    tester.testPlayTwo() ? cout << "Passed" << endl : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if testPlayThree works for eror case" << endl;
    tester.testPlayThree() ? cout << "Passed" << endl : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if testPlayFour works for eror case" << endl;
    tester.testPlayFour() ? cout << "Passed" << endl : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if assignmentNormal works for normal case" << endl;
    tester.testAssignmentNormal() ? cout << "Passed" << endl
                                  : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if assignmentEdge works for normal case" << endl;
    tester.testAssignmentEdge() ? cout << "Passed" << endl
                                : cout << "Fail" << endl;

    cout << endl;

    cout << "Test if testSelfAssignment works" << endl;
    tester.testSelfAssignment() ? cout << "Passed" << endl
                                : cout << "Fail" << endl;

    return 0;
}

// test default constructor and checks that member variables were set to default
bool Tester::testDefaultConstructor() {
    Bingo object;
    bool  result = true;
    result       = result && (object.m_numRows == 0);
    result       = result && (object.m_numCols == 0);
    result       = result && (object.m_minBallVal == 0);
    result       = result && (object.m_maxBallVal == 0);
    result       = result && (object.m_helperSize == 0);
    result       = result && (object.m_trackCols == nullptr);
    result       = result && (object.m_trackRows == nullptr);
    result       = result && (object.m_card == nullptr);
    result       = result && (object.m_helper == nullptr);

    return result;
}

// this tests the optional constructor with a normal case
// uses normal paremeters ad checks that variables were set to those values
// Also checks that memory was allocated for necesary variables
bool Tester::testOptionalConstructorOne() {
    int   rows = 10;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result = true;
    result       = result && (object.m_numRows == rows);
    // cout << "rows" << object.m_numRows << endl;
    result = result && (object.m_numCols == cols);
    result = result && (object.m_minBallVal == min);
    result = result && (object.m_maxBallVal == max);
    result = result && (object.m_helperSize == (max + 1));
    result = result && (object.m_trackCols != nullptr);
    result = result && (object.m_trackRows != nullptr);
    result = result && (object.m_card != nullptr);
    // cout << "mem allocated for m_card" << endl;
    result = result && (object.m_helper != nullptr);

    // object.initCard();
    // object.dumpCard();

    return result;
}

// this tests the optional constructor with an edge case
// uses edge paremeters ad checks that variables were set to those values
// Also checks that memory was allocated for necesary variables
bool Tester::testOptionalConstructorTwo() {
    // egde
    int   rows = 2;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result = true;
    result       = result && (object.m_numRows == rows);
    // cout << "rows" << object.m_numRows << endl;
    result = result && (object.m_numCols == cols);
    result = result && (object.m_minBallVal == min);
    result = result && (object.m_maxBallVal == max);
    result = result && (object.m_helperSize == (max + 1));
    result = result && (object.m_trackCols != nullptr);
    result = result && (object.m_trackRows != nullptr);
    result = result && (object.m_card != nullptr);
    // cout << "mem allocated for m_card" << endl;
    result = result && (object.m_helper != nullptr);

    // object.initCard();
    // object.dumpCard();

    return result;
}

// this a construcor test with an error case
// since parameters are invalid this test checks that object stays empty
bool Tester::testOptionalConstructorThree() {
    // egde?
    int   rows = -5;
    int   cols = -10;
    int   max  = 85;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result = true;
    result       = result && (object.m_numRows == 0);
    // cout << "rows" << object.m_numRows << endl;
    result = result && (object.m_numCols == 0);
    result = result && (object.m_minBallVal == 0);
    result = result && (object.m_maxBallVal == 0);
    result = result && (object.m_helperSize == 0);
    result = result && (object.m_trackCols == nullptr);
    result = result && (object.m_trackRows == nullptr);
    result = result && (object.m_card == nullptr);
    // cout << "mem allocated for m_card" << endl;
    result = result && (object.m_helper == nullptr);

    // object.initCard();
    // object.dumpCard();

    return result;
}

// this test recreateCard functionality for a normal case
// parameters passed in are valid
// this tests to make sure card is recreated with new values
// card is orignally constructed with the global values
bool Tester::testrecreateCardOne() {
    // egde?
    int   rows = 10;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object(CARDROWS, CARDCOLS, MINVAL, MAXVAL);
    object.clear();
    object.reCreateCard(rows, cols, min, max);
    bool result = true;
    result      = result && (object.m_numRows == rows);
    // cout << "rows" << object.m_numRows << endl;
    result = result && (object.m_numCols == cols);
    result = result && (object.m_minBallVal == min);
    result = result && (object.m_maxBallVal == max);
    result = result && (object.m_helperSize == max + 1);
    result = result && (object.m_trackCols != nullptr);
    result = result && (object.m_trackRows != nullptr);
    result = result && (object.m_card != nullptr);
    // cout << "mem allocated for m_card" << endl;
    result = result && (object.m_helper != nullptr);

    // object.initCard();
    // object.dumpCard();

    return result;
}

// this test recreateCard functionality for a edge case
// parameters passed in are valid
// this tests to make sure card is recreated with new values
// card is orignally constructed with the global values
bool Tester::testrecreateCardTwo() {
    // egde?
    int   rows = 2;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object(CARDROWS, CARDCOLS, MINVAL, MAXVAL);
    object.clear();
    object.reCreateCard(rows, cols, min, max);
    bool result = true;
    result      = result && (object.m_numRows == rows);
    // cout << "rows" << object.m_numRows << endl;
    result = result && (object.m_numCols == cols);
    result = result && (object.m_minBallVal == min);
    result = result && (object.m_maxBallVal == max);
    result = result && (object.m_helperSize == max + 1);
    result = result && (object.m_trackCols != nullptr);
    result = result && (object.m_trackRows != nullptr);
    result = result && (object.m_card != nullptr);
    // cout << "mem allocated for m_card" << endl;
    result = result && (object.m_helper != nullptr);

    // object.initCard();
    // object.dumpCard();

    return result;
}

// this test recreateCard functionality for a normal case
// parameters passed in are not valid
// this tests to make sure card is not touched
// but since this function is called with clear
// object should now be empty since clear was called and card can't recreated
bool Tester::testrecreateCardThree() {
    int   rows = -5;
    int   cols = -10;
    int   max  = 85;
    int   min  = 11;
    Bingo object(CARDROWS, CARDCOLS, MINVAL, MAXVAL);
    object.clear();
    object.reCreateCard(rows, cols, min, max);
    // cout << object.m_numRows << endl;
    bool result = true;
    result      = result && (object.m_numRows == 0);
    // cout << "rows" << object.m_numRows << endl;
    result = result && (object.m_numCols == 0);
    result = result && (object.m_minBallVal == 0);
    result = result && (object.m_maxBallVal == 0);
    result = result && (object.m_helperSize == 0);
    result = result && (object.m_trackCols == nullptr);
    result = result && (object.m_trackRows == nullptr);
    result = result && (object.m_card == nullptr);
    // cout << "mem allocated for m_card" << endl;
    result = result && (object.m_helper == nullptr);

    return result;
}

// tests that InitCard works with normal case
// in this test I check the bool results of init function for normal case
// I also check that each value is in the correct range based on it's column
// this check ensures that m_card was created correctly
bool Tester::testInitCardOne() {
    int   rows = 5;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result;
    if (object.initCard() != false) {
        result = true;
    } else {
        result = false;
        return result;
    }
    for (int r = 0; r < object.m_numRows; r++) {
        for (int c = 0; c < object.m_numCols; c++) {
            int columnMin =
                object.m_minBallVal +
                ((object.m_maxBallVal - object.m_minBallVal) * (0.2 * c));
            int columnMax = object.m_minBallVal +
                            ((object.m_maxBallVal - object.m_minBallVal) *
                             (0.2 + (0.2 * c)));
            result = result && ((object.m_card[r][c].m_value >= columnMin) &&
                                (object.m_card[r][c].m_value <= columnMax));
        }
    }

    return result;
}

// tests that InitCard works with edge case
// in this test I check the bool results of init function for edge case
// I also check that each value is in the correct range based on it's column
// this check ensures that m_card was created correctly
bool Tester::testInitCardTwo() {
    int   rows = 2;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result;
    if (object.initCard() != false) {
        result = true;
    } else {
        result = false;
    }

    for (int r = 0; r < object.m_numRows; r++) {
        for (int c = 0; c < object.m_numCols; c++) {
            int columnMin =
                object.m_minBallVal +
                ((object.m_maxBallVal - object.m_minBallVal) * (0.2 * c));
            int columnMax = object.m_minBallVal +
                            ((object.m_maxBallVal - object.m_minBallVal) *
                             (0.2 + (0.2 * c)));
            result = result && ((object.m_card[r][c].m_value >= columnMin) &&
                                (object.m_card[r][c].m_value <= columnMax));
        }
    }
    return result;
}

// tests that InitCard works with error case
// in this test I check the bool results of init function for error case
// I also check that the card is empty because it can't be filled if its not
// valid
bool Tester::testInitCardThree() {
    int   rows = -5;
    int   cols = -10;
    int   max  = 90;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result;
    if (object.initCard() == false) {
        result = true;
    } else {
        result = false;
    }

    result = result && (object.m_numRows == 0);
    result = result && (object.m_numCols == 0);
    result = result && (object.m_minBallVal == 0);
    result = result && (object.m_maxBallVal == 0);
    result = result && (object.m_helperSize == 0);
    result = result && (object.m_trackCols == nullptr);
    result = result && (object.m_trackRows == nullptr);
    result = result && (object.m_card == nullptr);
    result = result && (object.m_helper == nullptr);

    return result;
}

// This tester check play with an normal case
// I manuall fill the card wit the first column containing numbers
// 11,12,13,14,15 in a row I then manually fill the vector for rndBalls with
// 11,12,13,14,15 in a row in the front The play fucntion should return 5 since
// it only takes 5 hits for a bingo
bool Tester::testPlayOne() {
    int   rows = 5;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result;

    for (int c = 0; c < object.m_numCols; c++) {
        for (int r = 0; r < object.m_numRows; r++) {
            int columnMin =
                object.m_minBallVal +
                ((object.m_maxBallVal - object.m_minBallVal) * (0.2 * c));
            object.m_card[r][c].m_value = columnMin + r;
            int val                     = columnMin + r;
            object.m_helper[val].setRow(r);
            object.m_helper[val].setCol(c);
            object.m_helper[val].setVal(val);
        }
    }
    /*for (int r = 0; r < object.m_numRows; r++) {
        for (int c = 0; c < object.m_numCols; c++) {
            cout << "[" << object.m_card[r][c].m_value << "], ";
        }
        cout << endl;
    }*/
    vector<int> rndBalls;
    for (int i = min; i <= max; i++) {
        rndBalls.push_back(i);
    }

    int numHits  = 0;
    int numDraws = 10;
    numHits      = object.play(numDraws, rndBalls);

    // cout << numHits << endl;

    if (numHits == rows) {
        result = true;
    } else {
        result = false;
    }

    return result;
}

// This tester check play with an edge case
// I manuall fill the card wit the first column containing numbers 11,12 in a
// row I then manually fill the vector for rndBalls with 11,12 in a row in the
// front The play fucntion should return 2 since it only takes 2 hits for a
// bingo
bool Tester::testPlayTwo() {
    int   rows = 2;
    int   cols = 5;
    int   max  = 90;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result;

    for (int c = 0; c < object.m_numCols; c++) {
        for (int r = 0; r < object.m_numRows; r++) {
            int columnMin =
                object.m_minBallVal +
                ((object.m_maxBallVal - object.m_minBallVal) * (0.2 * c));
            object.m_card[r][c].m_value = columnMin + r;
            int val                     = columnMin + r;
            object.m_helper[val].setRow(r);
            object.m_helper[val].setCol(c);
            object.m_helper[val].setVal(val);
        }
    }
    /*for (int r = 0; r < object.m_numRows; r++) {
        for (int c = 0; c < object.m_numCols; c++) {
            cout << "[" << object.m_card[r][c].m_value << "], ";
        }
        cout << endl;
    }*/

    vector<int> rndBalls;
    for (int i = min; i <= max; i++) {
        rndBalls.push_back(i);
    }

    int numHits  = 0;
    int numDraws = 10;
    numHits      = object.play(numDraws, rndBalls);

    // cout << numHits << endl;

    if (numHits == rows) {
        result = true;
    } else {
        result = false;
    }

    return result;
}

// This tester check play with an error case
// I then manually fill the vector for rndBalls with balls in a row starting at
// min The play fucntion should return 0 hits since object is empty because it
// isn't valid
bool Tester::testPlayThree() {
    int   rows = -5;
    int   cols = -10;
    int   max  = 85;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result;

    int numHits  = 0;
    int numDraws = max;

    vector<int> rndBalls;
    for (int i = min; i <= max; i++) {
        rndBalls.push_back(i);
    }

    numHits = object.play(numDraws, rndBalls);

    if (numHits == 0) {
        result = true;
    } else {
        result = false;
    }

    return result;
}

// This tester check play with an error case
// I manuall fill the card wit the first column containing numbers
// 11,12,13,14,15 in a row I then manually fill the vector for rndBalls with
// 11,12,13,14,15 The play fucntion should return 3 since the only number of
// draws I allow is 3
bool Tester::testPlayFour() {
    int   rows = 5;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object(rows, cols, min, max);
    bool  result;

    int numHits  = 0;
    int numDraws = 3;

    for (int c = 0; c < object.m_numCols; c++) {
        for (int r = 0; r < object.m_numRows; r++) {
            int columnMin =
                object.m_minBallVal +
                ((object.m_maxBallVal - object.m_minBallVal) * (0.2 * c));
            object.m_card[r][c].m_value = columnMin + r;
            int val                     = columnMin + r;
            object.m_helper[val].setRow(r);
            object.m_helper[val].setCol(c);
            object.m_helper[val].setVal(val);
        }
    }

    vector<int> rndBalls;
    for (int i = min; i <= max; i++) {
        rndBalls.push_back(i);
    }

    numHits = object.play(numDraws, rndBalls);

    if (numHits == numDraws) {
        result = true;
    } else {
        result = false;
    }

    return result;
}

// this tester checks the aissignment operator with a normal case
// after asignming an objet to another I go through each member variable, check
// if they are =
bool Tester::testAssignmentNormal() {
    Bingo object1(5, 5, 11, 85);

    object1.initCard();

    Bingo object2;

    bool result = true;

    object2 = object1;

    for (int i = 0; i < object1.m_numRows; i++) {
        for (int j = 0; j < object1.m_numCols; j++) {
            result = result && (object2.m_card[i][j] == object1.m_card[i][j]);
        }
    }

    for (int i = 0; i < object1.m_helperSize; i++) {
        result = result && (object2.m_helper[i] == object1.m_helper[i]);
    }

    for (int i = 0; i < object1.m_numRows; i++) {
        result = result && (object2.m_trackRows[i] == object1.m_trackRows[i]);
    }

    for (int i = 0; i < object1.m_numCols; i++) {
        result = result && (object2.m_trackCols[i] == object1.m_trackCols[i]);
    }
    result = result && (object2.m_minBallVal == object1.m_minBallVal);
    result = result && (object2.m_maxBallVal == object1.m_maxBallVal);

    return result;
}

// this tester checks the aissignment operator with a edge case
//  I set an  empty object equal to another
// I go through each member variable andcheck if they are equal
bool Tester::testAssignmentEdge() {
    int   rows = 5;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object1(rows, cols, min, max);

    object1.initCard();

    int   rows2 = -5;
    int   cols2 = -10;
    int   max2  = 85;
    int   min2  = 11;
    Bingo object2(rows2, cols2, min2, max2);

    bool result = true;

    object2 = object1;

    for (int i = 0; i < object1.m_numRows; i++) {
        for (int j = 0; j < object1.m_numCols; j++) {
            result = result && (object2.m_card[i][j] == object1.m_card[i][j]);
        }
    }

    for (int i = 0; i < object1.m_helperSize; i++) {
        result = result && (object2.m_helper[i] == object1.m_helper[i]);
    }

    for (int i = 0; i < object1.m_numRows; i++) {
        result = result && (object2.m_trackRows[i] == object1.m_trackRows[i]);
    }

    for (int i = 0; i < object1.m_numCols; i++) {
        result = result && (object2.m_trackCols[i] == object1.m_trackCols[i]);
    }
    result = result && (object2.m_minBallVal == object1.m_minBallVal);
    result = result && (object2.m_maxBallVal == object1.m_maxBallVal);
    return result;

    return result;
}

// this tester checks for self assignment with the aissignment operator
// I used 2 variables, before and after to check that the cells are the same
// they should be the same because self asighnment causes asignment operator to
// return *this
bool Tester::testSelfAssignment() {
    int   rows = 5;
    int   cols = 5;
    int   max  = 85;
    int   min  = 11;
    Bingo object1(rows, cols, min, max);

    object1.initCard();
    Cell before = object1.m_card[rows - 1][cols - 1];
    bool result = true;
    object1     = object1;
    Cell after  = object1.m_card[rows - 1][cols - 1];

    if (before == after) {
        result = true;
    } else {
        result = false;
    }

    return result;
}
