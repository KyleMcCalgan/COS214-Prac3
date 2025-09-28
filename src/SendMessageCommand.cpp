/**
 * @file SendMessageCommand.cpp
 * @brief Implementation of SendMessageCommand class with Logger
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "SendMessageCommand.h"
#include "ChatRoom.h"
#include "Users.h"
#include "Logger.h"
#include <iostream>

SendMessageCommand::SendMessageCommand(ChatRoom* room, User* user, std::string msg)
    : Command(room, user, msg) {
    
    Logger::debug("[SendMessageCommand] Created for user: " + user->getName());
}

void SendMessageCommand::execute() {
    Logger::debug("[SendMessageCommand] Executing - sending message to all users");
   
    chatRoom->sendMessage(message, fromUser);
    
    Logger::debug("[SendMessageCommand] Message delivery completed");
}