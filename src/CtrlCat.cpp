/**
 * @file CtrlCat.cpp
 * @brief Implementation of CtrlCat concrete mediator
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "CtrlCat.h"
#include "Users.h"
#include <algorithm>
#include <iostream>
#include <vector>

void CtrlCat::registerUser(User* user) {
    // MEDIATOR PATTERN BEHAVIOR:

    // Check if user already exists using iterator
    std::vector<User*>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            std::cout << "[CtrlCat] User " << user->getName() << " already registered!" << std::endl;
            return;
        }
    }
    
    users.push_back(user);
    user->addChatRoom(this);
    std::cout << "[CtrlCat] User " << user->getName() << " joined the cat room" << std::endl;
}



void CtrlCat::removeUser(User* user) {
    // MEDIATOR PATTERN: Remove user from this mediator's user list
    std::vector<User*>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            users.erase(it);
            std::cout << "[CtrlCat] User " << user->getName() << " left the cat room." << std::endl;
            return;
        }
    }
    
    std::cout << "[CtrlCat] User " << user->getName() << " was not in this room." << std::endl;
}