// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"

Bingo::Bingo() {
    m_numRows    = CARDROWS;
    m_numCols    = CARDCOLS;
    m_minBallVal = MINVAL;
    m_maxBallVal = MAXVAL;
    m_helperSize = (MAXVAL - MINVAL + 1);
    m_trackCols  = new int[m_numCols];
    for (int i = 0; i < m_numCols; i++) {
        m_trackCols[i] = 0;
    }
    m_trackRows = new int[m_numRows];
    for (int i = 0; i < m_numRows; i++) {
        m_trackRows[i] = 0;
    }
    m_card   = nullptr;
    m_helper = nullptr;
}

Bingo::Bingo(
    int rows,
    int columns,
    int min,
    int max
) {
    // cout << rows << columns << min << max << endl;
    // cout << validateParams(rows, columns, min, max) << endl;
    if (!validateParams(rows, columns, min, max)) {
        m_numRows    = 0;
        m_numCols    = 0;
        m_minBallVal = 0;
        m_maxBallVal = 0;
        m_helperSize = 0;
        m_trackCols  = new int[m_numCols];
        m_trackRows  = new int[m_numRows];
        m_card       = nullptr;
        m_helper     = nullptr;
        return;
    }
    m_numRows    = rows;
    m_numCols    = columns;
    m_minBallVal = min;
    m_maxBallVal = max;
    m_helperSize = (max - min + 1);
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
    m_helper = new Cell[max + 1];
}

bool Bingo::reCreateCard(
    int rows,
    int columns,
    int min,
    int max
) {
    if (!validateParams(rows, columns, min, max)) {
        m_numRows    = 0;
        m_numCols    = 0;
        m_minBallVal = 0;
        m_maxBallVal = 0;
        m_helperSize = 0;
        m_trackCols  = new int[m_numCols];
        for (int i = 0; i < m_numCols; i++) {
            m_trackCols[i] = 0;
        }
        m_trackRows = new int[m_numRows];
        for (int i = 0; i < m_numRows; i++) {
            m_trackRows[i] = 0;
        }
        m_card   = nullptr;
        m_helper = nullptr;
        return false;
    }

    m_numRows    = rows;
    m_numCols    = columns;
    m_minBallVal = min;
    m_maxBallVal = max;
    m_helperSize = (max - min + 1);
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
    return true;
}

Bingo::~Bingo() {
    clear();
}

void Bingo::clear() {
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
}

/* m_numRows    = rows;
    m_numCols    = columns;
    m_minBallVal = min;
    m_maxBallVal = max;
*/

// cell = row, columns, value
bool Bingo::initCard() {
    int possibleNums[MAXVAL - MINVAL + 1];
    for (int i = 0; i <= (MAXVAL - MINVAL); i++) {
        possibleNums[i] = MINVAL + i;
    }

    for (int i = m_minBallVal; i <= m_maxBallVal; i++) {
        m_helper[i].setRow(-1);
        m_helper[i].setCol(-1);
        m_helper[i].setVal(EMPTYCELL);
    }

    if (m_card == nullptr) {
        return false;
    }
    if (m_helper == nullptr) {
        return false;
    }
    Random random(m_minBallVal, m_maxBallVal, UNIFORMINT);

    for (int r = 0; r < m_numRows; r++) {
        for (int c = 0; c < m_numCols; c++) {
            int columnMin =
                m_minBallVal + ((m_maxBallVal - m_minBallVal) * (0.2 * c));
            int columnMax = m_minBallVal +
                            ((m_maxBallVal - m_minBallVal) * (0.2 + (0.2 * c)));
            bool uniqueVal = false;
            int  value;
            while (!uniqueVal) {
                value = columnMin +
                        random.getRandNum() % (columnMax - columnMin + 1);
                for (int i = 0; i <= (MAXVAL - MINVAL); i++) {
                    if (possibleNums[i] == value) {
                        m_card[r][c].setVal(value);
                        m_helper[value].setRow(r);
                        m_helper[value].setCol(c);
                        m_helper[value].setVal(value);
                        possibleNums[i] = -1;
                        uniqueVal       = true;
                    }
                }
            }
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
    vector<int> balls;

    if (m_card == nullptr) {
        return balls;
    }
    int expectedNumBalls = (m_maxBallVal - m_minBallVal + 1);
    if (balls.size() == expectedNumBalls) {
        Random R(m_minBallVal, m_maxBallVal, SHUFFLE);
        R.getShuffle(balls);
    }
    return balls;
}

int Bingo::play(
    int         numDraws,
    vector<int> rndBalls
) {
    int numHits    = 0;
    int countDraws = 0;
    int row        = 0;
    int col        = 0;

    if (numDraws > (m_maxBallVal - m_minBallVal + 1)) {
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

    for (unsigned int i = 0; i < rndBalls.size(); i++) {
        int ballVal = rndBalls[i];
        if (m_helper[ballVal].getVal() == ballVal) {
            // m_helper[i].setVal(EMPTYCELL);
            row = m_helper[ballVal].getRow();
            col = m_helper[ballVal].getCol();
            m_card[row][col].setVal(EMPTYCELL);
            m_trackRows[row] += 1;
            m_trackCols[col] += 1;
            numHits++;
        }
        for (int i = 0; i < m_numRows; i++) {
            if (m_trackRows[i] == m_numRows) {
                // BINGO
                return numHits;
            }
        }
        for (int i = 0; i < m_numCols; i++) {
            if (m_trackCols[i] == m_numCols) {
                // BINGO
                return numHits;
            }
        }
        countDraws++;
    }

    if (countDraws = numDraws) {
        return numHits;
    }
}

const Bingo &Bingo::operator=(const Bingo &rhs) {
    if (this == &rhs) {
        return *this;
    }
    clear();

    m_maxBallVal = rhs.m_maxBallVal;
    m_minBallVal = rhs.m_minBallVal;
    m_numRows    = rhs.m_numRows;
    m_numCols    = rhs.m_numCols;
    m_helperSize = rhs.m_helperSize;
    m_helper     = new Cell[m_helperSize];
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
            m_card[r][c].setCol(rhs.m_card[r][c].getCol());
            m_card[r][c].setRow(rhs.m_card[r][c].getRow());
            m_card[r][c].setVal(rhs.m_card[r][c].getVal());
        }
    }

    for (int i = 0; i < m_helperSize; i++) {
        // m_helper[i] = rhs.m_helper[i];
        m_helper[i].setCol(rhs.m_helper[i].getCol());
        m_helper[i].setRow(rhs.m_helper[i].getRow());
        m_helper[i].setVal(rhs.m_helper[i].getVal());
    }

    for (int i = 0; i < m_numCols; i++) {
        m_trackCols[i] = rhs.m_trackCols[i];
    }
    for (int i = 0; i < m_numRows; i++) {
        m_trackRows[i] = rhs.m_trackRows[i];
    }
    return *this;
}

bool Bingo::validateParams(
    int rows,
    int columns,
    int min,
    int max
) {
    // cout << rows << columns << min << max << endl;
    if (min >= max) {
        cout << "your max and min are likely flipped\n";
        return false;
    }
    if (columns != 5) {
        cout << "Failed Col Check \n";
        return false;
    } else if (rows < 2 || rows > 15) {
        cout << "Failed Row Check";
        return false;
    } else if (((max - min + 1) % 5) != 0) {
        int inner    = (max - min + 1);
        int modCheck = inner % 5;
        cout << "Fail Divisibility Check => " << max << " " << min << " \n";
        cout << "Inner => " << inner << " modCheck => " << modCheck << "\n";
        return false;
    }
    return true;
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
