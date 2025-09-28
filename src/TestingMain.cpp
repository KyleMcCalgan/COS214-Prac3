/**
 * @file TestingMain.cpp
 * @brief Separated test functions for Design Patterns - call individually as needed
 * @date 2025-09-28
 */

#include <iostream>
#include "Users.h"
#include "ChatRoom.h"
#include "CtrlCat.h"
#include "Dogorithm.h"
#include "Command.h"
#include "SendMessageCommand.h"
#include "SaveMessageCommand.h"

void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << title << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

// ================== MEDIATOR PATTERN TEST ==================
void testMediatorPattern() {
    printSeparator("MEDIATOR PATTERN TEST");
    
    // Create mediators (chat rooms)
    ChatRoom* ctrlCat = new CtrlCat();
    ChatRoom* dogorithm = new Dogorithm();
    
    // Create colleagues (users)
    User* alice = new PremiumUser("Alice");
    User* bob = new PremiumUser("Bob");
    User* charlie = new PremiumUser("Charlie");
    
    // Register users with mediators
    std::cout << "\n--- User Registration ---" << std::endl;
    ctrlCat->registerUser(alice);
    ctrlCat->registerUser(bob);
    ctrlCat->registerUser(charlie);
    
    dogorithm->registerUser(alice);
    dogorithm->registerUser(bob);
    
    // Test mediator communication
    std::cout << "\n--- Mediator Communication ---" << std::endl;
    alice->send("Hello everyone in CtrlCat!", ctrlCat);
    bob->send("Hi Alice! Great to chat!", ctrlCat);
    
    std::cout << "\n--- Cross-room mediation ---" << std::endl;
    alice->send("Dogs are awesome too!", dogorithm);
    bob->send("I love all pets!", dogorithm);
    
    // Cleanup
    delete alice;
    delete bob;
    delete charlie;
    delete ctrlCat;
    delete dogorithm;
}

// ================== COMMAND PATTERN TEST ==================
void testCommandPattern() {
    printSeparator("COMMAND PATTERN TEST");
    
    ChatRoom* testRoom = new CtrlCat();
    User* user = new PremiumUser("CommandTester");
    testRoom->registerUser(user);
    
    // Test automatic command creation
    std::cout << "\n--- Automatic Command Creation ---" << std::endl;
    user->send("This creates commands automatically!", testRoom);
    
    // Test manual command creation and queuing
    std::cout << "\n--- Manual Command Creation ---" << std::endl;
    Command* sendCmd1 = new SendMessageCommand(testRoom, user, "Manual command 1");
    Command* saveCmd1 = new SaveMessageCommand(testRoom, user, "Manual command 1");
    Command* sendCmd2 = new SendMessageCommand(testRoom, user, "Manual command 2");
    Command* saveCmd2 = new SaveMessageCommand(testRoom, user, "Manual command 2");
    
    user->addCommand(sendCmd1);
    user->addCommand(saveCmd1);
    user->addCommand(sendCmd2);
    user->addCommand(saveCmd2);
    
    std::cout << "Executing all queued commands:" << std::endl;
    user->executeAll();
    
    // Cleanup
    delete user;
    delete testRoom;
}

// ================== USER HIERARCHY TEST ==================
void testUserHierarchy() {
    printSeparator("USER HIERARCHY TEST");
    
    ChatRoom* testRoom = new CtrlCat();
    
    // Create different user types
    std::cout << "\n--- Creating Different User Types ---" << std::endl;
    FreeUser* freeUser = new FreeUser("FreeUserTest");
    PremiumUser* premiumUser = new PremiumUser("PremiumUserTest");
    AdminUser* adminUser = new AdminUser("AdminUserTest");
    
    testRoom->registerUser(freeUser);
    testRoom->registerUser(premiumUser);
    testRoom->registerUser(adminUser);
    
    // Test free user limits
    std::cout << "\n--- Testing Free User Limits ---" << std::endl;
    std::cout << "Free user limit: " << freeUser->getDailyMessageLimit() << " messages" << std::endl;
    
    for (int i = 1; i <= 12; i++) {
        std::string message = "Test message #" + std::to_string(i);
        bool success = freeUser->send(message, testRoom);
        if (!success) {
            std::cout << "Message " << i << " blocked - limit reached!" << std::endl;
            break;
        }
    }
    
    // Test premium user unlimited
    std::cout << "\n--- Testing Premium User Unlimited ---" << std::endl;
    premiumUser->send("Premium user message 1", testRoom);
    premiumUser->send("Premium user message 2", testRoom);
    premiumUser->send("Premium user message 3", testRoom);
    
    // Test admin special privileges
    std::cout << "\n--- Testing Admin Special Privileges ---" << std::endl;
    adminUser->send("Admin message with special logging", testRoom);
    
    // Test daily reset
    std::cout << "\n--- Testing Daily Reset ---" << std::endl;
    std::cout << "Before reset: " << freeUser->getDailyMessageCount() << "/" << freeUser->getDailyMessageLimit() << std::endl;
    freeUser->resetDailyCount();
    std::cout << "After reset: " << freeUser->getDailyMessageCount() << "/" << freeUser->getDailyMessageLimit() << std::endl;
    freeUser->send("Message after reset!", testRoom);
    
    // Cleanup
    delete freeUser;
    delete premiumUser;
    delete adminUser;
    delete testRoom;
}

// ================== POLYMORPHISM TEST ==================
void testPolymorphism() {
    printSeparator("POLYMORPHISM TEST");
    
    ChatRoom* polyRoom = new CtrlCat();
    
    // Create array of User pointers
    std::cout << "\n--- Polymorphic User Array ---" << std::endl;
    User* users[3];
    users[0] = new FreeUser("PolyFree");
    users[1] = new PremiumUser("PolyPremium");
    users[2] = new AdminUser("PolyAdmin");
    
    // Register all users
    for (int i = 0; i < 3; i++) {
        polyRoom->registerUser(users[i]);
    }
    
    // Test polymorphic behavior
    std::cout << "\n--- Polymorphic Method Calls ---" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::string message = "Message from " + users[i]->getUserTypeString() + " user";
        std::cout << "\n" << users[i]->getName() << " (" << users[i]->getUserTypeString() << "):" << std::endl;
        users[i]->send(message, polyRoom);
    }
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        delete users[i];
    }
    delete polyRoom;
}

// ================== INTEGRATION TEST ==================
void testIntegration() {
    printSeparator("INTEGRATION TEST - ALL PATTERNS TOGETHER");
    
    // Create system
    ChatRoom* ctrlCat = new CtrlCat();
    ChatRoom* dogorithm = new Dogorithm();
    
    User* alice = new FreeUser("Alice");
    User* bob = new PremiumUser("Bob");
    User* charlie = new AdminUser("Charlie");
    
    // Register users
    std::cout << "\n--- System Setup ---" << std::endl;
    ctrlCat->registerUser(alice);
    ctrlCat->registerUser(bob);
    ctrlCat->registerUser(charlie);
    
    dogorithm->registerUser(alice);
    dogorithm->registerUser(bob);
    dogorithm->registerUser(charlie);
    
    // Realistic conversation flow
    std::cout << "\n--- Realistic Chat Scenario ---" << std::endl;
    bob->send("Welcome to PetSpace everyone!", ctrlCat);
    alice->send("Thanks! I love cats!", ctrlCat);
    charlie->send("Admin here - great conversation!", ctrlCat);
    
    alice->send("Dogs are cool too!", dogorithm);
    bob->send("All pets are amazing!", dogorithm);
    
    // Manual command demonstration
    std::cout << "\n--- Manual Command Integration ---" << std::endl;
    Command* adminCmd1 = new SendMessageCommand(ctrlCat, charlie, "Admin announcement!");
    Command* adminCmd2 = new SaveMessageCommand(ctrlCat, charlie, "Admin announcement!");
    
    charlie->addCommand(adminCmd1);
    charlie->addCommand(adminCmd2);
    charlie->executeAll();
    
    // Cleanup
    delete alice;
    delete bob;
    delete charlie;
    delete ctrlCat;
    delete dogorithm;
}

// ================== MAIN FUNCTION ==================
int main() {
    std::cout << "=== PetSpace Chat System Test Suite ===" << std::endl;
    std::cout << "Testing Mediator and Command Patterns with User Hierarchy\n" << std::endl;

    testMediatorPattern();
    // testCommandPattern();
    // testUserHierarchy();
    // testPolymorphism();
    // testIntegration();
    
    std::cout << "\n=== All Tests Complete ===" << std::endl;
    return 0;
}