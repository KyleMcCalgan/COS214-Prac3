/**
 * @file Dogorithm.cpp
 * @brief Implementation of Dogorithm concrete mediator with Logger
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "Dogorithm.h"
#include "Users.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>
#include <vector>

void Dogorithm::registerUser(User* user) {
    std::vector<User*>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            Logger::info(user->getName() + " already in Dogorithm room");
            return;
        }
    }
    
    // Add user to this mediator's user list
    users.push_back(user);

    user->addChatRoom(this);

    Logger::info(user->getName() + " joined Dogorithm");

    Logger::debug("[Dogorithm] User " + user->getName() + " registered with mediator");
}

void Dogorithm::removeUser(User* user) {
    std::vector<User*>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            users.erase(it);
            Logger::info(user->getName() + " left Dogorithm");
            Logger::debug("[Dogorithm] User removed from mediator");
            return;
        }
    }
    
    Logger::debug("[Dogorithm] User " + user->getName() + " was not in this room");
}