/**
 * @file User.cpp
 * @brief Implementation of User class (Colleague and Invoker)
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#include "Users.h"
#include "ChatRoom.h"
#include "Command.h"
#include "SendMessageCommand.h"
#include "SaveMessageCommand.h"
#include <iostream>

User::User(std::string userName) : name(userName) {
    std::cout << "[User] " << name << " created!" << std::endl;
}

User::~User() {
    
    std::vector<Command*>::iterator it;
    for (it = commandQueue.begin(); it != commandQueue.end(); it++) {
        delete *it;
    }
    commandQueue.clear();
    
    std::cout << "[User] " << name << " destroyed!" << std::endl;
}


std::string User::getName() const {
    return name;
}



void User::send(std::string message, ChatRoom* room) {
    std::cout << "\n[" << name << "] Sending message: \"" << message << "\"" << std::endl;
  
    // Instead of directly calling room methods, create Command objects
    // Create commands for sending and saving the message
    Command* sendCmd = new SendMessageCommand(room, this, message);
    Command* saveCmd = new SaveMessageCommand(room, this, message);
    
    addCommand(sendCmd);
    addCommand(saveCmd);
  
    executeAll();
}

void User::receive(std::string message, User* fromUser, ChatRoom* room) {
    
    std::cout << "[" << name << "] Received: \"" << message << "\" from " 
              << fromUser->getName() << std::endl;
}

void User::addCommand(Command* command) {
    
    commandQueue.push_back(command);
    std::cout << "[" << name << "] Command added to queue" << std::endl;
}

void User::executeAll() {
  
    std::cout << "[" << name << "] Executing " << commandQueue.size() << " commands..." << std::endl;
    
    std::vector<Command*>::iterator it;
    for (it = commandQueue.begin(); it != commandQueue.end(); it++) {
        (*it)->execute();  // Execute the command
        delete *it;        // Clean up after execution
    }
    
    commandQueue.clear();  // Clear the queue
    std::cout << "[" << name << "] All commands executed!" << std::endl;
}

void User::addChatRoom(ChatRoom* room) {

    // Add room to user's list
    chatRooms.push_back(room);
    std::cout << "[" << name << "] Added to a chat room" << std::endl;
}