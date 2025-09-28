/**
 * @file SaveMessageCommand.cpp
 * @brief Implementation of SaveMessageCommand class with Logger
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "SaveMessageCommand.h"
#include "ChatRoom.h"
#include "Users.h"
#include "Logger.h"
#include <iostream>

SaveMessageCommand::SaveMessageCommand(ChatRoom* room, User* user, std::string msg)
    : Command(room, user, msg) {
    
    Logger::debug("[SaveMessageCommand] Created for message: \"" + msg + "\"");
}

void SaveMessageCommand::execute() {
    Logger::debug("[SaveMessageCommand] Executing - saving message to history");
    
    chatRoom->saveMessage(message, fromUser);
    
    Logger::debug("[SaveMessageCommand] Message saved to history");
}