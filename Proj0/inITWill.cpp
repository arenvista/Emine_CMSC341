/*

bool Bingo::initCard() {
    int possibleNums[MAXVAL - MINVAL + 1];
    for (int i = 0; i <= (MAXVAL - MINVAL); i++) {
        possibleNums[i] = MINVAL + i;
    }
    m_card = new Cell *[m_numRows];

    m_helper = new Cell[MAXVAL + 1];

    for (int i = MINVAL; i <= MAXVAL; i++) {
        m_helper[i].setRow(-1);
        m_helper[i].setCol(-1);
        m_helper[i].setVal(EMPTYCELL);
    }

    if (m_card == nullptr) {
        return false;
    }

    for (int r = 0; r < m_numRows; r++) {
        m_card[r] = new Cell[m_numCols];
        for (int c = 0; c < m_numCols; c++) {
            m_card[r][c].setRow(r);
            m_card[r][c].setCol(c);
            int  columnMin = MINVAL + ((MAXVAL - MINVAL) * (0.2 * c));
            int  columnMax = MINVAL + ((MAXVAL - MINVAL) * (0.2 + (0.2 * c)));
            bool uniqueVal = false;
            int  value;
            while (!uniqueVal) {
                value = columnMin + rand() % (columnMax - columnMin + 1);
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

*/
