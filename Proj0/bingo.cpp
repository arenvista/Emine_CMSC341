// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"

Bingo::Bingo() {
    // default constructor sets all values to default values to  create empty
    // object
    m_numRows    = 0;
    m_numCols    = 0;
    m_minBallVal = 0;
    m_maxBallVal = 0;
    m_helperSize = 0;
    m_trackCols  = nullptr;
    m_trackRows  = nullptr;
    m_card       = nullptr;
    m_helper     = nullptr;
}

Bingo::Bingo(
    int rows,
    int columns,
    int min,
    int max
) {
    // cout << rows << columns << min << max << endl;
    // cout << validateParams(rows, columns, min, max) << endl;
    // helper functions checks the passed in variables before creating the
    // object
    if (!validateParams(rows, columns, min, max)) {
        m_numRows    = 0;
        m_numCols    = 0;
        m_minBallVal = 0;
        m_maxBallVal = 0;
        m_helperSize = 0;
        m_trackCols  = nullptr;
        m_trackRows  = nullptr;
        m_card       = nullptr;
        m_helper     = nullptr;

        return;
    }
    // if data is valid then object is created with corresponding values
    // memory is also allocated so object is no longer empty
    m_numRows    = rows;
    m_numCols    = columns;
    m_minBallVal = min;
    m_maxBallVal = max;
    m_helperSize = (max + 1);
    m_trackCols  = new int[m_numCols];
    for (int i = 0; i < m_numCols; i++) {
        m_trackCols[i] = 0;
    }
    m_trackRows = new int[m_numRows];
    for (int i = 0; i < m_numRows; i++) {
        m_trackRows[i] = 0;
    }
    m_card = new Cell *[m_numRows];
    // the double for loop allocated all memory required for the 2d array
    for (int r = 0; r < m_numRows; r++) {
        m_card[r] = new Cell[m_numCols];
        for (int c = 0; c < m_numCols; c++) {
            m_card[r][c].setRow(r);
            m_card[r][c].setCol(c);
        }
    }
    m_helper = new Cell[m_helperSize];
}

bool Bingo::reCreateCard(
    int rows,
    int columns,
    int min,
    int max
) {
    /*for recreated card unlike comstructor it will not touch the object
     if parameters aren't valid and will return false instead */
    if (!validateParams(rows, columns, min, max)) {
        /* m_numRows  = 0;
         m_numCols    = 0;
         m_minBallVal = 0;
         m_maxBallVal = 0;
         m_helperSize = 0;
         m_trackCols  = new int[m_numCols];
         m_trackRows  = new int[m_numRows];
         m_card       = nullptr;
         m_helper     = nullptr; */

        return false;
    }
    /* if parameters are valid then object is recreated with the valid
    paremters. Memory allocation happens again becase before calling this
    function, clear is also called.*/
    m_numRows    = rows;
    m_numCols    = columns;
    m_minBallVal = min;
    m_maxBallVal = max;
    m_helperSize = (max + 1);
    m_trackCols  = new int[m_numCols];
    for (int i = 0; i < m_numCols; i++) {
        m_trackCols[i] = 0;
    }
    m_trackRows = new int[m_numRows];
    for (int i = 0; i < m_numRows; i++) {
        m_trackRows[i] = 0;
    }
    m_card = new Cell *[m_numRows];
    for (int r = 0; r < m_numRows; r++) {
        m_card[r] = new Cell[m_numCols];
        for (int c = 0; c < m_numCols; c++) {
            m_card[r][c].setRow(r);
            m_card[r][c].setCol(c);
        }
    }
    m_helper = new Cell[m_helperSize];
    // function returns true once object as be recreated with valid paremters
    return true;
}

Bingo::~Bingo() {
    // destructor calls clear function to deloacte memory and reset member
    // variable to default
    clear();
}

void Bingo::clear() {
    // loops through all allocated memory in arrays to ensure delete is called
    // on everything
    if (m_card != nullptr) {
        for (int r = 0; r < m_numRows; r++) {
            delete[] m_card[r];
        }
        delete[] m_card;
        m_card = nullptr;
    }

    if (m_trackCols != nullptr) {
        delete[] m_trackCols;
        m_trackCols = nullptr;
    }
    if (m_trackRows != nullptr) {
        delete[] m_trackRows;
        m_trackRows = nullptr;
    }

    if (m_helper != nullptr) {
        delete[] m_helper;
        m_helper = nullptr;
    }
    // member variables set to default values
    m_numRows    = 0;
    m_numCols    = 0;
    m_helperSize = 0;
    m_maxBallVal = 0;
    m_minBallVal = 0;
}

bool Bingo::initCard() {
    // checks that m_card or m_helper is allocated with memory.
    if (m_card == nullptr || m_helper == nullptr) {
        return false;
    }

    // initialized the m_helper with default values to get ready for filling the
    // card
    for (int i = 0; i <= m_maxBallVal; i++) {
        m_helper[i].setRow(-1);
        m_helper[i].setCol(-1);
        m_helper[i].setVal(EMPTYCELL);
    }
    // calucate the distance between each of the 5 intervals for the 5 columns
    int distance         = (m_maxBallVal - m_minBallVal + 1);
    int distanceInterval = (distance / m_numCols);
    // cout << "[DISTANCE] = " << distance << endl;
    // cout << "[MIN] = " << m_minBallVal << endl;
    // cout << "[MAX] = " << m_maxBallVal << endl;
    // 2d vector that fills in the numbers that are in range for each specific
    // column on card
    vector<vector<int>> intervals;
    // for loop that determines the upper and lower bound for each column
    // this guarentees the that each column gets 20% of the numbs in ranged from
    // min to max
    for (int i = 0; i < m_numCols; i++) {
        int lowerBound = (m_minBallVal + (i * distanceInterval));
        int upperBound = (lowerBound + distanceInterval - 1);
        // calls helper function to call the gerneator and the getSuffle on the
        // numbers
        intervals.push_back(generateNums(lowerBound, upperBound));
    }

    // iterates through the card and sets the spots on the card as well as
    // helper
    for (int r = 0; r < m_numRows; r++) {
        for (int c = 0; c < m_numCols; c++) {
            int value = intervals[c][r];
            m_card[r][c].setVal(value);
            m_helper[value].setRow(r);
            m_helper[value].setCol(c);
            m_helper[value].setVal(value);
            // cout << "Cell [" << r << "][" << c << "] value: " << value <<
            // endl;
        }
    }

    // for (int i = MINVAL; i <= MAXVAL; i++) {
    // cout << "Cell " << i << " value is " << m_helper[i].getVal() << endl;
    //}

    return true;
}

vector<int> Bingo::drawBalls() {
    // intialized the vector filled with ints
    vector<int> balls;
    // checks that m_card isn;t empty, if so can't filled the vector
    if (m_card == nullptr) {
        return balls;
    }
    // variable used to check if vector is filled alreadey
    unsigned int expectedNumBalls = (m_maxBallVal - m_minBallVal + 1);
    // if vector is not filled then fill it with the random and shuffle
    // function/class
    if (balls.size() != expectedNumBalls) {
        Random R(m_minBallVal, m_maxBallVal, SHUFFLE);
        R.getShuffle(balls);
    }
    // return the balls wether vector has been or hasn't been filled
    return balls;
}

int Bingo::play(
    int         numDraws,
    vector<int> rndBalls
) {
    int numHits =
        0; // variable that counts how many times you find a number on card
    int countDraws = 0; // variable that counts how many times you pick a ball
    int row        = 0; // variable that will hold the row number
    int col        = 0; // variable that will hold the column number
    // checks that numDraws is smaller then the size of the ball vecotor
    if (numDraws > (m_maxBallVal - m_minBallVal + 1)) {
        // does not preform a drawing at all if it is, so return 0
        return 0;
    }

    // Maybe something wrong with iteration
    // for int (i = 0; ii < vector.size(); ii ++)
    // Loop through card (like make card) to update card
    // (Youll probably end up with three nested loops)
    // 1. loop through balls
    // ballval = vector[ii]
    // 2. Look through m_helper (if m_helper[ballval].getval() = ballval)
    // If match, set to 99
    // iterates through the random balls stored in the vector
    for (unsigned int i = 0; i < rndBalls.size(); i++) {
        // stores the picked ball in a varible
        int ballVal = rndBalls[i];
        // iterates through the helper array
        for (int j = 0; j < m_helperSize; j++) {
            // if that ball number extirts in the helper array, it is on the
            // card
            if (m_helper[j].getVal() == ballVal) {
                // m_helper[i].setVal(EMPTYCELL);
                // gets both the row and column from helper cell to find the
                // number on the card
                row = m_helper[j].getRow();
                col = m_helper[j].getCol();
                // More Debugging
                // cout << "Index: " << j << "\nballVal/m_HelperVal = " <<
                // ballVal
                // << "/" << m_helper[j].getVal() << "\nRow: " << row + 1
                // << "\nColumn: " << col + 1 << endl;
                // set that value on the card to empty cell to show that it has
                // been hit
                m_card[row][col].setVal(EMPTYCELL);
                // increase the number of hits for the row and column in the
                // tracking arrays
                m_trackRows[row] += 1;
                m_trackCols[col] += 1;
                // increase number of hits to acount for the replacement on the
                // card
                numHits++;
            }
        }
        // Debugging
        // cout << "Balls drawn: " << i << "\nBalls vector: ";
        // for (unsigned int j = 0; j < rndBalls.size(); j++) {
        //    cout << "[" << rndBalls[j] << "], ";
        // }
        // cout << "\n Current Index: rndBalls[" << i << "] = " << rndBalls[i]
        //  << endl;
        // checks if each row has been completely filled to earna bingo
        for (int i = 0; i < m_numRows; i++) {
            if (m_trackRows[i] == m_numCols) {
                // BINGO
                // cout << " bingo rows" << endl;
                // end play function by returing num of hits once bingo has been
                // earned
                return numHits;
            }
        }
        // checks if each column has been completely filled to earna bingo
        for (int i = 0; i < m_numCols; i++) {
            if (m_trackCols[i] == m_numRows) {
                // BINGO
                // cout << " bingo cols" << endl;
                // cout << i << endl;
                // end play function by returing num of hits once bingo has been
                // earned
                return numHits;
            }
        }
        // increases the amount of balls drawn after a turn
        countDraws++;
        // checks if the amount of balls drawn is = to amount allowed in a game
        if (countDraws == numDraws) {
            // cout << "countDraws = numDraws" << endl;
            // if it is then game is ended
            return numHits;
        }
    }

    return numHits;
}

const Bingo &Bingo::operator=(const Bingo &rhs) {
    // checks for self asignment
    if (this == &rhs) {
        return *this;
    }
    clear();
    // once card has been cleared, set all member variables equal to whats on
    // RHS
    m_maxBallVal = rhs.m_maxBallVal;
    m_minBallVal = rhs.m_minBallVal;
    m_numRows    = rhs.m_numRows;
    m_numCols    = rhs.m_numCols;
    m_helperSize = rhs.m_helperSize;
    // allocated memory for variables that require it
    m_helper    = new Cell[m_helperSize];
    m_trackCols = new int[m_numCols];
    for (int i = 0; i < m_numCols; i++) {
        m_trackCols[i] = 0;
    }
    m_trackRows = new int[m_numRows];
    for (int i = 0; i < m_numRows; i++) {
        m_trackRows[i] = 0;
    }
    // double for loop to ensure each spot on m_card is = to the rhs
    m_card = new Cell *[m_numRows];
    for (int r = 0; r < m_numRows; r++) {
        m_card[r] = new Cell[m_numCols];
        for (int c = 0; c < m_numCols; c++) {
            m_card[r][c].setCol(rhs.m_card[r][c].getCol());
            m_card[r][c].setRow(rhs.m_card[r][c].getRow());
            m_card[r][c].setVal(rhs.m_card[r][c].getVal());
        }
    }
    // for loop allows each indez in helper to be = to whats on the rhs
    for (int i = 0; i < m_helperSize; i++) {
        // m_helper[i] = rhs.m_helper[i];
        m_helper[i].setCol(rhs.m_helper[i].getCol());
        m_helper[i].setRow(rhs.m_helper[i].getRow());
        m_helper[i].setVal(rhs.m_helper[i].getVal());
    }
    // also set each trackers to whats = on rhs by using more for loops
    for (int i = 0; i < m_numCols; i++) {
        m_trackCols[i] = rhs.m_trackCols[i];
    }
    for (int i = 0; i < m_numRows; i++) {
        m_trackRows[i] = rhs.m_trackRows[i];
    }
    // return copied object
    return *this;
}

bool Bingo::validateParams(
    int rows,
    int columns,
    int min,
    int max
) {
    // cout << rows << columns << min << max << endl;
    // check that min is less then the max for to ensure no bugs
    if (min >= max) {
        cout << "your max and min are likely flipped\n";
        return false;
    }
    // columns have to be 5
    if (columns != 5) {
        // cout << "Failed Col Check \n";
        return false;
        // rows have to be in beteen the range of 2 to `15
    } else if (rows < 2 || rows > 15) {
        cout << "Failed Row Check";
        return false;
        // and the range from max to min ball values must be divisible by 5
    } else if (((max - min + 1) % 5) != 0) {
        int inner    = (max - min + 1);
        int modCheck = inner % 5;
        cout << "Fail Divisibility Check => " << max << " " << min << " \n";
        cout << "Inner => " << inner << " modCheck => " << modCheck << "\n";
        return false;
    }
    return true;
}

// @brief Genearates a list of distinct numbers x in [min, max]
// @note this range is inclusive of the max and minimum values
vector<int> Bingo::generateNums(
    int min,
    int max
) {
    Random      R(min, max, SHUFFLE);
    vector<int> numsInRange;
    R.getShuffle(numsInRange);

    return numsInRange;
}

// The dump function renders the card in the terminal
// This function is provided to facilitate debugging
// Using this function as a test case will not be accepted
void Bingo::dumpCard() {
    cout << "  ";
    cout << "\033[1;35m B   I   N   G   O\033[0m";
    cout << endl;
    for (int i = 1; i <= m_numRows; i++) {
        if ((i < 10)) {
            cout << "\033[1;35m" << "0" << i << " \033[0m";
        } else {
            cout << "\033[1;35m" << i << " \033[0m";
        }
        for (int j = 1; j <= m_numCols; j++) {
            if (m_card[i - 1][j - 1].getVal() == EMPTYCELL) {
                cout << "\033[1;31m" << m_card[i - 1][j - 1].getVal()
                     << "\033[0m" << "  ";
            } else {
                cout << m_card[i - 1][j - 1].getVal() << "  ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// the overloaded equality operator for the Cell object
bool operator==(
    const Cell &lhs,
    const Cell &rhs
) {
    return (
        (lhs.m_col == rhs.m_col) && (lhs.m_row == rhs.m_row) &&
        (lhs.m_value == rhs.m_value)
    );
}
