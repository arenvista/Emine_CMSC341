// CMSC 341 - Fall 2026 - Project 4
/**
 * @file dealer.h
 * @brief Declarations of Car, one delivery record, and CarDB, a hash table of
 *        Car records with selectable probing and incremental rehashing.
 */
#ifndef DEALER_H
#define DEALER_H
#include "math.h"
#include <iostream>
#include <string>
using namespace std;
class Grader;
class Tester;
class Car;
class CarDB;
const int MINID = 100000; // dealer ID
const int MAXID = 999999; // dealer ID
const int MINQUANTITY = 1;
const int MAXQUANTITY = 10;
const int MINPRIME = 101;   // Min size for hash table
const int MAXPRIME = 99991; // Max size for hash table
#define EMPTY Car("", 0, 0, false)
#define DELETED Car("DELETED")
#define DELETEDKEY "DELETED"
typedef unsigned int (*hash_fn)(string); // declaration of hash function
enum prob_t {
    QUADRATIC,
    DOUBLEHASH,
    LINEAR
}; // types of collision handling policy
#define DEFPOLCY QUADRATIC

/**
 * @brief One delivery record: a quantity of one car model sent to one dealer.
 *
 * The model is the hash key. The model and the dealer together identify a
 * record (see operator==). m_used is the lazy-deletion flag that CarDB reads.
 */
class Car {
    friend class Tester;
    friend class Grader;
    friend class CarDB;

  public:
    /**
     * @brief Construct a record. With no arguments, this is the empty record.
     * @param model    car model; the hash key
     * @param quantity number of cars delivered
     * @param dealer   dealer ID; together with @p model it identifies the
     * record
     * @param used     true for a live record; false for an empty or deleted one
     */
    Car(string model = "", int quantity = 0, int dealer = 0,
        bool used = false) {
        m_model = model;
        m_quantity = quantity;
        m_dealer = dealer;
        m_used = used;
    }
    /// @brief Set the car model (the hash key).
    void setModel(string model) { m_model = model; }
    /// @brief Set the number of cars delivered.
    void setQuantity(int quantity) { m_quantity = quantity; }
    /// @brief Set the dealer ID.
    void setDealer(int dealer) { m_dealer = dealer; }
    /// @brief Set the lazy-deletion flag (true = live, false = free for reuse).
    void setUsed(bool used) { m_used = used; }
    /// @return the car model (the hash key)
    string getModel() const { return m_model; }
    /// @return the number of cars delivered
    int getQuantity() const { return m_quantity; }
    /// @return the dealer ID
    int getDealer() const { return m_dealer; }
    /// @return true if the record is live, false if empty or deleted
    bool getUsed() const { return m_used; }
    /**
     * @brief Copy every field of @p rhs, including the m_used flag.
     * @param rhs record to copy
     * @return this record
     */
    const Car &operator=(const Car &rhs) {
        if (this != &rhs) {
            m_model = rhs.m_model;
            m_quantity = rhs.m_quantity;
            m_dealer = rhs.m_dealer;
            m_used = rhs.m_used;
        }
        return *this;
    }
    /**
     * @brief Print a record as `model (dealer, used)`. A null pointer or an
     *        empty model prints nothing.
     * @param sout output stream
     * @param car  record to print; may be nullptr
     * @return @p sout
     */
    friend ostream &operator<<(ostream &sout, const Car *car) {
        if ((car != nullptr) && !(car->getModel().empty()))
            sout << car->getModel() << " (" << car->getDealer() << ", "
                 << car->getUsed() << ")";
        else
            sout << "";
        return sout;
    }
    /**
     * @brief Identity comparison: equal when model and dealer match. Quantity
     *        and m_used are ignored.
     * @param lhs first record
     * @param rhs second record
     * @return true if both records have the same model and dealer
     */
    friend bool operator==(const Car &lhs, const Car &rhs) {
        // since the uniqueness of an object is defined by key and ID
        // the equality operator considers only those two criteria
        return ((lhs.getModel() == rhs.getModel()) &&
                (lhs.getDealer() == rhs.getDealer()));
    }
    /**
     * @brief Identity comparison against a pointed-to record, by model and
     *        dealer.
     * @param rhs record to compare with; must not be nullptr
     * @return true if both records have the same model and dealer
     */
    bool operator==(const Car *&rhs) {
        // since the uniqueness of an object is defined by sequence and ID
        // the equality operator considers only those two criteria
        return ((getModel() == rhs->getModel()) &&
                (getDealer() == rhs->getDealer()));
    }

  private:
    string m_model; // car type, the search string used as key in the hash table
    int m_quantity; // number of card delivered to the dealer
    int m_dealer; // dealer information, car model and dealer together determine
                  // uniqueness of a Car object
    // the following variable is used for lazy delete scheme in hash table
    // if it is set to false, it means the bucket in the hash table is free for
    // insert if it is set to true, it means the bucket contains live data, and
    // we cannot overwrite it
    bool m_used;
};

/**
 * @brief Hash table of Car records keyed on the model, with lazy deletion and
 *        incremental rehashing.
 *
 * Inserts always go into the current table. A rehash creates a new table and
 * turns the current table into the old table. From then on, each insert or
 * remove moves the live records of the next quarter of the old table into the
 * current table, until the old table is empty and is freed. Each table keeps
 * the probing policy it was created with.
 */
class CarDB {
  public:
    friend class Grader;
    friend class Tester;
    /**
     * @brief Create an empty database with one table.
     * @param size    requested capacity. Values below MINPRIME or above
     *                MAXPRIME are clamped to that bound; a non-prime value is
     *                raised to the next prime.
     * @param hash    hash function applied to Car::m_model
     * @param probing collision policy of the first table. The default,
     *                DEFPOLCY, is given on the definition in dealer.cpp, so
     * code that includes only this header must pass it.
     */
    CarDB(int size, hash_fn hash, prob_t probing);
    /// @brief Free every Car and both tables, including an old table that is
    ///        still being drained.
    ~CarDB();
    /**
     * @brief Load factor of the current table.
     * @return occupied buckets (live and deleted) divided by the capacity
     */
    float lambda() const;
    /**
     * @brief Fraction of the current table's occupied buckets that are deleted.
     * @return m_currNumDeleted / m_currentSize, or 0 if the table is empty
     */
    float deletedRatio() const;
    /**
     * @brief Insert a copy of @p car into the current table.
     *
     * Probing starts at m_hash(model) % m_currentCap and follows
     * m_currProbing; a deleted bucket may be reused. Afterwards, a rehash in
     * progress moves its next quarter. If none is in progress, a rehash
     * starts when the load factor exceeds 0.5.
     *
     * @param car record to insert
     * @return true if inserted; false if the dealer ID is outside
     *         [MINID, MAXID] or an equal record already exists in either table
     */
    bool insert(Car car);
    /**
     * @brief Lazily delete the record equal to @p car from whichever table
     *        holds it.
     *
     * The bucket is tagged as deleted, not emptied, so later searches probe
     * past it. Afterwards, a rehash in progress moves its next quarter. If
     * none is in progress, a rehash starts when deletedRatio() exceeds 0.8.
     *
     * @param car record to remove, matched on model and dealer
     * @return true if a live equal record was found and deleted
     */
    bool remove(Car car);
    /**
     * @brief Find the live record with this model and dealer in either table.
     * @param model  car model (the hash key)
     * @param dealer dealer ID
     * @return a copy of the record, or an empty Car if there is no live match
     */
    const Car getCar(string model, int dealer) const;
    /**
     * @brief Change the dealer ID of the live record equal to @p car.
     *
     * Only the model is hashed, so the record stays in its bucket.
     *
     * @param car    record to update, matched on model and dealer
     * @param dealer new dealer ID
     * @return true if the record was found and updated
     */
    bool updateDealer(Car car, int dealer);
    /**
     * @brief Request the collision policy for the next table a rehash creates.
     *
     * Existing tables keep their policies.
     *
     * @param policy policy for tables created from the next rehash on
     */
    void changeProbPolicy(prob_t policy);
    /// @brief Print both tables in index order. For debugging only; never
    ///        call it from a test.
    void dump() const;

  private:
    hash_fn m_hash;     // hash function
    prob_t m_newPolicy; // stores the change of policy request

    Car **m_currentTable; // hash table
    int m_currentCap;     // hash table size (capacity)
    int m_currentSize;    // current number of entries
                          // m_currentSize includes deleted entries
    int m_currNumDeleted; // number of deleted entries
    prob_t m_currProbing; // collision handling policy

    Car **m_oldTable;    // hash table
    int m_oldCap;        // hash table size (capacity)
    int m_oldSize;       // current number of entries
                         // m_oldSize includes deleted entries
    int m_oldNumDeleted; // number of deleted entries
    prob_t m_oldProbing; // collision handling policy

    int m_transferIndex; // this can be used as a temporary place holder
                         // during incremental transfer to scanning the table

    /**
     * @brief Trial-division primality test.
     * @param number value to test
     * @return true if no integer in [2, number / 2] divides @p number
     *         (so also true for 0 and 1)
     */
    bool isPrime(int number);
    /**
     * @brief Smallest prime greater than @p current, kept within
     *        [MINPRIME, MAXPRIME].
     * @param current exclusive lower bound
     * @return MINPRIME if @p current < MINPRIME; MAXPRIME if no prime between
     *         @p current and MAXPRIME exceeds it; otherwise the next prime
     *         after @p current
     */
    int findNextPrime(int current);

    /******************************************
     * Private function declarations go here! *
     ******************************************/

    /**
     * @brief Bucket visited by the i-th probe of a table with a given policy.
     *
     * Probe 0 is the home bucket, @p hash % @p cap. The policy is a
     * parameter, so the same helper serves the current table
     * (m_currProbing) and the old table (m_oldProbing). Each call computes
     * probe @p i from the home bucket, never from the previous probe:
     * stepping from the previous bucket with i = 1 turns quadratic probing
     * into linear probing.
     *
     * @param hash   m_hash(model), computed once per search
     * @param i      probe number, starting at 0
     * @param cap    capacity of the table being probed
     * @param policy that table's collision policy
     * @return an index in [0, cap)
     */
    int probeIndex(unsigned int hash, int i, int cap, prob_t policy) const;
    /**
     * @brief Find the live record with this model and dealer in one table.
     *
     * Probes from the home bucket with @p policy, past deleted buckets, and
     * stops at a nullptr bucket or after @p cap probes.
     *
     * @param table  table to search; may be nullptr (no old table)
     * @param cap    its capacity
     * @param policy its collision policy
     * @param model  car model (the hash key)
     * @param dealer dealer ID
     * @return the bucket index, or -1 if there is no live match
     */
    int findIndex(Car **table, int cap, prob_t policy, string model,
                  int dealer) const;
    /**
     * @brief Store a copy of @p car in the first free bucket of the current
     *        table.
     *
     * A bucket is free if it is nullptr or holds a deleted record. Updates
     * m_currentSize or m_currNumDeleted to match. Does no duplicate check and
     * no rehash step, so insert() and transferChunk() can both use it.
     *
     * @param car record to store
     * @return true if stored; false if no free bucket turned up within
     *         m_currentCap probes
     */
    bool placeInCurrent(const Car &car);
    /**
     * @brief Begin an incremental rehash.
     *
     * Allocates a table of capacity
     * findNextPrime(4 * (m_currentSize - m_currNumDeleted)) with policy
     * m_newPolicy. Moves the current table, its counters and its policy into
     * the old-table fields, makes the new table current, resets
     * m_transferIndex, and moves the first quarter with transferChunk().
     *
     * @pre No rehash is in progress (m_oldTable == nullptr).
     */
    void startRehash();
    /**
     * @brief Move the live records in the next quarter of the old table into
     *        the current table.
     *
     * Scans floor(m_oldCap / 4) buckets starting at m_transferIndex; the
     * fourth pass runs to the end, so leftover buckets are included. A moved
     * record's old bucket is tagged deleted, not emptied, so records still
     * waiting in the old table stay findable. When the scan reaches the end,
     * frees the old table with freeTable().
     *
     * @pre A rehash is in progress (m_oldTable != nullptr).
     */
    void transferChunk();
    /**
     * @brief Delete every Car in a table, then the array, and set the pointer
     *        to nullptr.
     * @param table table to free; may already be nullptr
     * @param cap   its capacity
     */
    void freeTable(Car **&table, int cap);
    /**
     * @brief Turn a requested size into a legal capacity.
     * @param size requested capacity
     * @return MINPRIME if @p size < MINPRIME, MAXPRIME if @p size > MAXPRIME,
     *         @p size itself if it is prime, otherwise findNextPrime(size)
     */
    int normalizeCapacity(int size);
};
#endif
