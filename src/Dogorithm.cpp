/**
 * @file Dogorithm.cpp
 * @brief Implementation of Dogorithm concrete mediator
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "Dogorithm.h"
#include "Users.h"
#include <algorithm>
#include <iostream>
#include <vector>

void Dogorithm::registerUser(User* user) {
   
    std::vector<User*>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            std::cout << "[Dogorithm] User " << user->getName() << " already registered" << std::endl;
            return; // User already exists, don't add again
        }
    }
    
    // Add user to this mediator's user list
    users.push_back(user);
    
    // bidirectional relationship
    //Tell user they're in this room  
    user->addChatRoom(this);
    
    std::cout << "[Dogorithm] User " << user->getName() << " joined the dog room" << std::endl;
}

void Dogorithm::removeUser(User* user) {
   
    std::vector<User*>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            users.erase(it);
            std::cout << "[Dogorithm] User " << user->getName() << " left the dog room." << std::endl;
            return;
        }
    }
    
    std::cout << "[Dogorithm] User " << user->getName() << " was not in this room." << std::endl;
}