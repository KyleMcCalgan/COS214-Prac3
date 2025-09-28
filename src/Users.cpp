/**
 * @file Users.cpp
 * @brief Implementation of User classes (Colleague and Invoker)
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include "Users.h"
#include "ChatRoom.h"
#include "Command.h"
#include "SendMessageCommand.h"
#include "SaveMessageCommand.h"
#include <iostream>

// ================== Base User Class ==================

User::User(std::string userName, UserType type) : name(userName), userType(type) {
    std::cout << "[" << getUserTypeString() << " User] " << name << " created!" << std::endl;
}

User::~User() {
    std::vector<Command*>::iterator it;
    for (it = commandQueue.begin(); it != commandQueue.end(); it++) {
        delete *it;
    }
    commandQueue.clear();
    
    std::cout << "[" << getUserTypeString() << " User] " << name << " destroyed!" << std::endl;
}

std::string User::getName() const {
    return name;
}

UserType User::getUserType() const {
    return userType;
}

std::string User::getUserTypeString() const {
    switch (userType) {
        case UserType::FREE: return "Free";
        case UserType::PREMIUM: return "Premium";
        case UserType::ADMIN: return "Admin";
        default: return "Unknown";
    }
}

void User::receive(std::string message, User* fromUser, ChatRoom* room) {
    std::cout << "[" << name << "] Received: \"" << message << "\" from " 
              << fromUser->getName() << " (" << fromUser->getUserTypeString() << ")" << std::endl;
}

void User::addCommand(Command* command) {
    commandQueue.push_back(command);
    std::cout << "[" << name << "] Command added to queue" << std::endl;
}

void User::executeAll() {
    std::cout << "[" << name << "] Executing " << commandQueue.size() << " commands..." << std::endl;
    
    std::vector<Command*>::iterator it;
    for (it = commandQueue.begin(); it != commandQueue.end(); it++) {
        (*it)->execute();
        delete *it;
    }
    
    commandQueue.clear();
    std::cout << "[" << name << "] All commands executed!" << std::endl;
}

void User::addChatRoom(ChatRoom* room) {
    chatRooms.push_back(room);
    std::cout << "[" << name << "] Added to a chat room" << std::endl;
}

void User::performSend(std::string message, ChatRoom* room) {
    std::cout << "\n[" << name << "] Sending message: \"" << message << "\"" << std::endl;
    
    // Create commands for sending and saving the message
    Command* sendCmd = new SendMessageCommand(room, this, message);
    Command* saveCmd = new SaveMessageCommand(room, this, message);
    
    addCommand(sendCmd);
    addCommand(saveCmd);
    
    executeAll();
}

// ================== FreeUser Class ==================

FreeUser::FreeUser(std::string userName) : User(userName, UserType::FREE), dailyMessageCount(0) {
    std::cout << "[Free User] " << name << " has " << DAILY_MESSAGE_LIMIT 
              << " messages per day" << std::endl;
}

bool FreeUser::send(std::string message, ChatRoom* room) {
    if (dailyMessageCount >= DAILY_MESSAGE_LIMIT) {
        std::cout << "\n[" << name << "] BLOCKED: Daily message limit (" 
                  << DAILY_MESSAGE_LIMIT << ") reached!" << std::endl;
        std::cout << "[" << name << "] Consider upgrading to Premium for unlimited messages!" << std::endl;
        return false;
    }
    
    dailyMessageCount++;
    std::cout << "\n[" << name << "] Messages used today: " << dailyMessageCount 
              << "/" << DAILY_MESSAGE_LIMIT << std::endl;
    
    performSend(message, room);
    return true;
}

void FreeUser::resetDailyCount() {
    dailyMessageCount = 0;
    std::cout << "[" << name << "] Daily message count reset!" << std::endl;
}

int FreeUser::getDailyMessageCount() const {
    return dailyMessageCount;
}

int FreeUser::getDailyMessageLimit() const {
    return DAILY_MESSAGE_LIMIT;
}

// ================== PremiumUser Class ==================

PremiumUser::PremiumUser(std::string userName) : User(userName, UserType::PREMIUM) {
    std::cout << "[Premium User] " << name << " has unlimited messaging!" << std::endl;
}

bool PremiumUser::send(std::string message, ChatRoom* room) {
    //std::cout << "\n[" << name << "] Premium user - no message limits!" << std::endl;
    performSend(message, room);
    return true;
}

// ================== AdminUser Class ==================

AdminUser::AdminUser(std::string userName) : User(userName, UserType::ADMIN) {
    std::cout << "[Admin User] " << name << " has administrative privileges!" << std::endl;
}

bool AdminUser::send(std::string message, ChatRoom* room) {
    std::cout << "\n[" << name << "] Admin user - unlimited messaging with admin privileges!" << std::endl;
    performSend(message, room);
    return true;
}

void AdminUser::receive(std::string message, User* fromUser, ChatRoom* room) {
    // Admin gets additional logging for monitoring
    std::cout << "[ADMIN LOG] Message received in system" << std::endl;
    
    // Call base class implementation
    User::receive(message, fromUser, room);
    
    std::cout << "[ADMIN LOG] Message logged for moderation review" << std::endl;
}