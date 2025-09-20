/**
 * @file SaveMessageCommand.cpp
 * @brief Implementation of SaveMessageCommand class
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "SaveMessageCommand.h"
#include "ChatRoom.h"
#include "Users.h"
#include <iostream>

SaveMessageCommand::SaveMessageCommand(ChatRoom* room, User* user, std::string msg)
    : Command(room, user, msg) {
    
    std::cout << "[SaveMessageCommand] Created for message: \"" << msg << "\"" << std::endl;
}

void SaveMessageCommand::execute() {

    std::cout << "[SaveMessageCommand] Executing - saving message to history" << std::endl;
    
    chatRoom->saveMessage(message, fromUser);
    
    std::cout << "[SaveMessageCommand] Message saved to history" << std::endl;
}