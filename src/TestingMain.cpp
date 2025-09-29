/**
 * @file TestingMain.cpp
 * @brief Separated test functions for Design Patterns - call individually as needed
 * @date 2025-09-28
 */

#include <iostream>
#include <cassert>
#include "Users.h"
#include "ChatRoom.h"
#include "CtrlCat.h"
#include "Dogorithm.h"
#include "Command.h"
#include "Iterator.h"
#include "ConcreteAggregate.h"
#include "SendMessageCommand.h"
#include "SaveMessageCommand.h"
#include "Logger.h"
#include "ValidationStrategy.h"

void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << title << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

// ================== MEDIATOR PATTERN TEST ==================
void testMediatorPattern() {
    printSeparator("MEDIATOR PATTERN TEST");
    
    ChatRoom* ctrlCat = new CtrlCat();
    ChatRoom* dogorithm = new Dogorithm();
    
    User* alice = new PremiumUser("Alice");
    User* bob = new PremiumUser("Bob");
    User* charlie = new PremiumUser("Charlie");
    
    std::cout << "\n--- User Registration ---" << std::endl;
    ctrlCat->registerUser(alice);
    ctrlCat->registerUser(bob);
    ctrlCat->registerUser(charlie);
    
    dogorithm->registerUser(alice);
    dogorithm->registerUser(bob);
    
    std::cout << "\n--- Mediator Communication ---" << std::endl;
    alice->send("Hello everyone in CtrlCat!", ctrlCat);
    bob->send("Hi Alice! Great to chat!", ctrlCat);
    
    std::cout << "\n--- Cross-room mediation ---" << std::endl;
    alice->send("Dogs are awesome too!", dogorithm);
    bob->send("I love all pets!", dogorithm);
    
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
    
    std::cout << "\n--- Automatic Command Creation ---" << std::endl;
    user->send("This creates commands automatically!", testRoom);
    
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
    
    delete user;
    delete testRoom;
}

// ================== USER HIERARCHY TEST ==================
void testUserHierarchy() {
    printSeparator("USER HIERARCHY TEST");
    
    ChatRoom* testRoom = new CtrlCat();
    
    std::cout << "\n--- Creating Different User Types ---" << std::endl;
    FreeUser* freeUser = new FreeUser("FreeUserTest");
    PremiumUser* premiumUser = new PremiumUser("PremiumUserTest");
    AdminUser* adminUser = new AdminUser("AdminUserTest");
    
    testRoom->registerUser(freeUser);
    testRoom->registerUser(premiumUser);
    testRoom->registerUser(adminUser);
    
    std::cout << "\n--- Testing Free User Limits ---" << std::endl;
    std::cout << "Free user limit: " << freeUser->getDailyMessageLimit() << " messages" << std::endl;
    
    for (int i = 1; i <= 12; i++) {
        std::string message = "Test message " + std::to_string(i);
        bool success = freeUser->send(message, testRoom);
        if (!success) {
            std::cout << "Message " << i << " blocked - limit reached!" << std::endl;
            break;
        }
    }
    
    std::cout << "\n--- Testing Premium User Unlimited ---" << std::endl;
    premiumUser->send("Premium user message 1", testRoom);
    premiumUser->send("Premium user message 2", testRoom);
    premiumUser->send("Premium user message 3", testRoom);
    
    std::cout << "\n--- Testing Admin Special Privileges ---" << std::endl;
    adminUser->send("Admin message with special logging", testRoom);
    
    std::cout << "\n--- Testing Daily Reset ---" << std::endl;
    std::cout << "Before reset: " << freeUser->getDailyMessageCount() << "/" << freeUser->getDailyMessageLimit() << std::endl;
    freeUser->resetDailyCount();
    std::cout << "After reset: " << freeUser->getDailyMessageCount() << "/" << freeUser->getDailyMessageLimit() << std::endl;
    freeUser->send("Message after reset!", testRoom);
    
    delete freeUser;
    delete premiumUser;
    delete adminUser;
    delete testRoom;
}

// ================== ITERATOR PATTERN TEST ==================
void testIterator() {
    printSeparator("ITERATOR PATTERN TEST");
    
    ChatRoom* testRoom = new CtrlCat();
    
    FreeUser* alice = new FreeUser("Alice");
    PremiumUser* bob = new PremiumUser("Bob");  
    AdminUser* charlie = new AdminUser("Charlie");
    
    std::cout << "\n--- User Registration ---" << std::endl;
    testRoom->registerUser(alice);
    testRoom->registerUser(bob);
    testRoom->registerUser(charlie);
    
    std::cout << "\n--- Generating Chat History ---" << std::endl;
    alice->send("Hello everyone!", testRoom);
    bob->send("Hey Alice! How are you?", testRoom);
    charlie->send("Admin here - great chat!", testRoom);
    
    std::cout << "\n--- Admin Iterator Access ---" << std::endl;
    charlie->iterateChatHistory(testRoom);
    
    std::cout << "\n--- Non-Admin Access Denial ---" << std::endl;
    Iterator* aliceIterator = alice->requestChatHistoryIterator(testRoom);
    if (!aliceIterator) {
        std::cout << "Correctly denied access to free user" << std::endl;
    }
    
    Iterator* bobIterator = bob->requestChatHistoryIterator(testRoom);
    if (!bobIterator) {
        std::cout << "Correctly denied access to premium user" << std::endl;
    }
    
    std::cout << "\n--- Manual Iterator Operations ---" << std::endl;
    Iterator* manualIterator = charlie->requestChatHistoryIterator(testRoom);
    
    if (manualIterator) {
        manualIterator->first();
        std::cout << "First message: " << manualIterator->currentItem() << std::endl;
        
        manualIterator->next();
        std::cout << "Second message: " << manualIterator->currentItem() << std::endl;
        
        while (!manualIterator->isDone()) {
            manualIterator->next();
        }
        
        std::cout << "Iteration complete" << std::endl;
        delete manualIterator;
    }
    
    delete alice;
    delete bob;
    delete charlie;
    delete testRoom;
}

// ================== mutiple iterators test ==================
void testMultipleIterators() {
    ChatRoom* room = new CtrlCat();
    AdminUser* admin1 = new AdminUser("Admin1");
    AdminUser* admin2 = new AdminUser("Admin2");
    
    room->registerUser(admin1);
    room->registerUser(admin2);
    
    admin1->send("Message 1", room);
    admin1->send("Message 2", room);
    
    // Create two iterators simultaneously
    Iterator* iter1 = admin1->requestChatHistoryIterator(room);
    Iterator* iter2 = admin2->requestChatHistoryIterator(room);
    
    if (iter1 && iter2) {
        iter1->first();
        iter2->first();
        
        std::cout << "Iter1: " << iter1->currentItem() << std::endl;
        std::cout << "Iter2: " << iter2->currentItem() << std::endl;
        
        iter1->next();
        std::cout << "Iter1 next: " << iter1->currentItem() << std::endl;
        std::cout << "Iter2 still: " << iter2->currentItem() << std::endl;
        
        delete iter1;
        delete iter2;
    }
    
    delete admin1;
    delete admin2;
    delete room;
}

//profanity test-----------------------
void testProfanityInWords() {
    ChatRoom* room = new CtrlCat();
    FreeUser* free = new FreeUser("Free");
    room->registerUser(free);
    
    // Test that "class" doesn't trigger "ass" filter
    free->send("I love my class", room);
    
    // Test "assumptions" doesn't trigger
    free->send("My assumptions are correct", room);
    
    // But "ass" alone should fail
    free->send("You are an ass", room);
    
    delete free;
    delete room;
}

//test users in chat room
void testIsInChatRoom() {
    ChatRoom* room1 = new CtrlCat();
    ChatRoom* room2 = new Dogorithm();
    FreeUser* user = new FreeUser("Test");
    
    room1->registerUser(user);
    
    // Test positive case
    if (user->isInChatRoom(room1)) {
        std::cout << "User is in room1" << std::endl;
    }
    
    // Test negative case
    if (!user->isInChatRoom(room2)) {
        std::cout << "User is not in room2" << std::endl;
    }
    
    delete user;
    delete room1;
    delete room2;
}
// ================== EDGE CASES TEST ==================
void testEdgeCases() {
    printSeparator("EDGE CASES TEST");
    
    ChatRoom* room = new CtrlCat();
    FreeUser* user = new FreeUser("EdgeTester");
    AdminUser* admin = new AdminUser("AdminEdge");
    
    room->registerUser(user);
    room->registerUser(admin);
    
    std::cout << "\n--- Empty Message Test ---" << std::endl;
    user->send("", room);
    
    std::cout << "\n--- Boundary Tests (100 chars) ---" << std::endl;
    std::string exactly100(100, 'a');
    user->send(exactly100, room);
    
    std::cout << "\n--- Just Over Boundary (101 chars) ---" << std::endl;
    std::string exactly101(101, 'a');
    user->send(exactly101, room);
    
    std::cout << "\n--- Unregistered User Test ---" << std::endl;
    FreeUser* unregistered = new FreeUser("NotInRoom");
    unregistered->send("Should fail", room);
    
    std::cout << "\n--- Iterator on Empty Room ---" << std::endl;
    ChatRoom* emptyRoom = new Dogorithm();
    admin->addChatRoom(emptyRoom);
    admin->iterateChatHistory(emptyRoom);
    
    std::cout << "\n--- Iterator Edge Cases ---" << std::endl;
    Iterator* iter = admin->requestChatHistoryIterator(room);
    if (iter) {
        // Navigate to end
        while (!iter->isDone()) {
            iter->next();
        }
        
        // Try operations when done
        std::cout << "Trying next when done..." << std::endl;
        iter->next();
        std::cout << "Trying currentItem when done: \"" << iter->currentItem() << "\"" << std::endl;
        
        // Reset and try again
        iter->first();
        std::cout << "After reset: " << iter->currentItem() << std::endl;
        
        delete iter;
    }
    
    std::cout << "\n--- Remove Non-Existent User ---" << std::endl;
    FreeUser* notInRoom = new FreeUser("Ghost");
    room->removeUser(notInRoom);
    
    std::cout << "\n--- Null Parameter Tests ---" << std::endl;
    const std::vector<std::string>* nullHistory = room->getChatHistory(nullptr);
    if (!nullHistory) {
        std::cout << "Correctly handled null user for history" << std::endl;
    }
    
    Iterator* nullIter = room->createIterator(nullptr);
    if (!nullIter) {
        std::cout << "Correctly handled null user for iterator" << std::endl;
    }
    
    std::cout << "\n--- Double Registration Test ---" << std::endl;
    room->registerUser(user);  // Already registered
    
    delete user;
    delete admin;
    delete unregistered;
    delete notInRoom;
    delete room;
    delete emptyRoom;
}

// ================== VALIDATION STRATEGY BOUNDARIES TEST ==================
void testValidationBoundaries() {
    printSeparator("VALIDATION STRATEGY BOUNDARIES TEST");
    
    ChatRoom* room = new CtrlCat();
    
    FreeUser* free = new FreeUser("Free");
    PremiumUser* premium = new PremiumUser("Premium");
    AdminUser* admin = new AdminUser("Admin");
    
    room->registerUser(free);
    room->registerUser(premium);
    room->registerUser(admin);
    
    std::cout << "\n--- Free User Profanity Tests ---" << std::endl;
    free->send("This is stupid", room);
    free->send("I hate this", room);
    free->send("This sucks", room);
    
    std::cout << "\n--- Premium User Mild Profanity ---" << std::endl;
    premium->send("This is stupid but works", room);
    premium->send("That sucks but allowed", room);
    
    std::cout << "\n--- Premium User Severe Profanity ---" << std::endl;
    premium->send("This is fucking broken", room);
    premium->send("What the shit", room);
    
    std::cout << "\n--- Admin System Threats ---" << std::endl;
    admin->send("Normal admin message", room);
    admin->send("DELETE FROM users", room);
    admin->send("shutdown now", room);
    admin->send("rm -rf /", room);
    
    std::cout << "\n--- Caps Limit Tests ---" << std::endl;
    free->send("HELLO WORLD TEST", room);
    premium->send("HELLO PREMIUM WORLD", room);
    admin->send("ADMIN ANNOUNCEMENT", room);
    
    std::cout << "\n--- Character Repetition Tests ---" << std::endl;
    free->send("Hellooooo", room);
    premium->send("Awesooooooome!", room);
    
    std::cout << "\n--- Whitespace Tests ---" << std::endl;
    free->send("   ", room);
    premium->send("     ", room);
    
    delete free;
    delete premium;
    delete admin;
    delete room;
}

// ================== USER LIMITS TEST ==================
void testUserLimits() {
    printSeparator("USER LIMITS TEST");
    
    ChatRoom* room = new CtrlCat();
    FreeUser* free = new FreeUser("LimitTester");
    room->registerUser(free);
    
    std::cout << "\n--- Testing Exact Message Limit ---" << std::endl;
    for (int i = 1; i <= 10; i++) {
        bool result = free->send("Message " + std::to_string(i), room);
        std::cout << "Message " << i << ": " << (result ? "Sent" : "Failed") << std::endl;
    }
    
    std::cout << "\n--- Testing 11th Message (Should Fail) ---" << std::endl;
    bool result = free->send("Message 11", room);
    std::cout << "11th message result: " << (result ? "Sent" : "Failed") << std::endl;
    
    std::cout << "\n--- Testing After Reset ---" << std::endl;
    free->resetDailyCount();
    result = free->send("After reset", room);
    std::cout << "After reset: " << (result ? "Sent" : "Failed") << std::endl;
    
    std::cout << "\n--- Testing Reset at Zero ---" << std::endl;
    FreeUser* fresh = new FreeUser("FreshUser");
    room->registerUser(fresh);
    std::cout << "Count before: " << fresh->getDailyMessageCount() << std::endl;
    fresh->resetDailyCount();
    std::cout << "Count after: " << fresh->getDailyMessageCount() << std::endl;
    
    delete free;
    delete fresh;
    delete room;
}

// ================== COMMAND QUEUE TEST ==================
void testCommandQueue() {
    printSeparator("COMMAND QUEUE TEST");
    
    ChatRoom* room = new CtrlCat();
    PremiumUser* user = new PremiumUser("QueueTester");
    room->registerUser(user);
    
    std::cout << "\n--- Execute Empty Queue ---" << std::endl;
    user->executeAll();
    
    std::cout << "\n--- Add Multiple Commands ---" << std::endl;
    Command* cmd1 = new SendMessageCommand(room, user, "Command 1");
    Command* cmd2 = new SaveMessageCommand(room, user, "Command 1");
    Command* cmd3 = new SendMessageCommand(room, user, "Command 2");
    
    user->addCommand(cmd1);
    user->addCommand(cmd2);
    user->addCommand(cmd3);
    
    std::cout << "\n--- Execute All Commands ---" << std::endl;
    user->executeAll();
    
    std::cout << "\n--- Queue Should Be Empty ---" << std::endl;
    user->executeAll();
    
    delete user;
    delete room;
}

// ================== CROSS-ROOM FUNCTIONALITY TEST ==================
void testCrossRoomFunctionality() {
    printSeparator("CROSS-ROOM FUNCTIONALITY TEST");
    
    ChatRoom* ctrlCat = new CtrlCat();
    ChatRoom* dogorithm = new Dogorithm();
    
    PremiumUser* alice = new PremiumUser("Alice");
    PremiumUser* bob = new PremiumUser("Bob");
    
    std::cout << "\n--- Register Users in Multiple Rooms ---" << std::endl;
    ctrlCat->registerUser(alice);
    ctrlCat->registerUser(bob);
    
    dogorithm->registerUser(alice);
    dogorithm->registerUser(bob);
    
    std::cout << "\n--- Send to Different Rooms ---" << std::endl;
    alice->send("Message in CtrlCat", ctrlCat);
    alice->send("Message in Dogorithm", dogorithm);
    
    std::cout << "\n--- Leave One Room ---" << std::endl;
    ctrlCat->removeUser(alice);
    
    std::cout << "\n--- Try Sending to Left Room ---" << std::endl;
    alice->send("Should fail in CtrlCat", ctrlCat);
    alice->send("Should work in Dogorithm", dogorithm);
    
    std::cout << "\n--- Bob Still in Both Rooms ---" << std::endl;
    bob->send("Bob in CtrlCat", ctrlCat);
    bob->send("Bob in Dogorithm", dogorithm);
    
    delete alice;
    delete bob;
    delete ctrlCat;
    delete dogorithm;
}

// ================== CONCRETE AGGREGATE TEST ==================
void testConcreteAggregate() {
    printSeparator("CONCRETE AGGREGATE TEST");
    
    ChatRoom* room = new CtrlCat();
    AdminUser* admin = new AdminUser("AdminAgg");
    PremiumUser* user = new PremiumUser("UserAgg");
    
    room->registerUser(admin);
    room->registerUser(user);
    
    std::cout << "\n--- Generate History ---" << std::endl;
    user->send("Message 1", room);
    user->send("Message 2", room);
    user->send("Message 3", room);
    
    std::cout << "\n--- Get History and Create Aggregate ---" << std::endl;
    const std::vector<std::string>* history = room->getChatHistory(admin);
    
    if (history) {
        ConcreteAggregate* aggregate = new ConcreteAggregate(history);
        Iterator* aggIterator = aggregate->createIterator();
        
        std::cout << "\n--- Iterate Through Aggregate ---" << std::endl;
        for (aggIterator->first(); !aggIterator->isDone(); aggIterator->next()) {
            std::cout << "Via aggregate: " << aggIterator->currentItem() << std::endl;
        }
        
        delete aggIterator;
        delete aggregate;
    }
    
    delete admin;
    delete user;
    delete room;
}

// ================== POLYMORPHISM TEST ==================
void testPolymorphism() {
    printSeparator("POLYMORPHISM TEST");
    
    ChatRoom* room = new CtrlCat();
    
    std::cout << "\n--- Polymorphic User Array ---" << std::endl;
    User* users[3];
    users[0] = new FreeUser("PolyFree");
    users[1] = new PremiumUser("PolyPremium");
    users[2] = new AdminUser("PolyAdmin");
    
    for (int i = 0; i < 3; i++) {
        room->registerUser(users[i]);
    }
    
    std::cout << "\n--- Polymorphic Method Calls ---" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::string message = "Message from " + users[i]->getUserTypeString();
        std::cout << "\n" << users[i]->getName() << " (" << users[i]->getUserTypeString() << "):" << std::endl;
        users[i]->send(message, room);
    }
    
    std::cout << "\n--- Polymorphic toString ---" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << users[i]->toString() << std::endl;
    }
    
    for (int i = 0; i < 3; i++) {
        delete users[i];
    }
    delete room;
}

// ================== STRATEGY PATTERN TEST ==================
void testStrategyPattern() {
    printSeparator("STRATEGY PATTERN TEST");
    
    ChatRoom* room = new CtrlCat();
    
    FreeUser* free = new FreeUser("StrategyFree");
    PremiumUser* premium = new PremiumUser("StrategyPremium");
    AdminUser* admin = new AdminUser("StrategyAdmin");
    
    room->registerUser(free);
    room->registerUser(premium);
    room->registerUser(admin);
    
    std::cout << "\n--- Strategy Information ---" << std::endl;
    std::cout << "Free strategy: " << free->getValidationStrategy()->getStrategyName() << std::endl;
    std::cout << "Free max length: " << free->getValidationStrategy()->getMaxMessageLength() << std::endl;
    
    std::cout << "Premium strategy: " << premium->getValidationStrategy()->getStrategyName() << std::endl;
    std::cout << "Premium max length: " << premium->getValidationStrategy()->getMaxMessageLength() << std::endl;
    
    std::cout << "Admin strategy: " << admin->getValidationStrategy()->getStrategyName() << std::endl;
    std::cout << "Admin max length: " << admin->getValidationStrategy()->getMaxMessageLength() << std::endl;
    
    std::cout << "\n--- Different Behaviors with Same Message ---" << std::endl;
    std::string testMsg = "This is stupid and sucks";
    
    std::cout << "Free user: ";
    free->send(testMsg, room);
    
    std::cout << "Premium user: ";
    premium->send(testMsg, room);
    
    std::cout << "Admin user: ";
    admin->send(testMsg, room);
    
    delete free;
    delete premium;
    delete admin;
    delete room;
}

void testStrategyGetters() {
    FreeUser* free = new FreeUser("Free");
    
    ValidationStrategy* strategy = free->getValidationStrategy();
    
    // Call all getter methods
    std::string name = strategy->getStrategyName();
    int maxLen = strategy->getMaxMessageLength();
    
    std::cout << "Strategy: " << name << ", Max: " << maxLen << std::endl;
    
    delete free;
}

void testBaseCreateIterator() {
    ChatRoom* room = new CtrlCat();
    PremiumUser* user = new PremiumUser("Test");
    room->registerUser(user);
    
    user->send("Test message", room);
    
    // Call the base Aggregate::createIterator() directly
    Iterator* baseIter = room->createIterator();  // No user parameter
    
    if (baseIter) {
        std::cout << "Base iterator created" << std::endl;
        baseIter->first();
        std::cout << baseIter->currentItem() << std::endl;
        delete baseIter;
    }
    
    delete user;
    delete room;
}

// ================== RECEIVE METHOD TEST ==================
void testReceiveMethod() {
    printSeparator("RECEIVE METHOD TEST");
    
    ChatRoom* room = new CtrlCat();
    PremiumUser* sender = new PremiumUser("Sender");
    PremiumUser* receiver = new PremiumUser("Receiver");
    AdminUser* adminReceiver = new AdminUser("AdminReceiver");
    
    room->registerUser(sender);
    room->registerUser(receiver);
    room->registerUser(adminReceiver);
    
    std::cout << "\n--- Testing receive() through sendMessage ---" << std::endl;
    sender->send("Test receive method", room);
    
    delete sender;
    delete receiver;
    delete adminReceiver;
    delete room;
}

// ================== VALIDATION STRATEGY SWITCHING TEST ==================
void testStrategySwitch() {
    printSeparator("STRATEGY SWITCHING TEST");
    
    ChatRoom* room = new CtrlCat();
    FreeUser* user = new FreeUser("SwitchTest");
    room->registerUser(user);
    
    std::cout << "\n--- Original Strategy ---" << std::endl;
    std::cout << "Strategy: " << user->getValidationStrategy()->getStrategyName() << std::endl;
    user->send("Original message", room);
    
    std::cout << "\n--- Switch to Premium Strategy ---" << std::endl;
    user->setValidationStrategy(new PremiumUserValidationStrategy());
    std::cout << "New strategy: " << user->getValidationStrategy()->getStrategyName() << std::endl;
    
    std::string longMsg(200, 'x');
    user->send(longMsg, room);  // Should work now with premium strategy
    
    delete user;
    delete room;
}

// ================== CHAT HISTORY ACCESS TEST ==================
void testChatHistoryAccess() {
    printSeparator("CHAT HISTORY ACCESS TEST");
    
    ChatRoom* room = new CtrlCat();
    FreeUser* free = new FreeUser("FreeHistory");
    PremiumUser* premium = new PremiumUser("PremiumHistory");
    AdminUser* admin = new AdminUser("AdminHistory");
    
    room->registerUser(free);
    room->registerUser(premium);
    room->registerUser(admin);
    
    admin->send("History message 1", room);
    admin->send("History message 2", room);
    
    std::cout << "\n--- Free User Requesting History ---" << std::endl;
    const std::vector<std::string>* freeHistory = room->getChatHistory(free);
    if (!freeHistory) {
        std::cout << "Correctly denied to free user" << std::endl;
    }
    
    std::cout << "\n--- Premium User Requesting History ---" << std::endl;
    const std::vector<std::string>* premiumHistory = room->getChatHistory(premium);
    if (!premiumHistory) {
        std::cout << "Correctly denied to premium user" << std::endl;
    }
    
    std::cout << "\n--- Admin User Requesting History ---" << std::endl;
    const std::vector<std::string>* adminHistory = room->getChatHistory(admin);
    if (adminHistory) {
        std::cout << "Admin got history with " << adminHistory->size() << " messages" << std::endl;
    }
    
    delete free;
    delete premium;
    delete admin;
    delete room;
}
void testSpecialCharacters() {
    printSeparator("SPECIAL CHARACTERS TEST");
    
    ChatRoom* room = new CtrlCat();
    PremiumUser* user = new PremiumUser("SpecialTest");
    room->registerUser(user);
    
    std::cout << "\n--- Numbers ---" << std::endl;
    user->send("12345", room);
    
    std::cout << "\n--- Special chars ---" << std::endl;
    user->send("!@#$%^&*()", room);
    
    std::cout << "\n--- Mixed ---" << std::endl;
    user->send("Test123!@#", room);
    
    std::cout << "\n--- Unicode/Emoji (if supported) ---" << std::endl;
    user->send("Hello world 123", room);
    
    std::cout << "\n--- Newlines and tabs ---" << std::endl;
    user->send("Line1\nLine2\tTabbed", room);
    
    delete user;
    delete room;
}
void testDailyCountGetters() {
    printSeparator("DAILY COUNT GETTERS TEST");
    
    ChatRoom* room = new CtrlCat();
    FreeUser* free = new FreeUser("CountTest");
    room->registerUser(free);
    
    std::cout << "Initial count: " << free->getDailyMessageCount() << std::endl;
    std::cout << "Limit: " << free->getDailyMessageLimit() << std::endl;
    
    free->send("Message 1", room);
    std::cout << "After 1 message: " << free->getDailyMessageCount() << std::endl;
    
    free->send("Message 2", room);
    std::cout << "After 2 messages: " << free->getDailyMessageCount() << std::endl;
    
    free->resetDailyCount();
    std::cout << "After reset: " << free->getDailyMessageCount() << std::endl;
    
    delete free;
    delete room;
}
void testToStringMethods() {
    printSeparator("TO STRING METHODS TEST");
    
    ChatRoom* room1 = new CtrlCat();
    ChatRoom* room2 = new Dogorithm();
    
    FreeUser* free = new FreeUser("FreeString");
    PremiumUser* premium = new PremiumUser("PremiumString");
    AdminUser* admin = new AdminUser("AdminString");
    
    room1->registerUser(free);
    room1->registerUser(premium);
    room2->registerUser(admin);
    
    free->send("Test", room1);
    free->send("Test", room1);
    
    std::cout << "\n--- Free User toString ---" << std::endl;
    std::cout << free->toString();
    
    std::cout << "\n--- Premium User toString ---" << std::endl;
    std::cout << premium->toString();
    
    std::cout << "\n--- Admin User toString ---" << std::endl;
    std::cout << admin->toString();
    
    delete free;
    delete premium;
    delete admin;
    delete room1;
    delete room2;
}


// ================== MAIN FUNCTION ==================
int main() {
    Logger::setLevel(USER_ONLY);
    
    std::cout << "Starting Comprehensive Test Suite..." << std::endl;
    
    testMediatorPattern();
    testCommandPattern();
    testUserHierarchy();
    testIterator();
    testEdgeCases();
    testValidationBoundaries();
    testUserLimits();
    testCommandQueue();
    testCrossRoomFunctionality();
    testConcreteAggregate();
    testPolymorphism();
    testStrategyPattern();

     testMultipleIterators();
    
    testProfanityInWords();
    
    testIsInChatRoom();
    testStrategyGetters();
    testBaseCreateIterator();
    
    testChatHistoryAccess();
    testStrategySwitch();
    testReceiveMethod();

    testDailyCountGetters();
    testToStringMethods();
    std::cout << "\n=== All Tests Complete ===" << std::endl;
    return 0;
}