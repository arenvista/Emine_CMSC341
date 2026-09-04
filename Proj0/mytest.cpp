// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"

class Tester {
public:
    bool testDefaultConstructors() {
        bool res = true;
        // Testing default constructor
        Bingo obj0;
        res = res && (obj0.m_trackCols == nullptr);
        res = res && (obj0.m_trackRows == nullptr);
        res = res && (obj0.m_helper == nullptr);

        res = res && (obj0.m_helperSize == 0);
        res = res && (obj0.m_card == nullptr);
        res = res && (obj0.m_numRows == CARDROWS);
        res = res && (obj0.m_numCols == CARDCOLS);
        res = res && (obj0.m_minBallVal == MINVAL);
        res = res && (obj0.m_maxBallVal == MAXVAL);

        obj0.initCard();
        obj0.dumpCard();
        return res;
    }

    bool testOptConstructor() {
        bool res = true;
        // Testing Optional constructor with valid parameters
        // the range holds 50 values and the card has 25 cells, so every cell
        // can be given a different ball number
        const int rows = 5;
        const int cols = 5;
        const int min  = 1;
        const int max  = 50;

        Bingo obj0(rows, cols, min, max);
        res = res && (obj0.m_trackCols == nullptr);
        res = res && (obj0.m_trackRows == nullptr);
        res = res && (obj0.m_helper == nullptr);

        res = res && (obj0.m_helperSize == 0);
        res = res && (obj0.m_card == nullptr);
        res = res && (obj0.m_numRows == rows);
        res = res && (obj0.m_numCols == cols);
        res = res && (obj0.m_minBallVal == min);
        res = res && (obj0.m_maxBallVal == max);
        obj0.initCard();
        obj0.dumpCard();
        return res;
    }

    bool testInvalidConstructor() {
        bool res = true;
        // Testing the constructor with invalid parameters
        // min is not smaller than max, and a single value cannot fill the 100
        // cells of a 10x10 card, so the constructor must create an empty
        // object holding the default values instead
        Bingo obj0(10, 10, 10, 10);
        res = res && (obj0.m_trackCols == nullptr);
        res = res && (obj0.m_trackRows == nullptr);
        res = res && (obj0.m_helper == nullptr);

        res = res && (obj0.m_helperSize == 0);
        res = res && (obj0.m_card == nullptr);
        res = res && (obj0.m_numRows == CARDROWS);
        res = res && (obj0.m_numCols == CARDCOLS);
        res = res && (obj0.m_minBallVal == MINVAL);
        res = res && (obj0.m_maxBallVal == MAXVAL);

        // a negative size is invalid as well
        Bingo obj1(-1, CARDCOLS, MINVAL, MAXVAL);
        res = res && (obj1.m_numRows == CARDROWS);
        res = res && (obj1.m_numCols == CARDCOLS);

        // reCreateCard() must reject the same parameters, and it must leave
        // the object it was called on untouched
        res = res && (obj0.reCreateCard(10, 10, 10, 10) == false);
        res = res && (obj0.m_card == nullptr);
        return res;
    }

private:
    /**********************************************
     * If we need helper functions to be reused in *
     *   test functions they can be declared here!
     **********************************************/
};

int main() {
    Tester tester;
    tester.testDefaultConstructors()
        ? cout << "Default constructor test passed!" << endl
        : cout << "Default constructor test failed!" << endl;

    tester.testOptConstructor()
        ? cout << "Opt constructor test passed!" << endl
        : cout << "Opt constructor test failed!" << endl;

    tester.testInvalidConstructor()
        ? cout << "Invalid parameter constructor test passed!" << endl
        : cout << "Invalid parameter constructor test failed!" << endl;
}
