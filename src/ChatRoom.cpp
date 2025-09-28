/**
 * @file ChatRoom.cpp
 * @brief Implementation of ChatRoom class with Iterator pattern support
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include "ChatRoom.h"
#include "Users.h"
#include "ConcreteAggregate.h"
#include "ConcreteIterator.h"

#include <iostream>
#include <algorithm>
#include <vector>

void ChatRoom::sendMessage(std::string message, User* fromUser) {
    // Send message to all users

    //can take this out later just for testing 
    std::cout << "\n[ChatRoom] Broadcasting message from " << fromUser->getName() << std::endl;

     for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {

        if (*it != fromUser) {
            (*it)->receive(message, fromUser, this);
        }
     }
}

void ChatRoom::saveMessage(std::string message, User* fromUser) {
    // Format: "UserName: message"
    std::string formattedMessage = fromUser->getName() + ": " + message;
    chatHistory.push_back(formattedMessage);

     std::cout << "[ChatRoom] Message saved to history: " << formattedMessage << std::endl;
}

const std::vector<std::string>* ChatRoom::getChatHistory(User* requestingUser) const {
    // Check if requesting user is admin
    if (requestingUser && requestingUser->getUserType() == UserType::ADMIN) {
        std::cout << "[ChatRoom] Admin " << requestingUser->getName() 
                  << " granted access to chat history (" << chatHistory.size() << " messages)" << std::endl;
        return &chatHistory;
    } else {
        std::cout << "[ChatRoom] Access denied - only admins can access chat history" << std::endl;
        if (requestingUser) {
            std::cout << "[ChatRoom] User " << requestingUser->getName() 
                      << " (" << requestingUser->getUserTypeString() << ") lacks admin privileges" << std::endl;
        }
        return nullptr;
    }
}

Iterator* ChatRoom::createIterator(User* requestingUser) {
    // Check if requesting user is admin
    if (requestingUser && requestingUser->getUserType() == UserType::ADMIN) {
        std::cout << "[ChatRoom] Creating iterator for admin " << requestingUser->getName() << std::endl;
        return new ConcreteIterator(&chatHistory);
    } else {
        std::cout << "[ChatRoom] Iterator access denied - only admins can iterate chat history" << std::endl;
        if (requestingUser) {
            std::cout << "[ChatRoom] User " << requestingUser->getName() 
                      << " (" << requestingUser->getUserTypeString() << ") lacks admin privileges" << std::endl;
        }
        return nullptr;
    }
}

Iterator* ChatRoom::createIterator() {
    // Base implementation for Aggregate interface
    std::cout << "[ChatRoom] Creating unrestricted iterator (base Aggregate method)" << std::endl;
    return new ConcreteIterator(&chatHistory);
}