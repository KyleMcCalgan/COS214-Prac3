/**
 * @file Iterator.h
 * @brief Iterator interface for iterating through collections
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef ITERATOR_H
#define ITERATOR_H

#include <string>

/**
 * @brief Abstract Iterator interface
 * Defines the interface for iterating through a collection
 */
class Iterator {
public:
    virtual ~Iterator() = default;
    
    /**
     * @brief Move to the first element
     */
    virtual void first() = 0;
    
    /**
     * @brief Move to the next element
     */
    virtual void next() = 0;
    
    /**
     * @brief Check if iteration is complete
     * @return true if there are no more elements
     */
    virtual bool isDone() const = 0;
    
    /**
     * @brief Get the current element
     * @return current item
     */
    virtual std::string currentItem() const = 0;
};

#endif // ITERATOR_H