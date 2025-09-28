/**
 * @file ConcreteIterator.h
 * @brief Concrete iterator for chat history messages
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#ifndef CONCRETE_ITERATOR_H
#define CONCRETE_ITERATOR_H

#include "Iterator.h"
#include <vector>
#include <string>

/**
 * @brief Concrete iterator implementation for chat history
 * Iterates through a vector of chat history messages
 */
class ConcreteIterator : public Iterator {
private:
    const std::vector<std::string>* chatHistory;  // Reference to chat history
    int currentIndex;                             // Current position in iteration
    
public:
    /**
     * @brief Constructor
     * @param history Pointer to the chat history vector
     */
    ConcreteIterator(const std::vector<std::string>* history);
    
    /**
     * @brief Destructor
     */
    virtual ~ConcreteIterator() = default;
    
    /**
     * @brief Move to the first element
     */
    virtual void first() override;
    
    /**
     * @brief Move to the next element
     */
    virtual void next() override;
    
    /**
     * @brief Check if iteration is complete
     * @return true if there are no more elements
     */
    virtual bool isDone() const override;
    
    /**
     * @brief Get the current element
     * @return current chat history message
     */
    virtual std::string currentItem() const override;
};

#endif // CONCRETE_ITERATOR_H