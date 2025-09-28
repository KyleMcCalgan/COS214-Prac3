/**
 * @file ConcreteAggregate.h
 * @brief Concrete aggregate implementation for chat history
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef CONCRETE_AGGREGATE_H
#define CONCRETE_AGGREGATE_H

#include "Aggregate.h"
#include "Iterator.h"
#include <vector>
#include <string>

/**
 * @brief Concrete aggregate implementation for chat history
 * Manages chat history collection and creates iterators for it
 */
class ConcreteAggregate : public Aggregate {
private:
    const std::vector<std::string>* chatHistory;  // Reference to chat history
    
public:
    /**
     * @brief Constructor
     * @param history Pointer to the chat history vector
     */
    ConcreteAggregate(const std::vector<std::string>* history);
    
    /**
     * @brief Destructor
     */
    virtual ~ConcreteAggregate() = default;
    
    /**
     * @brief Create an iterator for the chat history
     * @return Pointer to a new ConcreteIterator instance
     */
    virtual Iterator* createIterator() override;
};

#endif // CONCRETE_AGGREGATE_H