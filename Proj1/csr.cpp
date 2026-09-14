// CMSC 341 - Fall 2026 - Project 1
#include "csr.h"

///////////////CSR Class Implementation///////////////

// Default constructor.
//
// Builds an EMPTY object, which the spec defines as one that holds no
// dynamic memory at all. Every member has to be given a value here --
// leaving m_values/m_col_index/m_row_index uninitialised means the
// destructor later calls delete[] on a garbage address.
//
// Empty means: the three arrays are nullptr, m_nonzeros/m_m/m_n are 0, and
// m_next is nullptr.
//
// Note that "empty" is NOT the same thing as "a matrix that happens to be
// all zeros". A 3x3 all-zero matrix still owns an m_row_index of 4 ints and
// still reports its dimensions; sparseRatio() returns 100 for it but 0 for
// an empty object. Pick the empty() test (below) so that the two stay
// distinguishable.
CSR::CSR() {}

// Destructor.
//
// Releases everything this object owns -- the three arrays -- and nothing
// it does not. m_next is a link to a neighbour in the list, NOT a child:
// the spec says the CSR class never allocates or deallocates through
// m_next, so deleting it here would cascade down the whole list and then
// double-free when CSRList::clear() walks the same nodes.
//
// clear() already does exactly the right work, so this can simply defer to
// it rather than repeating the delete[] calls.
CSR::~CSR() {}

// Copy constructor -- deep copy of rhs.
//
// Deep means three fresh allocations sized from rhs (m_nonzeros for
// m_values and m_col_index, m_m + 1 for m_row_index) with the contents
// copied across, so that destroying either object leaves the other intact.
//
// Two things that are easy to miss:
//   1. m_next must be set to nullptr, not copied. A copy is a standalone
//      matrix; inheriting rhs's link would splice the new object into a
//      list it was never inserted into, and that list's clear() would then
//      free memory this object still points at. The driver does exactly
//      this -- CSR bCSR(aCSR); -- and then inserts bCSR into a different
//      list.
//   2. If rhs is empty there is nothing to allocate; produce an empty
//      object rather than calling new with a zero size and having to
//      remember which arrays are real.
CSR::CSR(const CSR &rhs) {}

// Returns the object to the empty state described at the default
// constructor: deallocate the three arrays, set those pointers to nullptr,
// and zero m_nonzeros, m_m, and m_n.
//
// Setting the pointers to nullptr after delete[] is what makes this safe to
// call twice (and makes the destructor safe after an explicit clear()) --
// delete[] on nullptr is a no-op, delete[] on a dangling pointer is a
// double free.
//
// Does not touch m_next, for the reason given at the destructor: this
// object does not own its neighbour, and CSRList is the only thing allowed
// to relink nodes.
void CSR::clear() {}

// Reports whether the object is in the empty state.
//
// Test a member that is only ever non-trivial when memory is held --
// m_row_index != nullptr is the reliable one, since a valid matrix always
// allocates m_m + 1 entries there. m_values is a poor test: a matrix whose
// members are all zero has m_nonzeros == 0, so m_values is legitimately
// empty/null while the object itself is not.
//
// Everything else keys off this predicate -- dump(), sparseRatio(),
// getAt(), operator== -- so getting the distinction right here is what
// keeps the all-zero-matrix cases in the spec working.
bool CSR::empty() const {}

// Compresses a sparse matrix, given row-major in array, into CSR form.
//
// This is where all allocation for the class happens, and it may be called
// more than once on the same object, so the first step is to release
// whatever is already held (clear()) before allocating again -- otherwise
// the second call leaks the first call's arrays.
//
// Interpreting the arguments (all of this is specified in "Additional
// Requirements"):
//   - m x n is the logical size. array is read row by row: for a 2 x 4
//     matrix, indices 0..3 are row 0 and indices 4..7 are row 1.
//   - arraySize < m * n  -> use what is there, treat the missing tail of
//     the matrix as zeros.
//   - arraySize > m * n  -> use the first m * n entries, ignore the rest.
//   - m == 0 or n == 0   -> an empty object, even if array holds data.
//     Guard against negative m/n here too; they cannot produce a matrix.
//   - no data provided   -> a valid m x n matrix whose members are all
//     zero. Still allocates m_row_index (m + 1 entries, all 0) and still
//     records m_m/m_n; it is not an empty object.
//
// The algorithm itself, with idx(i, j) = i * n + j the index into array and
// a value counted as present only when idx(i, j) < arraySize:
//   1. Count the non-zero members and store the total in m_nonzeros. A
//      counting pass first means the arrays can be allocated at exactly
//      the right size instead of the m * n worst case.
//   2. Allocate m_values[m_nonzeros], m_col_index[m_nonzeros], and
//      m_row_index[m_m + 1].
//   3. Walk the matrix again in the same row-by-row order, keeping a
//      running count of non-zeros seen so far. At the START of row i write
//      that running count into m_row_index[i]; for each non-zero member
//      append its value to m_values and its column j to m_col_index.
//   4. After the last row write the final running count -- which equals
//      m_nonzeros -- into m_row_index[m_m].
//
// The invariant that makes getAt() work is that m_row_index[i] is the
// cumulative number of non-zeros in rows 0..i-1, so m_row_index is
// non-decreasing, starts at 0, and ends at m_nonzeros. Because zeros are
// never stored, the representation is canonical: two equal matrices of the
// same shape always compress to identical arrays, which is what lets
// operator== compare the arrays directly.
void CSR::compress(
    int m,
    int n,
    int array[],
    int arraySize
) {}

// Returns the matrix member at (row, col).
//
// Out-of-range access is an error, not a zero: throw runtime_error when
// row is outside [0, m_m) or col is outside [0, m_n). An empty object has
// no valid indices at all, so every call on one throws. Give the exception
// a message the caller can read -- driver.cpp prints e.what().
//
// For an in-range lookup, the row's stored members occupy one contiguous
// slice of m_values:
//     row_start = m_row_index[row]
//     row_end   = m_row_index[row + 1]
// Scan k over [row_start, row_end). If m_col_index[k] == col, the answer is
// m_values[k]. Falling off the end of that slice is the normal outcome for
// a member that was zero -- return 0, do not throw. The distinction is
// between an index that does not exist (error) and one that exists and
// holds zero (a perfectly good answer that simply was not stored).
//
// Worth noting the cost: O(number of non-zeros in that row), not O(1). That
// is the price the format pays for not storing the zeros.
int CSR::getAt(
    int row,
    int col
) const {}

// Overloaded equality -- true when the two objects represent the same
// matrix.
//
// Order the checks cheapest-first:
//   1. Both empty -> equal. One empty, one not -> not equal.
//   2. Dimensions differ (m_m or m_n) -> not equal. Two matrices with the
//      same non-zero members in the same places are still different
//      matrices if their shapes differ, and the same compressed arrays can
//      come from different shapes.
//   3. m_nonzeros differs -> not equal.
//   4. Compare m_values and m_col_index elementwise over m_nonzeros
//      entries, and m_row_index over m_m + 1 entries.
//
// Step 4 is only sound because compress() never stores an explicit zero,
// which makes the compressed form canonical (see compress()). Without that
// guarantee the arrays would have to be decompressed before comparing.
//
// const in both directions -- the parameter and the function -- so it can
// be called on const objects and used inside CSRList::operator==.
bool CSR::operator==(const CSR &rhs) const {}

// Returns the sparsity ratio as a whole-number percentage: the share of
// members that are zero, times 100, truncated to an int.
//
// Two cases the spec calls out explicitly:
//   - empty object      -> 0 (there is no matrix; m_m * m_n is 0, so this
//                          guard also avoids dividing by zero)
//   - every member zero -> 100
//
// Do the multiplication before the division:
//     (m_m * m_n - m_nonzeros) * 100 / (m_m * m_n)
// Integer division first would collapse every ratio below 1 to 0 and give
// 0 for every matrix that is not entirely zeros. Truncation toward zero is
// what the spec asks for -- 0.656 becomes 65, not 66.
int CSR::sparseRatio() {}

// Debugging aid, provided complete; not to be modified and not to be used
// as a test result. Prints the three arrays on three lines -- values,
// column indices, then the m_m + 1 row offsets -- which is the format
// driver.txt is written in. Reading it against the worked example in the
// project description is the fastest way to see whether compress() built
// what it should have.
void CSR::dump() {
    cout << endl;
    if (!empty()) {
        for (int i = 0; i < m_nonzeros; i++) {
            cout << m_values[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < m_nonzeros; i++) {
            cout << m_col_index[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < m_m + 1; i++) {
            cout << m_row_index[i] << " ";
        }
    } else {
        cout << "The object is empty!";
    }
    cout << endl;
}

//////////////CSRList Class Implementation///////////////

// Default constructor -- an empty list: m_head is nullptr and m_size is 0.
// No allocation. Both members must be initialised; a garbage m_head is a
// segfault on the first traversal.
CSRList::CSRList() {}

// Copy constructor -- a deep copy of rhs that preserves ORDER.
//
// The obvious loop (walk rhs, insertAtHead each node) produces a reversed
// list, which the spec explicitly rejects. Two ways round it:
//   - keep a tail pointer: copy rhs's head into m_head, then for each
//     further node allocate a copy and append it after the tail, advancing
//     the tail;
//   - or build the copy back-to-front by recursing to the end of rhs first.
//
// Either way, each node is a NEW CSR built with the CSR copy constructor,
// so the two lists share no arrays and no nodes; only the sequence of
// values is shared. Set m_size from rhs, and make sure the final node's
// m_next is nullptr so traversals terminate.
//
// An empty rhs must yield an empty list, not a list containing one
// default node.
CSRList::CSRList(const CSRList &rhs) {}

// Destructor -- releases every node. clear() already walks the list and
// deletes each node, so defer to it.
CSRList::~CSRList() {}

// True when the list holds no nodes. m_head == nullptr is the direct test;
// m_size == 0 should agree with it, and if the two ever disagree that is a
// sign a link or a counter update was missed somewhere.
bool CSRList::empty() const {}

// Inserts matrix at the head of the list.
//
// matrix is a const reference the caller still owns, so the list cannot
// link to it directly -- doing so would leave the list holding a pointer to
// a stack object that dies at the end of the caller's scope, and would let
// two lists share a node. Allocate a new CSR copied from matrix (the CSR
// copy constructor does the deep copy).
//
// Then the standard head insert, in this order:
//     newNode->m_next = m_head;
//     m_head = newNode;
//     m_size++;
// Assigning m_head first would lose the rest of the list. This works
// unchanged on an empty list: m_head is nullptr, so the new node's m_next
// becomes nullptr and it is correctly both head and tail.
//
// CSRList is a friend of CSR, which is what permits touching m_next here.
//
// Consequence worth remembering when reading driver.txt: the most recently
// inserted matrix is index 0. aCSRList inserts aCSR then cCSR, so cCSR
// prints first and aCSR is the one at index 1.
void CSRList::insertAtHead(const CSR &matrix) {}

// Returns the list to its initial empty state.
//
// Walk from m_head, and at each step save the next pointer BEFORE deleting
// the current node -- reading temp->m_next after delete temp is a
// use-after-free. Deleting a node runs ~CSR(), which frees that matrix's
// three arrays; since ~CSR() deliberately leaves m_next alone, the deletion
// does not cascade and each node is freed exactly once.
//
// Finish by setting m_head to nullptr and m_size to 0, so the object is
// reusable and a second clear() (or the destructor afterwards) is harmless.
void CSRList::clear() {}

// Returns the member at (row, col) of the matrix stored at position
// CSRIndex, counting from the head as index 0.
//
// Two distinct error conditions, both signalled with runtime_error:
//   - CSRIndex is negative, or >= m_size, or the list is empty -- the
//     matrix is not in the list. driver.txt shows the wording for this one:
//     "Exception Error: Object is not in the list!"
//   - the matrix exists but (row, col) is out of its range -- that check
//     belongs to CSR::getAt, so simply let its exception propagate rather
//     than duplicating the bounds logic here.
//
// Otherwise step a temporary pointer CSRIndex times from m_head and return
// that node's getAt(row, col). Guard the walk against running off the end
// even after the index check, so a stale m_size cannot turn into a
// nullptr dereference.
//
// const, and CSR::getAt is const too, so the traversal pointer should be a
// pointer to const if the compiler is to accept it cleanly.
int CSRList::getAt(
    int CSRIndex,
    int row,
    int col
) const {}

// Overloaded equality -- true when both lists hold equal matrices in the
// same order.
//
// Compare m_size first as a cheap reject, then walk both lists in lockstep
// comparing the current nodes with CSR::operator==, stopping at the first
// difference. Same contents in a different order must compare false, which
// is why this is a lockstep walk and not a search.
//
// Two empty lists are equal. The loop as written already handles that: both
// pointers start nullptr and the loop body never runs.
bool CSRList::operator==(const CSRList &rhs) const {}

// Overloaded assignment -- makes *this an exact deep copy of rhs.
//
// The three steps, in order:
//   1. Guard against self-assignment (if (this == &rhs) return *this;).
//      Without it, step 2 destroys the very nodes step 3 is about to read.
//   2. clear() -- release what this list already holds, or it leaks.
//   3. Rebuild from rhs exactly as the copy constructor does, preserving
//      order, then copy m_size.
//   4. Return *this so assignments can chain.
//
// The edge case the spec asks to be tested is assigning an empty rhs to a
// populated list: step 2 must still run, and step 3 must leave m_head
// nullptr and m_size 0 rather than doing nothing.
//
// The return type is const CSRList& as declared in csr.h -- it must match.
const CSRList &CSRList::operator=(const CSRList &rhs) {}

// Returns the mean of the sparse ratios of every matrix in the list, again
// as a whole-number percentage.
//
// Guard the empty list first and return 0 -- dividing the sum by m_size
// would be a division by zero.
//
// Otherwise accumulate each node's sparseRatio() while traversing and
// divide the total by m_size. Note that this averages values that have
// already been truncated to integers by CSR::sparseRatio(), so it is not
// identical to (total zeros / total members) across the whole list; the
// spec defines it as the average of the ratios, so average the ratios.
int CSRList::averageSparseRatio() {}

// Debugging aid, provided complete; not to be modified. Walks the list from
// the head and calls CSR::dump() on each node, so the output appears in
// list order -- head first. Comparing this against driver.txt checks
// compress(), insertAtHead(), and the copy paths all at once.
void CSRList::dump() {
    if (!empty()) {
        CSR *temp = m_head;
        while (temp != nullptr) {
            temp->dump();
            temp = temp->m_next;
        }
    } else {
        cout << "Error: List is empty!" << endl;
    }
}
