/**
 * @file CtrlCat.cpp
 * @brief Implementation of CtrlCat concrete mediator with clean logging
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "CtrlCat.h"
#include "Users.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>
#include <vector>

void CtrlCat::registerUser(User* user) {
    // Check if user already exists using iterator
    std::vector<User*>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            Logger::info(user->getName() + " is already in CtrlCat room");
            return;
        }
    }
    
    users.push_back(user);
    user->addChatRoom(this);
    
    // Clean user experience
    Logger::info(user->getName() + " joined CtrlCat");
    
    // Debug info
    Logger::debug("[CtrlCat] User " + user->getName() + " registered with mediator");
}

void CtrlCat::removeUser(User* user) {
    std::vector<User*>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            users.erase(it);
            Logger::info(user->getName() + " left CtrlCat");
            Logger::debug("[CtrlCat] User removed from mediator");
            return;
        }
    }
    
    Logger::debug("[CtrlCat] User " + user->getName() + " was not in this room");
}