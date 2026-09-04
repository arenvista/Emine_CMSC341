// UMBC - CMSC 341 - Fall 2026 - Proj0
#include "bingo.h"
Bingo::Bingo(){
    
}

Bingo::Bingo(int rows, int columns, int min, int max){
    
}

bool Bingo::reCreateCard(int rows, int columns, int min, int max){
            
}

Bingo::~Bingo(){
    
}

void Bingo::clear(){
    
}

bool Bingo::initCard(){
    
}

vector<int> Bingo::drawBalls(){
    
}

int Bingo::play(int numDraws, vector<int> rndBalls){
    
}

const Bingo & Bingo::operator=(const Bingo & rhs){
    return *this;
}

// The dump function renders the card in the terminal
// This function is provided to facilitate debugging
// Using this function as a test case will not be accepted
void Bingo::dumpCard(){
    cout << "  ";
    cout << "\033[1;35m B   I   N   G   O\033[0m";    
    cout << endl;
    for (int i=1;i <= m_numRows;i++){
        if ((i < 10))
            cout << "\033[1;35m" << "0" << i << " \033[0m";
        else
            cout << "\033[1;35m" << i << " \033[0m";
        for (int j=1;j<=m_numCols;j++){
            if (m_card[i-1][j-1].getVal() == EMPTYCELL)
                cout << "\033[1;31m" << m_card[i-1][j-1].getVal() << "\033[0m" << "  ";
            else
                cout << m_card[i-1][j-1].getVal() << "  ";
        }
        cout << endl;
    }
    cout << endl;
}

// the overloaded equality operator for the Cell object
bool operator==(const Cell & lhs, const Cell & rhs){
    return ((lhs.m_col == rhs.m_col) &&
            (lhs.m_row == rhs.m_row) &&
            (lhs.m_value == rhs.m_value));
}
