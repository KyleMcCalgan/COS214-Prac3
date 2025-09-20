/**
 * @file Command.cpp
 * @brief Implementation of Command base class
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "Command.h"
#include <iostream>

Command::Command(ChatRoom* room, User* user, std::string msg) 
    : chatRoom(room), fromUser(user), message(msg) {
    
    //Store all parameters needed for later execution
    std::cout << "[Command] Command created with message: \"" << msg << "\"" << std::endl;
}