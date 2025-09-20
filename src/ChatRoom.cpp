/**
 * @file ChatRoom.cpp
 * @brief Implementation of ChatRoom class
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "ChatRoom.h"
#include "Users.h"

#include <iostream>
#include <algorithm>
#include <vector>

void ChatRoom::sendMessage(std::string message, User* fromUser) {
    // Send message to all users

    //can take this out later just for testing 
    std::cout << "\n[ChatRoom] Broadcasting message from " << fromUser->getName() << std::endl;

     for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {

        if (*it != fromUser) {
            (*it)->receive(message, fromUser, this);
        }
     }
}

void ChatRoom::saveMessage(std::string message, User* fromUser) {
    // Format: "UserName: message"
    std::string formattedMessage = fromUser->getName() + ": " + message;
    chatHistory.push_back(formattedMessage);

     std::cout << "[ChatRoom] Message saved to history: " << formattedMessage << std::endl;
}

