/**
 * @file Users.cpp
 * @brief Implementation of User classes with Strategy pattern integration
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include "Users.h"
#include "ChatRoom.h"
#include "Command.h"
#include "SendMessageCommand.h"
#include "SaveMessageCommand.h"
#include "Logger.h"
#include "ValidationStrategy.h"
#include "Iterator.h"
#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================== Base User Class ==================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

User::User(std::string userName, UserType type) : name(userName), userType(type), validationStrategy(nullptr) {
    Logger::debug("[" + getUserTypeString() + " User] " + name + " base constructor");
}

User::~User() {
    std::vector<Command*>::iterator it;
    for (it = commandQueue.begin(); it != commandQueue.end(); it++) {
        delete *it;
    }
    commandQueue.clear();
    delete validationStrategy;
    
    Logger::debug("[" + getUserTypeString() + " User] " + name + " destroyed!");
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

std::string User::toString() const {
    std::stringstream ss;
    ss << "=== User Debug Info ===" << std::endl;
    ss << "Name: " << name << std::endl;
    ss << "Type: " << getUserTypeString() << std::endl;
    ss << "Chat Rooms: " << chatRooms.size() << " rooms" << std::endl;
    
    for (size_t i = 0; i < chatRooms.size(); i++) {
        ss << "  - Room " << i + 1 << " (address: " << chatRooms[i] << ")" << std::endl;
    }
    
    ss << "Command Queue: " << commandQueue.size() << " pending commands" << std::endl;
    
    if (validationStrategy) {
        ss << "Validation Strategy: " << validationStrategy->getStrategyName() << std::endl;
        int maxLen = validationStrategy->getMaxMessageLength();
        ss << "Max Message Length: " << (maxLen == -1 ? "Unlimited" : std::to_string(maxLen)) << std::endl;
    }
    
    ss << "========================" << std::endl;
    
    return ss.str();
}

void User::receive(std::string message, User* fromUser, ChatRoom* room) {
    (void)message;
    (void)room;
    Logger::debug("[" + name + "] Received message from " + fromUser->getName() + " (" + fromUser->getUserTypeString() + ")");
}

void User::addCommand(Command* command) {
    commandQueue.push_back(command);
    Logger::debug("[" + name + "] Command added to queue");
}

void User::executeAll() {
    Logger::debug("[" + name + "] Executing " + std::to_string(commandQueue.size()) + " commands...");
    
    std::vector<Command*>::iterator it;
    for (it = commandQueue.begin(); it != commandQueue.end(); it++) {
        (*it)->execute();
        delete *it;
    }
    
    commandQueue.clear();
    Logger::debug("[" + name + "] All commands executed!");
}

void User::addChatRoom(ChatRoom* room) {

    for (std::vector<ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); it++) {
        if (*it == room) {
            Logger::debug("[" + name + "] Already in this chat room");
            return;
        }
    }
    
    chatRooms.push_back(room);
    Logger::debug("[" + name + "] Added to a chat room");
}

void User::removeChatRoom(ChatRoom* room) {
    for (std::vector<ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); it++) {
        if (*it == room) {
            chatRooms.erase(it);
            Logger::info(name + " left a chat room");
            return;
        }
    }
    Logger::debug("[" + name + "] Was not in the specified chat room");
}

bool User::isInChatRoom(ChatRoom* room) const {
    for (std::vector<ChatRoom*>::const_iterator it = chatRooms.begin(); it != chatRooms.end(); it++) {
        if (*it == room) {
            return true;
        }
    }
    return false;
}

void User::setValidationStrategy(ValidationStrategy* strategy) {
    delete validationStrategy;
    validationStrategy = strategy;
    Logger::debug("[" + name + "] Validation strategy changed to " + 
                  (strategy ? strategy->getStrategyName() : "None"));
}

ValidationStrategy* User::getValidationStrategy() const {
    return validationStrategy;
}

bool User::validateMessage(const std::string& message) {
    if (!validationStrategy) {
        Logger::debug("[" + name + "] No validation strategy set - allowing message");
        return true;
    }
    
    return validationStrategy->validateMessage(message, name);
}

void User::performSend(std::string message, ChatRoom* room) {
    if (!isInChatRoom(room)) {
        Logger::user(name + " tried to send a message but isn't in the room!");
        return;
    }
    
    Logger::debug("[" + name + "] Sending message: \"" + message + "\"");

    Command* sendCmd = new SendMessageCommand(room, this, message);
    Command* saveCmd = new SaveMessageCommand(room, this, message);
    
    addCommand(sendCmd);
    addCommand(saveCmd);
    
    executeAll();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================== FreeUser Class ==================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FreeUser::FreeUser(std::string userName) : User(userName, UserType::FREE), dailyMessageCount(0) {
    validationStrategy = new FreeUserValidationStrategy();
    
    Logger::info(name + " joined PetSpace (Free User - " + std::to_string(DAILY_MESSAGE_LIMIT) + 
                " messages/day, " + std::to_string(validationStrategy->getMaxMessageLength()) + " char limit)");
    Logger::debug("[FreeUser] " + name + " using " + validationStrategy->getStrategyName() + " validation");
}

std::string FreeUser::toString() const {
    std::stringstream ss;
    ss << User::toString();
    ss << "=== Free User Specific ===" << std::endl;
    ss << "Daily Messages Used: " << dailyMessageCount << "/" << DAILY_MESSAGE_LIMIT << std::endl;
    ss << "==========================" << std::endl;
    
    return ss.str();
}

bool FreeUser::send(std::string message, ChatRoom* room) {
    if (dailyMessageCount >= DAILY_MESSAGE_LIMIT) {
        Logger::user(name + ": Daily message limit reached! Upgrade to Premium for unlimited messaging.");
        return false;
    }

    if (!isInChatRoom(room)) {
        Logger::user(name + " tried to send a message but isn't in the room!");
        return false;
    }

    if (!validateMessage(message)) {
        Logger::debug("[" + name + "] Message blocked by " + validationStrategy->getStrategyName() + " strategy");
        return false;
    }

    dailyMessageCount++;
    Logger::debug("[" + name + "] Messages used today: " + std::to_string(dailyMessageCount) + 
                  "/" + std::to_string(DAILY_MESSAGE_LIMIT));
    
    performSend(message, room);
    return true;
}

void FreeUser::resetDailyCount() {
    dailyMessageCount = 0;
    Logger::info(name + "'s daily message count has been reset");
}

int FreeUser::getDailyMessageCount() const {
    return dailyMessageCount;
}

int FreeUser::getDailyMessageLimit() const {
    return DAILY_MESSAGE_LIMIT;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================== PremiumUser Class ==================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PremiumUser::PremiumUser(std::string userName) : User(userName, UserType::PREMIUM) {
    validationStrategy = new PremiumUserValidationStrategy();
    
    Logger::info(name + " joined PetSpace (Premium User - unlimited messaging, mild language allowed)");
    Logger::debug("[PremiumUser] " + name + " using " + validationStrategy->getStrategyName() + " validation");
}

std::string PremiumUser::toString() const {
    std::stringstream ss;
    ss << User::toString();
    ss << "=== Premium User Specific ===" << std::endl;
    ss << "Status: Unlimited messaging enabled" << std::endl;
    ss << "Language Policy: Mild profanity allowed" << std::endl;
    ss << "=============================" << std::endl;
    
    return ss.str();
}

bool PremiumUser::send(std::string message, ChatRoom* room) {
    if (!isInChatRoom(room)) {
        Logger::user(name + " tried to send a message but isn't in the room!");
        return false;
    }

    if (!validateMessage(message)) {
        Logger::debug("[" + name + "] Message blocked by " + validationStrategy->getStrategyName() + " strategy");
        return false;
    }
    
    performSend(message, room);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================== AdminUser Class ==================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AdminUser::AdminUser(std::string userName) : User(userName, UserType::ADMIN) {
    validationStrategy = new AdminUserValidationStrategy();
    
    Logger::info(name + " joined PetSpace (Admin User - full privileges, " + 
                std::to_string(validationStrategy->getMaxMessageLength()) + " char limit)");
    Logger::debug("[AdminUser] " + name + " using " + validationStrategy->getStrategyName() + " validation");
}

std::string AdminUser::toString() const {
    std::stringstream ss;
    ss << User::toString();
    ss << "=== Admin User Specific ===" << std::endl;
    ss << "Privileges: Full administrative access" << std::endl;
    ss << "Can access: Chat history, user management" << std::endl;
    ss << "Language Policy: All language allowed for moderation" << std::endl;
    ss << "============================" << std::endl;
    
    return ss.str();
}

bool AdminUser::send(std::string message, ChatRoom* room) {
    if (!isInChatRoom(room)) {
        Logger::user(name + " tried to send a message but isn't in the room!");
        return false;
    }

    if (!validateMessage(message)) {
        Logger::debug("[" + name + "] Admin message blocked by " + validationStrategy->getStrategyName() + " strategy");
        return false;
    }
    
    Logger::debug("[" + name + "] Admin user - message approved with minimal restrictions");
    performSend(message, room);
    return true;
}

void AdminUser::receive(std::string message, User* fromUser, ChatRoom* room) {
    Logger::debug("[ADMIN LOG] " + name + " received message for moderation review");

    User::receive(message, fromUser, room);
}

Iterator* AdminUser::requestChatHistoryIterator(ChatRoom* room) {
    Logger::debug("[" + name + "] Admin requesting chat history iterator...");
    return room->createIterator(this);
}

void AdminUser::iterateChatHistory(ChatRoom* room) {
    Logger::info("[Admin] " + name + " is viewing chat history...");
    
    Iterator* iterator = requestChatHistoryIterator(room);
    
    if (iterator) {
        Logger::user("=== CHAT HISTORY ===");
        
        for (iterator->first(); !iterator->isDone(); iterator->next()) {
            std::string message = iterator->currentItem();
            Logger::user("  " + message);
        }
        
        Logger::user("=== END HISTORY ===");
        delete iterator;
    } else {
        Logger::user("[Admin] " + name + " failed to access chat history");
    }
}