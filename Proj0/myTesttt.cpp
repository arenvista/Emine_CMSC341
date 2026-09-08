// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"

class Tester {
public:
    bool testDefaultConstructor() {
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

    bool testOptionalConstructorOne() {
        // egde?
        int   rows = 10;
        int   cols = 5;
        int   max  = 90;
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

    bool testOptionalConstructorTwo() {
        // egde
        int   rows = 2;
        int   cols = 5;
        int   max  = 90;
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

    bool testOptionalConstructorThree() {
        // egde?
        int   rows = -5;
        int   cols = -10;
        int   max  = 90;
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

    bool testrecreateCardOne() {
        // egde?
        int   rows = 10;
        int   cols = 5;
        int   max  = 90;
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

    bool testrecreateCardTwo() {
        // egde?
        int   rows = 2;
        int   cols = 5;
        int   max  = 90;
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

    bool testrecreateCardThree() {
        int   rows = -5;
        int   cols = -10;
        int   max  = 90;
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

    bool testInitCardOne() {
        int   rows = 5;
        int   cols = 5;
        int   max  = 90;
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
                result =
                    result && ((object.m_card[r][c].m_value >= columnMin) &&
                               (object.m_card[r][c].m_value <= columnMax));
            }
        }

        return result;
    }

    bool testInitCardTwo() {
        int   rows = 2;
        int   cols = 5;
        int   max  = 90;
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
                result =
                    result && ((object.m_card[r][c].m_value >= columnMin) &&
                               (object.m_card[r][c].m_value <= columnMax));
            }
        }
        return result;
    }

    bool testInitCardThree() {
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

    bool testPlayOne() {
        int   rows = 5;
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

    bool testPlayTwo() {
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

    bool testPlayThree() {
        int   rows = 5;
        int   cols = 5;
        int   max  = 90;
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

    bool testPlayFour() {
        int   rows = 5;
        int   cols = 5;
        int   max  = 90;
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

    bool testAssignmentNormal() {
        Bingo object1(5, 5, 11, 90);

        object1.initCard();

        Bingo object2;

        bool result = true;

        object2 = object1;

        for (int i = 0; i < object1.m_numRows; i++) {
            for (int j = 0; j < object1.m_numCols; j++) {
                result =
                    result && (object2.m_card[i][j] == object1.m_card[i][j]);
            }
        }

        for (int i = 0; i < object1.m_helperSize; i++) {
            result = result && (object2.m_helper[i] == object1.m_helper[i]);
        }

        for (int i = 0; i < object1.m_numRows; i++) {
            result =
                result && (object2.m_trackRows[i] == object1.m_trackRows[i]);
        }

        for (int i = 0; i < object1.m_numCols; i++) {
            result =
                result && (object2.m_trackCols[i] == object1.m_trackCols[i]);
        }
        result = result && (object2.m_minBallVal == object1.m_minBallVal);
        result = result && (object2.m_maxBallVal == object1.m_maxBallVal);

        return result;
    }

    bool testAssignmentEdge() {
        int   rows = 5;
        int   cols = 5;
        int   max  = 90;
        int   min  = 11;
        Bingo object1(rows, cols, min, max);

        object1.initCard();

        int   rows2 = -5;
        int   cols2 = -10;
        int   max2  = 90;
        int   min2  = 11;
        Bingo object2(rows2, cols2, min2, max2);

        bool result = true;

        object2 = object1;

        for (int i = 0; i < object1.m_numRows; i++) {
            for (int j = 0; j < object1.m_numCols; j++) {
                result =
                    result && (object2.m_card[i][j] == object1.m_card[i][j]);
            }
        }

        for (int i = 0; i < object1.m_helperSize; i++) {
            result = result && (object2.m_helper[i] == object1.m_helper[i]);
        }

        for (int i = 0; i < object1.m_numRows; i++) {
            result =
                result && (object2.m_trackRows[i] == object1.m_trackRows[i]);
        }

        for (int i = 0; i < object1.m_numCols; i++) {
            result =
                result && (object2.m_trackCols[i] == object1.m_trackCols[i]);
        }
        result = result && (object2.m_minBallVal == object1.m_minBallVal);
        result = result && (object2.m_maxBallVal == object1.m_maxBallVal);
        return result;

        return result;
    }

    bool testSelfAssignment() {
        int   rows = 5;
        int   cols = 5;
        int   max  = 90;
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