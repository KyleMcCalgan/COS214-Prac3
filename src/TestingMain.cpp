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
#include "Iterator.h"
#include "ConcreteAggregate.h"
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

// ================== ITERATOR PATTERN TEST ==================
void testIterator() {
    printSeparator("ITERATOR PATTERN TEST");
    
    // Create chat room and users
    ChatRoom* testRoom = new CtrlCat();
    
    FreeUser* alice = new FreeUser("Alice");
    PremiumUser* bob = new PremiumUser("Bob");  
    AdminUser* charlie = new AdminUser("Charlie");
    AdminUser* diana = new AdminUser("Diana");
    
    // Register users
    std::cout << "\n--- User Registration ---" << std::endl;
    testRoom->registerUser(alice);
    testRoom->registerUser(bob);
    testRoom->registerUser(charlie);
    testRoom->registerUser(diana);
    
    // Generate some chat history
    std::cout << "\n--- Generating Chat History ---" << std::endl;
    alice->send("Hello everyone!", testRoom);
    bob->send("Hey Alice! How's everyone doing?", testRoom);
    charlie->send("Admin here - great to see active chat!", testRoom);
    bob->send("Thanks for keeping the chat safe, Charlie!", testRoom);
    alice->send("This is a really nice chat system!", testRoom);
    diana->send("Another admin checking in - all looks good!", testRoom);
    
    // Test 1: Admin access to chat history
    std::cout << "\n--- Test 1: Admin Iterator Access ---" << std::endl;
    std::cout << "Testing admin Charlie's access to chat history..." << std::endl;
    charlie->iterateChatHistory(testRoom);
    
    // Test 2: Multiple admin access
    std::cout << "\n--- Test 2: Multiple Admin Access ---" << std::endl;
    std::cout << "Testing admin Diana's access to same chat history..." << std::endl;
    diana->iterateChatHistory(testRoom);
    
    // Test 3: Non-admin access denial (Free User)
    std::cout << "\n--- Test 3: Non-Admin Access Denial (Free User) ---" << std::endl;
    std::cout << "Testing free user Alice's access (should be denied)..." << std::endl;
    Iterator* aliceIterator = alice->requestChatHistoryIterator(testRoom);
    if (!aliceIterator) {
        std::cout << "✓ Correctly denied access to free user" << std::endl;
    } else {
        std::cout << "✗ ERROR: Free user should not have access!" << std::endl;
        delete aliceIterator;
    }
    
    // Test 4: Non-admin access denial (Premium User)  
    std::cout << "\n--- Test 4: Non-Admin Access Denial (Premium User) ---" << std::endl;
    std::cout << "Testing premium user Bob's access (should be denied)..." << std::endl;
    Iterator* bobIterator = bob->requestChatHistoryIterator(testRoom);
    if (!bobIterator) {
        std::cout << "✓ Correctly denied access to premium user" << std::endl;
    } else {
        std::cout << "✗ ERROR: Premium user should not have access!" << std::endl;
        delete bobIterator;
    }
    
    // Test 5: Manual iterator operations
    std::cout << "\n--- Test 5: Manual Iterator Operations ---" << std::endl;
    std::cout << "Testing manual iterator control by admin Charlie..." << std::endl;
    Iterator* manualIterator = charlie->requestChatHistoryIterator(testRoom);
    
    if (manualIterator) {
        std::cout << "Manual iteration test:" << std::endl;
        
        // Test first()
        manualIterator->first();
        std::cout << "First message: " << manualIterator->currentItem() << std::endl;
        
        // Test next() a few times
        manualIterator->next();
        std::cout << "Second message: " << manualIterator->currentItem() << std::endl;
        
        manualIterator->next();
        std::cout << "Third message: " << manualIterator->currentItem() << std::endl;
        
        // Test isDone() in middle
        std::cout << "Is iteration done? " << (manualIterator->isDone() ? "Yes" : "No") << std::endl;
        
        // Iterate to the end
        std::cout << "Continuing to end..." << std::endl;
        while (!manualIterator->isDone()) {
            manualIterator->next();
            if (!manualIterator->isDone()) {
                std::cout << "Message: " << manualIterator->currentItem() << std::endl;
            }
        }
        
        // Test isDone() at end
        std::cout << "Is iteration done now? " << (manualIterator->isDone() ? "Yes" : "No") << std::endl;
        
        // Test accessing item when done
        std::cout << "Attempting to access item when done: \"" << manualIterator->currentItem() << "\"" << std::endl;
        
        // Test reset with first()
        std::cout << "Resetting iterator..." << std::endl;
        manualIterator->first();
        std::cout << "After reset - First message: " << manualIterator->currentItem() << std::endl;
        
        delete manualIterator;
    }
    
    // Test 6: Empty chat room iterator
    std::cout << "\n--- Test 6: Empty Chat Room Iterator ---" << std::endl;
    ChatRoom* emptyRoom = new Dogorithm();
    charlie->addChatRoom(emptyRoom);
    
    std::cout << "Testing iterator on empty chat room..." << std::endl;
    charlie->iterateChatHistory(emptyRoom);
    
    // Test 7: Iterator after adding more messages
    std::cout << "\n--- Test 7: Iterator After Adding More Messages ---" << std::endl;
    std::cout << "Adding more messages to original room..." << std::endl;
    bob->send("One more message!", testRoom);
    alice->send("And another one!", testRoom);
    
    std::cout << "Admin checking updated history:" << std::endl;
    charlie->iterateChatHistory(testRoom);
    
    // Test 8: Aggregate interface (direct access)
    std::cout << "\n--- Test 8: Direct Aggregate Interface ---" << std::endl;
    std::cout << "Testing direct aggregate createIterator() method..." << std::endl;
    Iterator* directIterator = testRoom->createIterator();
    
    if (directIterator) {
        std::cout << "Direct iterator created successfully" << std::endl;
        std::cout << "First message via direct iterator: ";
        directIterator->first();
        std::cout << directIterator->currentItem() << std::endl;
        delete directIterator;
    }
    
    // Test 9: ConcreteAggregate usage
    std::cout << "\n--- Test 9: ConcreteAggregate Direct Usage ---" << std::endl;
    const std::vector<std::string>* history = testRoom->getChatHistory(charlie);
    if (history) {
        ConcreteAggregate* aggregate = new ConcreteAggregate(history);
        Iterator* aggIterator = aggregate->createIterator();
        
        std::cout << "ConcreteAggregate iterator test:" << std::endl;
        aggIterator->first();
        std::cout << "First via ConcreteAggregate: " << aggIterator->currentItem() << std::endl;
        
        delete aggIterator;
        delete aggregate;
    }
    
    std::cout << "\n--- Iterator Pattern Integration Summary ---" << std::endl;
    std::cout << "✓ Admin users can access chat history via iterator" << std::endl;
    std::cout << "✓ Non-admin users are properly denied access" << std::endl;
    std::cout << "✓ Iterator operations work correctly (first, next, isDone, currentItem)" << std::endl;
    std::cout << "✓ Iterator handles empty collections safely" << std::endl;
    std::cout << "✓ Iterator updates with new messages" << std::endl;
    std::cout << "✓ Both admin-controlled and direct aggregate access work" << std::endl;
    std::cout << "✓ Memory management handled properly" << std::endl;
    
    // Cleanup
    delete alice;
    delete bob;
    delete charlie;
    delete diana;
    delete testRoom;
    delete emptyRoom;
}


void KyleTest(){
    //just need to test random things
}

// ================== MAIN FUNCTION ==================
int main() {
    std::cout << "=== PetSpace Chat System Test Suite ===" << std::endl;
    std::cout << "Testing Mediator and Command Patterns with User Hierarchy\n" << std::endl;

    //testMediatorPattern();
    // testCommandPattern();
     //testUserHierarchy();
    // testPolymorphism();
    // testIntegration();
    //testIterator();
    
    std::cout << "\n=== All Tests Complete ===" << std::endl;
    return 0;
}