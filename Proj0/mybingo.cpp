// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"

Bingo::Bingo() {
    m_numRows    = CARDROWS;
    m_numCols    = CARDCOLS;
    m_minBallVal = MINVAL;
    m_maxBallVal = MAXVAL;
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
    if (validParams(rows, columns, min, max)) {
        m_numRows    = rows;
        m_numCols    = columns;
        m_minBallVal = min;
        m_maxBallVal = max;
    } else {
        // invalid parameters produce an empty object, i.e. the same object
        // the default constructor creates
        m_numRows    = CARDROWS;
        m_numCols    = CARDCOLS;
        m_minBallVal = MINVAL;
        m_maxBallVal = MAXVAL;
    }
    // the constructor does not allocate, initCard() does
    m_helperSize = 0;
    m_trackCols  = nullptr;
    m_trackRows  = nullptr;
    m_card       = nullptr;
    m_helper     = nullptr;
}

Bingo::~Bingo() {
    clear();
}

bool Bingo::validParams(
    int rows,
    int columns,
    int min,
    int max
) {
    // a card needs at least one cell
    if (rows <= 0 || columns <= 0) {
        return false;
    }
    // m_helper is indexed by the ball number, so a ball number cannot be
    // negative, and a range needs a smaller min than max
    if (min < 0 || min >= max) {
        return false;
    }
    // a ball number appears at most once on the card, so the range has to
    // hold at least as many values as the card has cells
    if (max - min + 1 < rows * columns) {
        return false;
    }
    // EMPTYCELL marks a cell whose ball has been drawn, so it cannot also be
    // a live value on the card: a cell holding it would read as drawn from
    // the start and its row and column could never be completed
    if (min <= EMPTYCELL && EMPTYCELL <= max) {
        return false;
    }
    return true;
}

void Bingo::deallocate() {
    if (m_card != nullptr) {
        for (int i = 0; i < m_numRows; i++) {
            delete[] m_card[i];
        }
        delete[] m_card;
        m_card = nullptr;
    }
    delete[] m_helper;
    m_helper = nullptr;
    delete[] m_trackRows;
    m_trackRows = nullptr;
    delete[] m_trackCols;
    m_trackCols  = nullptr;
    m_helperSize = 0;
}

void Bingo::clear() {
    deallocate();
    m_numRows    = CARDROWS;
    m_numCols    = CARDCOLS;
    m_minBallVal = MINVAL;
    m_maxBallVal = MAXVAL;
}

bool Bingo::reCreateCard(
    int rows,
    int columns,
    int min,
    int max
) {
    // invalid parameters leave the current object untouched
    if (!validParams(rows, columns, min, max)) {
        return false;
    }
    // clear() before the new sizes are stored, so deallocate() walks the card
    // with the row count it was allocated with; calling clear() here also
    // makes the function safe when the caller did not call it first
    clear();
    m_numRows    = rows;
    m_numCols    = columns;
    m_minBallVal = min;
    m_maxBallVal = max;
    return initCard();
}

bool Bingo::initCard() {
    if (!validParams(m_numRows, m_numCols, m_minBallVal, m_maxBallVal)) {
        return false;
    }

    deallocate();

    m_helperSize = m_maxBallVal + 1;
    m_helper     = new Cell[m_helperSize];

    for (int i = 0; i < m_helperSize; i++) {
        m_helper[i].setRow(-1);
        m_helper[i].setCol(-1);
    }

    m_card = new Cell *[m_numRows];
    for (int i = 0; i < m_numRows; i++) {
        m_card[i] = new Cell[m_numCols];
    }

    m_trackCols = new int[m_numCols]();
    m_trackRows = new int[m_numRows]();

    Random      generator(m_minBallVal, m_maxBallVal, SHUFFLE);
    vector<int> values;
    generator.getShuffle(values);

    int next = 0;
    for (int i = 0; i < m_numRows; i++) {
        for (int j = 0; j < m_numCols; j++) {
            Cell cardCell(i, j, values[next]);
            next++;
            m_card[i][j]                = cardCell;
            m_helper[cardCell.getVal()] = cardCell;
        }
    }

    return true;
}

vector<int> Bingo::drawBalls() {
    // SHUFFLE returns every number of the range exactly once, so the list
    // holds no duplicate ball number
    vector<int> rndBalls;
    Random      generator(m_minBallVal, m_maxBallVal, SHUFFLE);
    generator.getShuffle(rndBalls);
    return rndBalls;
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
    // protection against self-assignment
    if (this == &rhs) {
        return *this;
    }

    // release the memory of this object before it takes a copy of rhs
    clear();

    m_numRows    = rhs.m_numRows;
    m_numCols    = rhs.m_numCols;
    m_minBallVal = rhs.m_minBallVal;
    m_maxBallVal = rhs.m_maxBallVal;
    m_helperSize = rhs.m_helperSize;

    // copying an empty object leaves this object empty as well
    if (rhs.m_card == nullptr) {
        m_helperSize = 0;
        return *this;
    }

    // a deep copy: this object gets its own memory holding the same values
    m_card = new Cell *[m_numRows];
    for (int i = 0; i < m_numRows; i++) {
        m_card[i] = new Cell[m_numCols];
        for (int j = 0; j < m_numCols; j++) {
            m_card[i][j] = rhs.m_card[i][j];
        }
    }

    m_helper = new Cell[m_helperSize];
    for (int i = 0; i < m_helperSize; i++) {
        m_helper[i] = rhs.m_helper[i];
    }

    m_trackRows = new int[m_numRows];
    for (int i = 0; i < m_numRows; i++) {
        m_trackRows[i] = rhs.m_trackRows[i];
    }

    m_trackCols = new int[m_numCols];
    for (int i = 0; i < m_numCols; i++) {
        m_trackCols[i] = rhs.m_trackCols[i];
    }

    return *this;
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
