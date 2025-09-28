/**
 * @file Aggregate.h
 * @brief Abstract aggregate interface for creating iterators
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef AGGREGATE_H
#define AGGREGATE_H

class Iterator;  // Forward declaration

/**
 * @brief Abstract Aggregate interface
 * Defines the interface for creating iterators
 */
class Aggregate {
public:
    virtual ~Aggregate() = default;
    
    /**
     * @brief Create an iterator for this aggregate
     * @return Pointer to a new iterator instance
     */
    virtual Iterator* createIterator() = 0;
};

#endif // AGGREGATE_H