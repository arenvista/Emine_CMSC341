// CMSC 341 - Fall 26 - Project 4
#include "dealer.h"
#include <stdexcept>
CarDB::CarDB(int size, hash_fn hash, prob_t probing = DEFPOLCY) {
    throw logic_error("CarDB::CarDB is not implemented yet");
}

/**
 * Each bucket is a Car*: delete every non-null one, then the array itself.
 * @note Unlike the other stubs this one does not throw: a destructor is
 *       implicitly noexcept, so a throw would call std::terminate. While the
 *       constructor throws, no CarDB is ever built, so it never runs anyway.
 */
CarDB::~CarDB() {
    if (m_currentTable != nullptr) {
        for (int i = 0; i < m_currentCap; i++) {
            if (m_currentTable[i] != nullptr)
                delete m_currentTable[i];
        }
        delete[] m_currentTable;
        m_currentTable = nullptr;
    }

    // the old table exists only while a rehash is in progress
    if (m_oldTable != nullptr) {
        for (int i = 0; i < m_oldCap; i++) {
            if (m_oldTable[i] != nullptr)
                delete m_oldTable[i];
        }
        delete[] m_oldTable;
        m_oldTable = nullptr;
    }

    m_currentCap = 0;
    m_currentSize = 0;
    m_currNumDeleted = 0;

    m_oldCap = 0;
    m_oldSize = 0;
    m_oldNumDeleted = 0;

    m_transferIndex = 0;
}

/**
 * Stores @p policy in m_newPolicy only; no existing table changes.
 * @throws logic_error until implemented.
 */
void CarDB::changeProbPolicy(prob_t policy) {
    throw logic_error("CarDB::changeProbPolicy is not implemented yet");
}

/**
 * @note When choosing a bucket, treat a deleted bucket as free. When checking
 *       for a duplicate, probe past deleted buckets, and also search the old
 *       table if it exists; getCar() does both, and the copy it returns has
 *       m_used set only for a match. Reusing a deleted bucket decrements
 *       m_currNumDeleted rather than incrementing m_currentSize.
 * @throws logic_error until implemented.
 */
bool CarDB::insert(Car car) {
    throw logic_error("CarDB::insert is not implemented yet");
}

/**
 * @note Search the current table with m_currProbing, then the old table with
 *       m_oldProbing. Never set a bucket to nullptr: an empty bucket ends a
 *       search early and can hide records stored further along the probe
 *       chain.
 * @throws logic_error until implemented.
 */
bool CarDB::remove(Car car) {
    throw logic_error("CarDB::remove is not implemented yet");
}

/**
 * @note const, so it never advances an in-progress rehash.
 */
const Car CarDB::getCar(string model, int id) const {
    unsigned int hash = m_hash(model);

    // the current table, with its own capacity and policy
    for (int i = 0; i < m_currentCap; i++) {
        int carIndex = probeIndex(hash, i, m_currentCap, m_currProbing);
        if (m_currentTable[carIndex] == nullptr)
            break; // end of the probe chain
        if (m_currentTable[carIndex]->m_model == model &&
            m_currentTable[carIndex]->m_dealer == id &&
            m_currentTable[carIndex]->m_used) {
            return *m_currentTable[carIndex];
        }
    }

    // then the old table, if a rehash is in progress
    if (m_oldTable != nullptr) {
        for (int i = 0; i < m_oldCap; i++) {
            int carIndex = probeIndex(hash, i, m_oldCap, m_oldProbing);
            if (m_oldTable[carIndex] == nullptr)
                break;
            if (m_oldTable[carIndex]->m_model == model &&
                m_oldTable[carIndex]->m_dealer == id &&
                m_oldTable[carIndex]->m_used) {
                return *m_oldTable[carIndex];
            }
        }
    }
    return EMPTY;
}

/**
 * @note Not an insert or a remove, so it never advances a rehash.
 * @throws logic_error until implemented.
 */
bool CarDB::updateDealer(Car car, int id) {
    throw logic_error("CarDB::updateDealer is not implemented yet");
}

/**
 * Computed as a float division: m_currentSize / m_currentCap.
 */
float CarDB::lambda() const {
    return float(m_currentSize) / float(m_currentCap);
}

/**
 * Computed as a float division: m_currNumDeleted / m_currentSize, or 0 for
 * an empty table rather than dividing by zero.
 */
float CarDB::deletedRatio() const {
    if (m_currentSize == 0)
        return 0;
    return float(m_currNumDeleted) / float(m_currentSize);
}

/** A deleted record still prints its data, with used flag 0. */
void CarDB::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

/** Tests divisors up to number / 2. */
bool CarDB::isPrime(int number) {
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

/**
 * @note The `i != current` test skips @p current itself, so an argument of
 *       exactly MINPRIME returns 103, not MINPRIME.
 */
int CarDB::findNextPrime(int current) {
    // we always stay within the range [MINPRIME-MAXPRIME]
    // the smallest prime starts at MINPRIME
    if (current < MINPRIME)
        current = MINPRIME - 1;
    for (int i = current; i < MAXPRIME; i++) {
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0)
                break;
            else if (j + 1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    // if a user tries to go over MAXPRIME
    return MAXPRIME;
}

/******************************************
 * Suggested private helpers              *
 * Outlines: each stub lists its steps    *
 * and throws until they are coded.       *
 ******************************************/

/**
 * @throws logic_error for an unknown policy.
 */
int CarDB::probeIndex(unsigned int hash, int i, int cap, prob_t policy) const {
    // long long throughout: i * i overflows int once i > 46340
    long long home = hash % cap; // probe 0
    switch (policy) {
    case LINEAR:
        return int((home + i) % cap);
    case QUADRATIC:
        return int((home + (long long)i * i) % cap);
    case DOUBLEHASH:
        return int((home + i * (11LL - hash % 11)) % cap);
    default:
        throw logic_error("CarDB::probeIndex: unknown probing policy");
    }
}

/**
 * @note Can replace the search loops in getCar(): call it once for the
 *       current table and once for the old table, each with that table's
 *       capacity and policy.
 * @throws logic_error until implemented.
 */
int CarDB::findIndex(Car **table, int cap, prob_t policy, string model,
                     int dealer) const {
    throw logic_error("CarDB::findIndex is not implemented yet");
}

/**
 * @note No duplicate check and no rehash step, so insert() and
 *       transferChunk() can both use it.
 * @throws logic_error until implemented.
 */
bool CarDB::placeInCurrent(const Car &car) {
    throw logic_error("CarDB::placeInCurrent is not implemented yet");
}

/**
 * @note Call only when m_oldTable == nullptr (no rehash in progress).
 * @throws logic_error until implemented.
 */
void CarDB::startRehash() {
    throw logic_error("CarDB::startRehash is not implemented yet");
}

/**
 * @note Does nothing when no rehash is in progress (m_oldTable == nullptr).
 *       Needs placeInCurrent() and freeTable(), which throw until written.
 */
void CarDB::transferChunk() {
    if (m_oldTable == nullptr)
        return;

    int quarter = m_oldCap / 4;
    int end = m_transferIndex + quarter;
    // the fourth pass takes the leftover buckets, e.g. bucket 100 of 101
    if (m_oldCap - end < quarter)
        end = m_oldCap;

    for (int i = m_transferIndex; i < end; i++) {
        Car *bucket = m_oldTable[i];
        // nullptr and deleted buckets are skipped: that is how a rehash
        // drops deleted records for good
        if (bucket != nullptr && bucket->m_used) {
            // cannot fail: the new table's capacity is over 4x the live count
            placeInCurrent(*bucket);
            // tag, never empty: probe chains into later quarters run
            // through this bucket
            bucket->m_used = false;
            m_oldNumDeleted++;
        }
    }
    m_transferIndex = end;

    // drained: free the old table and reset its counters
    if (end == m_oldCap) {
        freeTable(m_oldTable, m_oldCap);
        m_oldCap = 0;
        m_oldSize = 0;
        m_oldNumDeleted = 0;
        m_transferIndex = 0;
    }
}

/**
 * @note The destructor can call this once per table instead of repeating the
 *       loop, and transferChunk() can call it when the old table is drained.
 * @throws logic_error until implemented.
 */
void CarDB::freeTable(Car **&table, int cap) {
    // 1. table == nullptr: return.
    // 2. delete every non-null table[i], for i = 0 .. cap - 1.
    // 3. delete[] table, then table = nullptr. table is a reference, so this
    //    clears the member itself (e.g. m_oldTable).
    // The caller resets the capacity and counters.
    throw logic_error("CarDB::freeTable is not implemented yet");
}

/**
 * @note Used by the constructor. A rehash calls findNextPrime() directly.
 * @throws logic_error until implemented.
 */
int CarDB::normalizeCapacity(int size) {
    // 1. size < MINPRIME: return MINPRIME.
    // 2. size > MAXPRIME: return MAXPRIME.
    // 3. isPrime(size): return size. Test this before step 4:
    //    findNextPrime(MINPRIME) returns 103, so a prime size would be bumped.
    // 4. Otherwise: return findNextPrime(size).
    throw logic_error("CarDB::normalizeCapacity is not implemented yet");
}
