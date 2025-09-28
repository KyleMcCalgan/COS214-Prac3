/**
 * @file ConcreteIterator.cpp
 * @brief Implementation of ConcreteIterator class
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include "ConcreteIterator.h"
#include <iostream>

ConcreteIterator::ConcreteIterator(const std::vector<std::string>* history) 
    : chatHistory(history), currentIndex(0) {
    
    std::cout << "[ConcreteIterator] Created for chat history with " 
              << (history ? history->size() : 0) << " messages" << std::endl;
}

void ConcreteIterator::first() {
    currentIndex = 0;
    std::cout << "[ConcreteIterator] Reset to first element" << std::endl;
}

void ConcreteIterator::next() {
    if (!isDone()) {
        currentIndex++;
        std::cout << "[ConcreteIterator] Moved to index " << currentIndex << std::endl;
    } else {
        std::cout << "[ConcreteIterator] Already at end - cannot move next" << std::endl;
    }
}

bool ConcreteIterator::isDone() const {
    if (!chatHistory) {
        return true;  // No history to iterate over
    }
    
    bool done = currentIndex >= static_cast<int>(chatHistory->size());
    
    if (done) {
        std::cout << "[ConcreteIterator] Iteration complete" << std::endl;
    }
    
    return done;
}

std::string ConcreteIterator::currentItem() const {
    if (isDone() || !chatHistory) {
        std::cout << "[ConcreteIterator] No current item available" << std::endl;
        return "";
    }
    
    std::string item = (*chatHistory)[currentIndex];
    std::cout << "[ConcreteIterator] Current item: \"" << item << "\"" << std::endl;
    
    return item;
}