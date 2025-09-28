/**
 * @file Command.cpp
 * @brief Implementation of Command base class with Logger
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "Command.h"
#include "Logger.h"
#include <iostream>

Command::Command(ChatRoom* room, User* user, std::string msg) 
    : chatRoom(room), fromUser(user), message(msg) {
    
    Logger::debug("[Command] Command created with message: \"" + msg + "\"");
}