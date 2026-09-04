// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"

class Tester {
public:
    // This function is a sample test function
    // It shows how to write a test case
    bool assignmentNormal(
        Bingo &lhs,
        Bingo &rhs
    ) {
        // we expect that lhs object is an exact copy of rhs object
        bool result = true;
        // we expect that the corresponding cells in lhs and rhs
        //      cards carry the same cell information (exact same copy)
        for (int i = 0; i < rhs.m_numRows; i++) {
            for (int j = 0; j < rhs.m_numCols; j++) {
                result = result && (lhs.m_card[i][j] == rhs.m_card[i][j]);
            }
        }
        // we expect that the corresponding cells in lhs and rhs
        //      m_helper carry the same cell information (exact same copy)
        for (int i = 0; i < rhs.m_helperSize; i++) {
            result = result && (lhs.m_helper[i] == rhs.m_helper[i]);
        }
        // we expect that the corresponding cells in lhs and rhs
        //      m_trackRows carry the same cell information (exact same copy)
        for (int i = 0; i < rhs.m_numRows; i++) {
            result = result && (lhs.m_trackRows[i] == rhs.m_trackRows[i]);
        }
        // we expect that the corresponding cells in lhs and rhs
        //      m_trackCols carry the same cell information (exact same copy)
        for (int i = 0; i < rhs.m_numCols; i++) {
            result = result && (lhs.m_trackCols[i] == rhs.m_trackCols[i]);
        }
        result = result && (lhs.m_minBallVal == rhs.m_minBallVal);
        result = result && (lhs.m_maxBallVal == rhs.m_maxBallVal);
        return result;
    }

    bool testDefaultConstructor() {
        Bingo object;
        bool  result = true;
        result       = result && (object.m_numRows == CARDROWS);
        result       = result && (object.m_numCols == CARDCOLS);
        result       = result && (object.m_minBallVal == MINVAL);
        result       = result && (object.m_maxBallVal == MAXVAL);
        result       = result && (object.m_helperSize == (MAXVAL - MINVAL + 1));
        result       = result && (object.m_trackCols == nullptr);
        result       = result && (object.m_trackRows == nullptr);
        result       = result && (object.m_card == nullptr);
        result       = result && (object.m_helper == nullptr);

        return result;
    }

    bool testOptionalConstructor() {
        int   rows = 5;
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
        result = result && (object.m_helperSize == (max - min + 1));
        result = result && (object.m_trackCols != nullptr);
        result = result && (object.m_trackRows != nullptr);
        result = result && (object.m_card != nullptr);
        // cout << "mem allocated for m_card" << endl;
        result = result && (object.m_helper != nullptr);

        object.initCard();
        object.dumpCard();

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
    tester.testDefaultConstructor() ? cout << "Def Con Passed " << endl
                                    : cout << "Def Con Fail" << endl;

    tester.testOptionalConstructor() ? cout << "Opt Con Passed"
                                     : cout << "Opt Con Fail";

    return 0;
}
