/**
 * @file SendMessageCommand.cpp
 * @brief Implementation of SendMessageCommand class
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "SendMessageCommand.h"
#include "ChatRoom.h"
#include "Users.h"
#include <iostream>

SendMessageCommand::SendMessageCommand(ChatRoom* room, User* user, std::string msg)
    : Command(room, user, msg) {
    
    std::cout << "[SendMessageCommand] Created for user: " << user->getName() << std::endl;
}

void SendMessageCommand::execute() {
    
    std::cout << "[SendMessageCommand] Executing - sending message to all users" << std::endl;
   
    chatRoom->sendMessage(message, fromUser);
    
    std::cout << "[SendMessageCommand] Message delivery completed" << std::endl;
}