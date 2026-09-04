// UMBC - CMSC 341 - Fall 2026 - Proj0
#ifndef BINGO_H
#define BINGO_H
#include <algorithm>
#include <iostream>
#include <math.h>
#include <random>
#include <string>
#include <vector>
using namespace std;
class Grader; // this class is for grading purposes, no need to do anything
class Tester; // this is your tester class, you add your test functions in this
              // class
const int CARDROWS  = 5;               // default number of rows
const int CARDCOLS  = 5;               // default number of columns
const int MINVAL    = 11;              // default min value in a cell
const int MAXVAL    = 85;              // default max value in a cell
const int EMPTYCELL = 99;              // empty cell (already drawn)
const int BALLS = MAXVAL - MINVAL + 1; // the number of balls that we can draw

/**
 * @brief A single cell of a bingo card.
 *
 * A cell knows its position on the card (row and column) and the ball number
 * it holds. A cell whose value is EMPTYCELL is considered empty, i.e. the
 * corresponding ball has already been drawn.
 */
class Cell {
public:
    friend class Grader;
    friend class Tester;

    /**
     * @brief Constructs a cell at the given position holding the given value.
     *
     * Calling the constructor with default params creates an empty object.
     *
     * @param row the row index of the cell on the card
     * @param col the column index of the cell on the card
     * @param val the ball number stored in the cell, EMPTYCELL if empty
     */
    Cell(
        int row = 0,
        int col = 0,
        int val = EMPTYCELL
    ) {
        m_row = row, m_col = col, m_value = val;
    }

    /**
     * @brief Destructor. A Cell holds no dynamic memory, so nothing is freed.
     */
    ~Cell() {}

    /**
     * @brief Returns the row index of the cell.
     * @return the row index of the cell on the card
     */
    int getRow() { return m_row; }

    /**
     * @brief Returns the column index of the cell.
     * @return the column index of the cell on the card
     */
    int getCol() { return m_col; }

    /**
     * @brief Returns the value stored in the cell.
     * @return the ball number stored in the cell, EMPTYCELL if empty
     */
    int getVal() { return m_value; }

    /**
     * @brief Sets the row index of the cell.
     * @param row the new row index
     * @return void
     */
    void setRow(int row) { m_row = row; }

    /**
     * @brief Sets the column index of the cell.
     * @param col the new column index
     * @return void
     */
    void setCol(int col) { m_col = col; }

    /**
     * @brief Sets the value stored in the cell.
     * @param val the new ball number, EMPTYCELL to empty the cell
     * @return void
     */
    void setVal(int val) { m_value = val; }

    /**
     * @brief Reports whether the cell is empty.
     * @return true if the cell value is EMPTYCELL, false otherwise
     */
    bool isEmpty() {
        if (m_value == EMPTYCELL) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief Compares two cells for equality.
     *
     * Two cells are equal when their row, column, and value all match.
     *
     * @param lhs the cell on the left hand side of the operator
     * @param rhs the cell on the right hand side of the operator
     * @return true if both cells hold the same row, column, and value
     */
    friend bool operator==(
        const Cell &lhs,
        const Cell &rhs
    );

private:
    int m_row;
    int m_col;
    int m_value;
};

/**
 * @brief A bingo card and the game played on it.
 *
 * The class stores a 2D array of Cell objects along with helper structures
 * that make it possible to look a ball number up in the card and to detect a
 * completed row or column.
 */
class Bingo {
public:
    friend class Grader;
    friend class Tester;

    /**
     * @brief Default constructor. Creates an empty object.
     *
     * An empty object does not hold any memory. All member variables are
     * initialized to their default values.
     */
    Bingo();

    /**
     * @brief Constructs a card with the given dimensions and ball range.
     *
     * Initializes all member variables and allocates the required memory if
     * necessary. If any of the parameters is invalid, e.g. a negative size,
     * the constructor creates an empty object instead. The parameters min and
     * max specify the range of numbers that can be placed on a card; they also
     * indicate the smallest and the largest ball numbers that can be drawn.
     *
     * @param rows    the number of rows of the card
     * @param columns the number of columns of the card
     * @param min     the smallest value that can appear on the card
     * @param max     the largest value that can appear on the card
     */
    Bingo(
        int rows,
        int columns,
        int min,
        int max
    );

    /**
     * @brief Destructor. Deallocates all memory held by the object.
     */
    ~Bingo();

    /**
     * @brief Clears the object back to an empty object.
     *
     * Deallocates all memory and re-initializes all member variables to their
     * default values.
     *
     * @return void
     */
    void clear();

    /**
     * @brief Creates a new card with randomly generated numbers.
     *
     * Generally we call this function after calling clear(). Therefore, this
     * function needs to allocate required memory and initializes member
     * variables. Then it initializes the card. Using this function along with
     * clear() allows us to play multiple games with the same set of drawn
     * balls. This would be a simulation of a physical bingo game when multiple
     * players are playing together and at the same time if the same parameters
     * as the constructor parameters are passed to this function. It also is
     * possible to create a card with a new set of parameters.
     *
     * The parameters min and max specify the range of numbers that can be
     * placed on a card. They also indicate the smallest and the largest ball
     * numbers that can be drawn.
     *
     * There are certain requirements for the parameters that are passed to
     * this function. If any of the requirements is not satisfied the function
     * does not modify the current object. In such a case the result would be
     * an empty object since the function clear() was called first. For the
     * requirements of these parameters one may refer to the section "The Game
     * Rules" on this page.
     *
     * @param rows    the number of rows of the new card
     * @param columns the number of columns of the new card
     * @param min     the smallest value that can appear on the new card
     * @param max     the largest value that can appear on the new card
     * @return true if the card was re-created successfully, false otherwise
     */
    bool reCreateCard(
        int rows,
        int columns,
        int min,
        int max
    );

    /**
     * @brief Populates the card cells with random numbers.
     *
     * Moreover, it initializes other member variables that are used as
     * helpers. Dumping the object after calling this function will render a
     * randomly created card.
     *
     * There may be some error handling required in this function. For example,
     * before using memory we need to make sure whether required memory has
     * been allocated.
     *
     * @return true if the init operation has been successful, false otherwise
     */
    bool initCard();

    /**
     * @brief Plays a game using the given list of drawn balls.
     *
     * This function gets ball numbers from the vector rndBalls. If the ball
     * number exists in the card this function replaces the corresponding cell
     * in the card with an empty cell, i.e. EMPTYCELL. The function counts the
     * number of replacements and at the end returns it. Among multiple players
     * who are playing with the same rndBalls vector, the player who gets the
     * lower count is the winner.
     *
     * The game stops if the number of drawing is reached. If the number of
     * drawing is equal to the max ball number the game stops as soon as a row
     * or a column wins. If the number of drawing is greater than max ball
     * number or the size of ball vector is greater than the max ball number
     * the game does not perform any drawing. Such a case would be an error
     * case.
     *
     * @param numDraws the number of drawings allowed in the current play
     * @param rndBalls the list of ball numbers to draw from
     * @return the number of cells that were replaced with an empty cell
     */
    int play(
        int         numDraws,
        vector<int> rndBalls
    );

    /**
     * @brief Generates a list of randomly distributed ball numbers.
     *
     * There is no duplicate numbers in the list. One may use the RANDOM type
     * SHUFFLE from the Random class for generating the list. The ball numbers
     * are within the range of m_minBallVal and m_maxBallVal.
     *
     * @return a vector of shuffled ball numbers without duplicates
     */
    vector<int> drawBalls();

    /**
     * @brief Assignment operator. Creates a deep copy of rhs.
     *
     * A deep copy means the current object has the same information as the rhs
     * object, however, it has its own memory allocated. The operator is
     * protected against self-assignment.
     *
     * @param rhs the object to copy from
     * @return a reference to the current object after the copy
     */
    const Bingo &operator=(const Bingo &rhs);

    /**
     * @brief Renders the card on the terminal for debugging purposes.
     *
     * The implementation of this function is provided and does not need to be
     * modified. Do not call this function in the submitted test program.
     *
     * @return void
     */
    void dumpCard();

private:
    /* @brief use this to keep track & check if a col is complete
     * arrays which can be used for keeping track of the number of empty cells
     * in a row or in a column. An empty cell indicates that a ball with the
     * same number has been drawn. After multiple ball drawing once there is a
     * complete empty row or a complete empty column the game is finished */
    int *m_trackCols;

    /* @brief use this to keep track & check if a row is complete
     * arrays which can be used for keeping track of the number of empty cells
     * in a row or in a column. An empty cell indicates that a ball with the
     * same number has been drawn. After multiple ball drawing once there is a
     * complete empty row or a complete empty column the game is finished */
    int *m_trackRows;

    /* @brief The array to store cells in which index numbers are the cell
     * values is an array that stores the information about all cells in the
     * card. In fact we are storing every cell twice in two different data
     * structure. We do this for efficiency (running time). Everytime a ball is
     * drawn we look into m_helper to check whether the ball number exists in
     * the card. The index numbers of this data structures are the ball numbers.
     * This allows us to directly access a cell's information, e.g. if the drawn
     * ball exists in the card what are the row and column number. */
    Cell *m_helper;
    int   m_helperSize;

    /* @brief the 2d structure to store card information
     * 2D array which stores the information about every cell. Every cell
     * is of the type Cell. The implementaion of the Cell class is provided.
     * This member variable represents a bingo card in the game. The sizes of
     * this data structure are specified by the member variables m_numRows and
     * m_numCols. */
    Cell **m_card;

    // the number of rows in the card
    int m_numRows;
    // the number of columns in the card
    int m_numCols;
    // min value in a cell
    int m_minBallVal;
    // max value in a cell
    int m_maxBallVal;

    /******************************************
     * Private function declarations go here! *
     ******************************************/

    /**
     * @brief Reports whether a row or a column of the card is complete.
     *
     * A row or a column is complete when all of its cells are empty, i.e.
     * every ball number on it has been drawn.
     *
     * @return true if the card has a complete empty row or column,
     *         false otherwise or if the object is empty
     */
    bool checkBingo();

    /**
     * @brief Empties the cell holding ballNum, if the card holds it.
     *
     * The ball number is looked up directly in m_helper. If the card holds
     * the ball and the cell has not been emptied already, the cell is set to
     * EMPTYCELL and the row and column counters are updated.
     *
     * @param ballNum the drawn ball number
     * @return true if a cell was emptied, false otherwise
     */
    bool updateCard(int ballNum);

    /**
     * @brief Reports whether the given parameters describe a legal card.
     *
     * The card needs at least one cell, ball numbers cannot be negative
     * because m_helper is indexed by them, the range needs a smaller min
     * than max, the range must hold at least as many values as the card has
     * cells since a value may not repeat on the card, and the range may not
     * contain EMPTYCELL since that value marks a cell as already drawn.
     *
     * @param rows    the number of rows of the card
     * @param columns the number of columns of the card
     * @param min     the smallest value that can appear on the card
     * @param max     the largest value that can appear on the card
     * @return true if all requirements are satisfied, false otherwise
     *
     * @note static because it reads no member variable, which lets the
     *       constructors call it before the members are initialized
     */
    static bool validParams(
        int rows,
        int columns,
        int min,
        int max
    );

    /**
     * @brief Frees every array held by the object and nulls the pointers.
     *
     * Unlike clear() this leaves the size and range members untouched, so it
     * can be used to release the old memory before allocating new memory for
     * the same configuration.
     *
     * @return void
     */
    void deallocate();
};

// The following class is an auxiliary class to facilitate the random number
// generation The UNIFORMINT type generates random integer numbers in a range
//      if the range is small there might be duplicate numbers
// The UNIFORMREAL type generates random floating point numbers in a range
// The NORMAL type generates a set of random numbers with normal distribution
// The SHUFFLE type generates a list of randomly distributed integer numbers
// without duplicates
enum RANDOM {
    UNIFORMINT,
    UNIFORMREAL,
    NORMAL,
    SHUFFLE
};

/**
 * @brief An auxiliary class to facilitate random number generation.
 *
 * The behavior of the object is fixed at construction time by the RANDOM type
 * that is passed to the constructor.
 */
class Random {
public:
    /**
     * @brief Constructs a random number generator of the given type.
     *
     * The UNIFORMINT and UNIFORMREAL types use a fixed seed so the same
     * sequence repeats, which helps with testing. The NORMAL and SHUFFLE types
     * are seeded from a random device.
     *
     * @param min   the smallest number that can be generated
     * @param max   the largest number that can be generated
     * @param type  the kind of distribution to generate numbers from
     * @param mean  the mean of the data set, used by the NORMAL type only
     * @param stdev the standard deviation, used by the NORMAL type only
     */
    Random(
        int    min,
        int    max,
        RANDOM type  = UNIFORMINT,
        int    mean  = 50,
        int    stdev = 20
    )
        : m_min(min),
          m_max(max),
          m_type(type) {
        if (type == NORMAL) {
            // the case of NORMAL to generate integer numbers with normal
            // distribution
            m_generator = std::mt19937(m_device());
            // the data set will have the mean of 50 (default) and standard
            // deviation of 20 (default) the mean and standard deviation can
            // change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean, stdev);
        } else if (type == UNIFORMINT) {
            // the case of UNIFORMINT to generate integer numbers
            //  Using a fixed seed value generates always the same sequence
            //  of pseudorandom numbers, e.g. reproducing scientific experiments
            //  here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10); // 10 is the fixed seed value
            m_unidist   = std::uniform_int_distribution<>(min, max);
        } else if (type == UNIFORMREAL) { // the case of UNIFORMREAL to generate
                                          // real numbers
            m_generator = std::mt19937(10); // 10 is the fixed seed value
            m_uniReal   = std::uniform_real_distribution<double>(
                (double)min,
                (double)max
            );
        } else { // the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }

    /**
     * @brief Re-seeds the generator to change the sequence it produces.
     *
     * We have set a default value for the seed in the constructor. Calling
     * this function after the constructor gives us more randomness.
     *
     * @param seedNum the new seed value for the generator
     * @return void
     */
    void setSeed(int seedNum) {
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor
        // call this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    /**
     * @brief Fills a vector with a shuffled list of the numbers in the range.
     *
     * The user program creates the vector param and passes it here. Every
     * number between m_min and m_max appears exactly once.
     *
     * @param array the vector to populate with the shuffled numbers
     * @return void
     */
    void getShuffle(vector<int> &array) {
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i <= m_max; i++) {
            array.push_back(i);
        }
        shuffle(array.begin(), array.end(), m_generator);
    }

    /**
     * @brief Fills an array with a shuffled list of the numbers in the range.
     *
     * The user program creates the array and passes it here. The param array
     * must be of the size (m_max - m_min + 1).
     *
     * @param array the array to populate with the shuffled numbers
     * @return void
     */
    void getShuffle(int array[]) {
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i <= m_max; i++) {
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int                   i = 0;
        for (it = temp.begin(); it != temp.end(); it++) {
            array[i] = *it;
            i++;
        }
    }

    /**
     * @brief Returns a random integer number.
     *
     * The object must have been initialized to generate integers, i.e. with
     * the NORMAL or UNIFORMINT type. The returned number is limited by the min
     * and max values.
     *
     * @return a random integer in the range [m_min, m_max], 0 if the object
     *         was not initialized to generate integers
     */
    int getRandNum() {
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if (m_type == NORMAL) {
            // returns a random number in a set with normal distribution
            // we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max) {
                result = m_normdist(m_generator);
            }
        } else if (m_type == UNIFORMINT) {
            // this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    /**
     * @brief Returns a random real number truncated to two decimal points.
     *
     * The object must have been initialized to generate real numbers, i.e.
     * with the UNIFORMREAL type. For example if the generated value is
     * 15.0378, the function returns 15.03.
     *
     * @return a random real number in the range [m_min, m_max]
     */
    double getRealRandNum() {
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

private:
    int                             m_min;
    int                             m_max;
    RANDOM                          m_type;
    std::random_device              m_device;
    std::mt19937                    m_generator;
    std::normal_distribution<>      m_normdist; // normal distribution
    std::uniform_int_distribution<> m_unidist;  // integer uniform distribution
    std::uniform_real_distribution<double>
        m_uniReal; // real uniform distribution
};
#endif
