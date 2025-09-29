/**
 * @file ChatRoom.cpp
 * @brief Implementation of ChatRoom class with fixed message display
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include "ChatRoom.h"
#include "Users.h"
#include "ConcreteAggregate.h"
#include "ConcreteIterator.h"
#include "Logger.h"

#include <iostream>
#include <algorithm>
#include <vector>

void ChatRoom::sendMessage(std::string message, User* fromUser) {
    // Validate that the fromUser is actually in this room
    bool userFound = false;
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if (*it == fromUser) {
            userFound = true;
            break;
        }
    }
    
    if (!userFound) {
        Logger::debug("[ChatRoom] ERROR: User " + fromUser->getName() + " is not registered in this room!");
        return;
    }

    Logger::user(fromUser->getName() + ": " + message);
    Logger::debug("[ChatRoom] Broadcasting message from " + fromUser->getName());

    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if (*it != fromUser) {
            (*it)->receive(message, fromUser, this);
        }
    }
}

void ChatRoom::saveMessage(std::string message, User* fromUser) {
    bool userFound = false;
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if (*it == fromUser) {
            userFound = true;
            break;
        }
    }
    
    if (!userFound) {
        Logger::debug("[ChatRoom] ERROR: Cannot save message - User " + fromUser->getName() + " is not registered in this room!");
        return;
    }

    //Format: "UserName: message"
    std::string formattedMessage = fromUser->getName() + ": " + message;
    chatHistory.push_back(formattedMessage);

    Logger::debug("[ChatRoom] Message saved to history: " + formattedMessage);
}

const std::vector<std::string>* ChatRoom::getChatHistory(User* requestingUser) const {

    if (requestingUser && requestingUser->getUserType() == UserType::ADMIN) {
        Logger::debug("[ChatRoom] Admin " + requestingUser->getName() + " granted access to chat history (" + std::to_string(chatHistory.size()) + " messages)");
        return &chatHistory;
    } else {
        Logger::info("Access denied - only admins can access chat history");
        if (requestingUser) {
            Logger::debug("[ChatRoom] User " + requestingUser->getName() + " (" + requestingUser->getUserTypeString() + ") lacks admin privileges");
        }
        return nullptr;
    }
}

Iterator* ChatRoom::createIterator(User* requestingUser) {

    if (requestingUser && requestingUser->getUserType() == UserType::ADMIN) {
        Logger::debug("[ChatRoom] Creating iterator for admin " + requestingUser->getName());
        return new ConcreteIterator(&chatHistory);
    } else {
        Logger::info("Iterator access denied - only admins can iterate chat history");
        if (requestingUser) {
            Logger::debug("[ChatRoom] User " + requestingUser->getName() + " (" + requestingUser->getUserTypeString() + ") lacks admin privileges");
        }
        return nullptr;
    }
}

Iterator* ChatRoom::createIterator() {
    Logger::debug("[ChatRoom] WARNING: Creating unrestricted iterator (base Aggregate method)");
    return new ConcreteIterator(&chatHistory);
}

void ChatRoom::removeUser(User* user) {
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); it++) {
        if (*it == user) {
            users.erase(it);
            user->removeChatRoom(this);
            Logger::info(user->getName() + " left the room");
            return;
        }
    }
    
    Logger::debug("[ChatRoom] User " + user->getName() + " was not in this room");
}