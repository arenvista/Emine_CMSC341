// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"

Bingo::Bingo() {
    m_numRows    = CARDROWS;
    m_numCols    = CARDCOLS;
    m_minBallVal = MINVAL;
    m_maxBallVal = MAXVAL;
    m_helperSize = (MAXVAL - MINVAL + 1);
    m_trackCols  = new int[m_numCols];
    m_trackRows  = new int[m_numRows];
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
    if (!validateParams(rows, columns, min, max)) {
        m_numRows    = 0;
        m_numCols    = 0;
        m_minBallVal = 0;
        m_maxBallVal = 0;
        m_helperSize = 0;
        m_trackCols  = new int[m_numCols];

        m_trackRows = new int[m_numRows];

        m_card   = nullptr;
        m_helper = nullptr;
        return;
    }
    m_numRows    = rows;
    m_numCols    = columns;
    m_minBallVal = min;
    m_maxBallVal = max;
    m_helperSize = (max - min + 1);
    m_trackCols  = new int[columns];
    m_trackRows  = new int[rows];
    m_card       = new Cell *[rows];
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

        m_trackRows = new int[m_numRows];

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
    m_trackRows  = new int[m_numRows];
    m_card       = new Cell *[m_numRows];
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

    Random R(m_minBallVal, m_maxBallVal, SHUFFLE);
    R.getShuffle(balls);
    return balls;
}

int Bingo::play(
    int         numDraws,
    vector<int> rndBalls
) {
    // an empty object cannot play
    if (m_card == nullptr) {
        return 0;
    }
    // the number of balls that exist in this game, which is what the rules
    // mean by the max ball number; the range is counted rather than
    // m_maxBallVal alone so a full legal draw still fits when m_minBallVal
    // is not 1
    const int ballCount = m_maxBallVal - m_minBallVal + 1;

    // error cases in which no drawing is performed: more drawings than there
    // are balls, or a ball list holding more balls than there are balls
    if (numDraws <= 0 || numDraws > ballCount) {
        return 0;
    }
    if ((int)rndBalls.size() > ballCount) {
        return 0;
    }
    // we cannot draw more balls than the list holds
    if (numDraws > (int)rndBalls.size()) {
        return 0;
    }

    int cellsUpdated = 0;
    for (int i = 0; i < numDraws; i++) {
        if (updateCard(rndBalls[i])) {
            cellsUpdated++;
        }
        // the game ends as soon as a row or a column is complete
        if (checkBingo()) {
            break;
        }
    }
    return cellsUpdated;
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
    m_trackRows  = new int[m_numRows];
    m_card       = new Cell *[m_numRows];

    for (int r = 0; r < m_numRows; r++) {
        m_card[r] = new Cell[m_numCols];
        for (int c = 0; c < m_numCols; c++) {
            m_card[r][c] = rhs.m_card[r][c];
        }
    }

    for (int i = 0; i < m_helperSize; i++) {
        m_helper[i] = rhs.m_helper[i];
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

bool Bingo::updateCard(int ballNum) {
    // an empty object holds no cell to empty
    if (m_card == nullptr || m_helper == nullptr) {
        return false;
    }
    // a ball outside the range of the card is never on the card
    if (ballNum < 0 || ballNum >= m_helperSize) {
        return false;
    }
    // a negative row marks a helper slot that holds no cell of the card
    if (m_helper[ballNum].getRow() < 0) {
        return false;
    }

    int row = m_helper[ballNum].getRow();
    int col = m_helper[ballNum].getCol();

    // a repeated ball must not be counted, and must not push the row and
    // column counters past the size of the card
    if (m_card[row][col].getVal() == EMPTYCELL) {
        return false;
    }

    m_card[row][col].setVal(EMPTYCELL);
    m_trackRows[row]++;
    m_trackCols[col]++;
    return true;
}

// Helper Function
bool Bingo::checkBingo() {
    // an empty object has no card to win on
    if (m_trackRows == nullptr || m_trackCols == nullptr) {
        return false;
    }

    // Checking for wining conditions: row, and column hits (OMITING DIAGONALS)

    // Check for row bingo
    for (int i = 0; i < m_numRows; i++) {
        if (m_trackRows[i] == m_numCols) {
            return true;
        }
    }

    // Check for column bingo
    for (int i = 0; i < m_numCols; i++) {
        if (m_trackCols[i] == m_numRows) {
            return true;
        }
    }

    return false;
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
