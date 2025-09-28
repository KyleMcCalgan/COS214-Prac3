/**
 * @file ConcreteAggregate.cpp
 * @brief Implementation of ConcreteAggregate class
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include "ConcreteAggregate.h"
#include "ConcreteIterator.h"
#include <iostream>

ConcreteAggregate::ConcreteAggregate(const std::vector<std::string>* history) 
    : chatHistory(history) {
    
    std::cout << "[ConcreteAggregate] Created with chat history containing " 
              << (history ? history->size() : 0) << " messages" << std::endl;
}

Iterator* ConcreteAggregate::createIterator() {
    std::cout << "[ConcreteAggregate] Creating iterator for chat history" << std::endl;
    
    return new ConcreteIterator(chatHistory);
}