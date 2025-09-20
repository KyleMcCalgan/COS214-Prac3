/**
 * @file TestingMain.cpp
 * @brief Simple test for Mediator and Command patterns
 * @date 2025-09-19
 */

#include <iostream>
#include "Users.h"
#include "ChatRoom.h"
#include "CtrlCat.h"
#include "Dogorithm.h"
#include "Command.h"
#include "SendMessageCommand.h"
#include "SaveMessageCommand.h"

int main() {
    std::cout << "=== PetSpace Chat System Test ===" << std::endl;
    std::cout << "Testing Mediator and Command Patterns\n" << std::endl;
    
    // Create chat rooms (Concrete Mediators)
    std::cout << "--- Creating Chat Rooms ---" << std::endl;
    ChatRoom* ctrlCat = new CtrlCat();
    ChatRoom* dogorithm = new Dogorithm();
    
    // Create users (Colleagues in Mediator, Invokers in Command)
    std::cout << "\n--- Creating Users ---" << std::endl;
    User* alice = new User("Alice");
    User* bob = new User("Bob");
    User* charlie = new User("Charlie");
    
    // Test Mediator Pattern - User Registration
    std::cout << "\n--- Testing Mediator Pattern: User Registration ---" << std::endl;
    ctrlCat->registerUser(alice);
    ctrlCat->registerUser(bob);
    ctrlCat->registerUser(charlie);
    
    // Alice joins both rooms (requirement: at least 1 user in multiple rooms)
    dogorithm->registerUser(alice);
    dogorithm->registerUser(bob);
    
    // Test Command Pattern + Mediator Pattern - Message Sending
    std::cout << "\n--- Testing Command + Mediator Patterns: Message Sending ---" << std::endl;
    
    // Alice sends a message to CtrlCat room
    std::cout << "\n1. Alice sends message to CtrlCat:" << std::endl;
    alice->send("Hello fellow cat lovers!", ctrlCat);
    
    // Bob replies in CtrlCat
    std::cout << "\n2. Bob replies in CtrlCat:" << std::endl;
    bob->send("Hi Alice! Cats are amazing!", ctrlCat);
    
    // Alice sends message to Dogorithm room
    std::cout << "\n3. Alice sends message to Dogorithm:" << std::endl;
    alice->send("Dogs are pretty cool too!", dogorithm);
    
    // Test multiple messages from same user
    std::cout << "\n4. Charlie sends multiple messages:" << std::endl;
    charlie->send("I love cats!", ctrlCat);
    charlie->send("Especially when they code!", ctrlCat);
    
    // Test explicit command creation (to show Command pattern clearly)
    std::cout << "\n--- Testing Explicit Command Creation ---" << std::endl;
    Command* sendCmd = new SendMessageCommand(ctrlCat, alice, "Explicit command test!");
    Command* saveCmd = new SaveMessageCommand(ctrlCat, alice, "Explicit command test!");
    
    alice->addCommand(sendCmd);
    alice->addCommand(saveCmd);
    alice->executeAll();
    
    // Cleanup
    std::cout << "\n--- Cleanup ---" << std::endl;
    delete alice;
    delete bob;
    delete charlie;
    delete ctrlCat;
    delete dogorithm;
    
    std::cout << "\n=== Test Complete ===" << std::endl;
    return 0;
}