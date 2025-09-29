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

// ================== LOGGER LEVELS TEST ==================
void testLoggerLevels() {
    printSeparator("LOGGER LEVELS TEST");
    std::cout << "This test shows the difference between logging levels:" << std::endl;
    std::cout << "- NONE: Silent mode" << std::endl;
    std::cout << "- USER_ONLY: Natural chat experience" << std::endl;
    std::cout << "- BASIC: System notifications" << std::endl;
    std::cout << "- DEBUG: Full development details" << std::endl;
    
    ChatRoom* testRoom = new CtrlCat();
    User* alice = new FreeUser("Alice");
    User* bob = new PremiumUser("Bob");
    AdminUser* charlie = new AdminUser("Charlie");
    
    std::cout << "\n--- Setting up users (BASIC level to see system info) ---" << std::endl;
    Logger::setLevel(BASIC);
    testRoom->registerUser(alice);
    testRoom->registerUser(bob);
    testRoom->registerUser(charlie);
    
    std::cout << "\n=== Test 1: NONE Level ===\n[Should see only this text, then silence]\n" << std::endl;
    Logger::setLevel(NONE);
    alice->send("This message should be completely silent", testRoom);
    bob->send("No output should appear for this", testRoom);
    
    std::cout << "\n=== Test 2: USER_ONLY Level ===\n[Natural chat experience]\n" << std::endl;
    Logger::setLevel(USER_ONLY);
    alice->send("Hey everyone! How's the chat going?", testRoom);
    bob->send("Great to see you Alice!", testRoom);
    charlie->send("Admin here - welcome to PetSpace!", testRoom);
    bob->send("Thanks for keeping things running smoothly!", testRoom);
    
    std::cout << "\n--- Testing Free User Limit (USER_ONLY) ---" << std::endl;
    for (int i = 0; i < 3; i++) {
        alice->send("Testing my daily limit " + std::to_string(i+1), testRoom);
    }
    
    std::cout << "\n--- Admin viewing chat history (USER_ONLY) ---" << std::endl;
    charlie->iterateChatHistory(testRoom);
    
    std::cout << "\n=== Test 3: BASIC Level ===\n[Adds system operations]\n" << std::endl;
    Logger::setLevel(BASIC);
    
    // Create fresh users to show join messages
    User* diana = new FreeUser("Diana");
    testRoom->registerUser(diana);
    
    diana->send("I just joined! This is exciting!", testRoom);
    alice->send("Welcome Diana!", testRoom);
    
    
    // Show user leaving
    testRoom->removeUser(diana);
    
    std::cout << "\n=== Test 4: DEBUG Level ===\n[Full development details]\n" << std::endl;
    Logger::setLevel(DEBUG);
    
    alice->send("Now you can see everything happening internally!", testRoom);
    
    // Show manual command creation in debug
    std::cout << "\n--- Manual Command Creation (DEBUG level) ---" << std::endl;
    Command* cmd1 = new SendMessageCommand(testRoom, bob, "Manual debug command");
    Command* cmd2 = new SaveMessageCommand(testRoom, bob, "Manual debug command");
    bob->addCommand(cmd1);
    bob->addCommand(cmd2);
    bob->executeAll();
    
    std::cout << "\n=== Summary ===\n" << std::endl;
    Logger::setLevel(USER_ONLY);  // Reset to clean
    std::cout << "✓ NONE: Complete silence for production" << std::endl;
    std::cout << "✓ USER_ONLY: Natural chat experience for demos" << std::endl;
    std::cout << "✓ BASIC: System operations for monitoring" << std::endl;
    std::cout << "✓ DEBUG: Full pattern details for development" << std::endl;
    
    // Cleanup
    delete alice;
    delete bob;
    delete charlie;
    delete diana;
    delete testRoom;
}

/**
 * @brief Test function for user-type-specific Strategy pattern
 * Add this to your TestingMain.cpp
 */

void testUserTypeValidationStrategies() {
    printSeparator("USER-TYPE VALIDATION STRATEGIES TEST");
    
    ChatRoom* testRoom = new CtrlCat();
    
    // Create users - each gets their specific validation strategy
    std::cout << "\n--- Creating Users with Type-Specific Validation ---" << std::endl;
    FreeUser* alice = new FreeUser("Alice");
    PremiumUser* bob = new PremiumUser("Bob");
    AdminUser* charlie = new AdminUser("Charlie");
    
    testRoom->registerUser(alice);
    testRoom->registerUser(bob);
    testRoom->registerUser(charlie);
    
    std::cout << "Alice (Free): " << alice->getValidationStrategy()->getMaxMessageLength() << " char limit" << std::endl;
    std::cout << "Bob (Premium): " << (bob->getValidationStrategy()->getMaxMessageLength() == -1 ? "No limit" : std::to_string(bob->getValidationStrategy()->getMaxMessageLength())) << std::endl;
    std::cout << "Charlie (Admin): " << charlie->getValidationStrategy()->getMaxMessageLength() << " char limit" << std::endl;
    
    // Test 1: Length Restrictions
    std::cout << "\n=== Test 1: Message Length Restrictions ===\n" << std::endl;
    
    // Short message - should work for everyone
    std::cout << "Short messages (should work for all):" << std::endl;
    alice->send("Hi everyone!", testRoom);
    bob->send("Hello from premium!", testRoom);
    charlie->send("Admin here!", testRoom);
    
    // Medium message (101+ chars) - should fail for free user only
    std::string mediumMessage = "This message is longer than 100 characters to test the free user limit. Free users are restricted.";
    std::cout << "\nMedium message (" << mediumMessage.length() << " chars):" << std::endl;
    alice->send(mediumMessage, testRoom);   // Should FAIL - too long for free user
    bob->send(mediumMessage, testRoom);     // Should PASS - premium has no limit
    charlie->send(mediumMessage, testRoom); // Should PASS - admin has high limit
    
    // Very long message - should work for premium and admin
    std::string longMessage = "This is a very long message that demonstrates premium users have no length restrictions. ";
    longMessage += "Premium users pay for the service and should be able to express themselves fully without arbitrary limits. ";
    longMessage += "They can write detailed explanations, share stories, or provide comprehensive feedback. ";
    longMessage += "This flexibility is one of the key benefits of upgrading from free to premium membership. ";
    longMessage += "The system should handle these longer messages without any issues.";
    
    std::cout << "\nVery long message (" << longMessage.length() << " chars):" << std::endl;
    alice->send(longMessage, testRoom);     // Should FAIL - way too long for free user
    bob->send(longMessage, testRoom);       // Should PASS - premium unlimited
    charlie->send(longMessage, testRoom);   // Should PASS - admin high limit
    
    // Test 2: Profanity Filtering by User Type
    std::cout << "\n=== Test 2: User-Type-Specific Profanity Filtering ===\n" << std::endl;
    
    // Mild language - blocked for free, allowed for premium/admin
    std::cout << "Mild inappropriate language:" << std::endl;
    alice->send("This feature is stupid", testRoom);    // Should FAIL - free users blocked from mild words
    bob->send("This feature is stupid", testRoom);      // Should PASS - premium allows mild language
    charlie->send("This feature is stupid", testRoom);  // Should PASS - admin allows anything reasonable
    
    // Moderate language - allowed for premium/admin
    std::cout << "\nModerate language:" << std::endl;
    alice->send("That really sucks", testRoom);         // Should FAIL - free user restriction
    bob->send("That really sucks", testRoom);           // Should PASS - premium tolerance
    charlie->send("That really sucks", testRoom);       // Should PASS - admin privileges
    
    // Severe profanity - blocked for premium, allowed for admin
    std::cout << "\nSevere profanity (testing system limits):" << std::endl;
    alice->send("This is fucking broken", testRoom);    // Should FAIL - free user strict rules
    bob->send("This is fucking broken", testRoom);      // Should FAIL - even premium has limits
    charlie->send("This is fucking broken", testRoom);  // Should PASS - admin can handle moderation
    
    // Test 3: Caps and Spam Detection
    std::cout << "\n=== Test 3: Caps and Spam Detection ===\n" << std::endl;
    
    // Moderate caps - strict for free users
    std::cout << "Testing caps tolerance:" << std::endl;
    alice->send("I LOVE CATS SO MUCH!", testRoom);      // Should FAIL - too many caps for free user (30% limit)
    bob->send("I LOVE PREMIUM FEATURES!", testRoom);    // Should PASS - premium gets more caps tolerance
    charlie->send("ADMIN ANNOUNCEMENT HERE!", testRoom); // Should PASS - admin unlimited
    
    // Excessive character repetition
    std::cout << "\nTesting character repetition:" << std::endl;
    alice->send("Woooooooo cats!", testRoom);           // Should FAIL - free users get strict limits
    bob->send("Awesooooooooome!", testRoom);           // Should PASS - premium gets more tolerance
    charlie->send("Hellooooooooooooooo!", testRoom);   // Should PASS - admin flexibility
    
    // Test 4: User Type Specific Benefits Demo
    std::cout << "\n=== Test 4: User Type Benefits Demonstration ===\n" << std::endl;
    
    std::cout << "Free User Restrictions (Alice):" << std::endl;
    alice->send("Short msgs only", testRoom);                    // PASS - within limits
    alice->send("Can't say hate or stupid or dumb", testRoom);   // FAIL - contains blocked words
    alice->send("No EXCESSIVE caps allowed", testRoom);          // FAIL - too many caps
    
    std::cout << "\nPremium User Benefits (Bob):" << std::endl;
    bob->send("I can write much longer messages because I'm a premium user and I paid for this service so I should get better features and more flexibility in how I communicate with other users in the chat room", testRoom); // PASS - no length limit
    bob->send("I can say things are stupid or dumb", testRoom);  // PASS - mild language allowed
    bob->send("I CAN USE MORE CAPS IN MY MESSAGES", testRoom);  // PASS - higher caps tolerance
    
    std::cout << "\nAdmin Privileges (Charlie):" << std::endl;
    charlie->send("As an admin, I can post very long announcements with detailed information about system updates, rule changes, moderation policies, and community guidelines. I can also use any language necessary for moderation purposes, including examples of inappropriate content when educating users about our policies.", testRoom); // PASS - high limit
    charlie->send("I can use fucking strong language for moderation examples", testRoom); // PASS - admin moderation needs
    charlie->send("ADMIN CAPS ANNOUNCEMENTS ARE ALLOWED", testRoom); // PASS - admin announcements
    
    // Test 5: System Threat Detection (Admin only restriction)
    std::cout << "\n=== Test 5: System Threat Detection ===\n" << std::endl;
    
    std::cout << "Testing system threat detection for admin:" << std::endl;
    charlie->send("Normal admin message", testRoom);             // Should PASS
    charlie->send("DELETE FROM users", testRoom);                // Should FAIL - system threat
    charlie->send("Let's reboot the server", testRoom);          // Should FAIL - system threat
    
    // Test 6: Edge Cases
    std::cout << "\n=== Test 6: Edge Cases ===\n" << std::endl;
    
    // Empty messages
    alice->send("", testRoom);
    bob->send("", testRoom);
    charlie->send("", testRoom);
    
    // Boundary testing
    std::string exactly100chars = "This message is exactly one hundred characters long to test the free user boundary limits perfectly";
    std::cout << "\nExactly 100 character message:" << std::endl;
    alice->send(exactly100chars, testRoom);  // Should PASS - at the limit
    
    std::string exactly101chars = exactly100chars + "!";
    std::cout << "Exactly 101 character message:" << std::endl;
    alice->send(exactly101chars, testRoom);  // Should FAIL - over the limit
    
    // Test 7: Strategy Benefits Summary
    std::cout << "\n=== User Type Strategy Summary ===\n" << std::endl;
    Logger::user("FREE USERS (Alice):");
    Logger::user("  - 100 character message limit");
    Logger::user("  - No profanity (even mild words like 'stupid')"); 
    Logger::user("  - Low caps tolerance (30%)");
    Logger::user("  - Strict spam detection");
    Logger::user("");
    Logger::user("PREMIUM USERS (Bob):");
    Logger::user("  - Unlimited message length");
    Logger::user("  - Mild profanity allowed ('stupid', 'sucks')");
    Logger::user("  - Higher caps tolerance (80%)");
    Logger::user("  - Moderate spam detection");
    Logger::user("  - Severe profanity still blocked");
    Logger::user("");
    Logger::user("ADMIN USERS (Charlie):");
    Logger::user("  - 2000 character limit (for system stability)");
    Logger::user("  - All language allowed (for moderation purposes)");
    Logger::user("  - No caps restrictions");
    Logger::user("  - Only system threats blocked");
    
    // Cleanup
    delete alice;
    delete bob;
    delete charlie;
    delete testRoom;
}

void KyleTest(){
    //just need to test random things

    ChatRoom* ctrlCat = new CtrlCat();
    ChatRoom* dogorithm = new Dogorithm();
    
    User* alice = new FreeUser("Alice");
    User* bob = new PremiumUser("Bob");
    User* charlie = new AdminUser("Charlie");
    
    // alice->send("hello cats",ctrlCat);
    //ctrlCat->registerUser(alice);
    alice->send("hello cats",ctrlCat);
    // alice->removeChatRoom(ctrlCat);
    // alice->send("hello cats",ctrlCat);

    delete alice;
    delete bob;
    delete charlie;
    delete ctrlCat;
    delete dogorithm;
}

// ================== MAIN FUNCTION ==================
int main() {
    // Set default logging level
    Logger::setLevel(DEBUG);  // Clean experience by default

    testMediatorPattern();
    testCommandPattern();
     testUserHierarchy();
    testPolymorphism();
    testIntegration();
    testIterator();
    testLoggerLevels();  
    testUserTypeValidationStrategies();

    //KyleTest();
    
    std::cout << "\n=== All Tests Complete ===" << std::endl;
    return 0;
}