/**
 * @file DemoMain.cpp
 * @brief Interactive demo showcasing all design patterns in the chat system
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#include "Users.h"
#include "ChatRoom.h"
#include "CtrlCat.h"
#include "Dogorithm.h"
#include "Command.h"
#include "SendMessageCommand.h"
#include "SaveMessageCommand.h"
#include "Iterator.h"
#include "Logger.h"

class ChatSystemDemo {
private:
    std::vector<ChatRoom*> rooms;
    std::vector<User*> users;
    User* currentUser;
    
public:
    ChatSystemDemo() : currentUser(nullptr) {
        // Initialize chat rooms
        rooms.push_back(new CtrlCat());
        rooms.push_back(new Dogorithm());
        
        // Set initial logging level for clean demo experience
        Logger::setLevel(USER_ONLY);
        
        std::cout << "=== Welcome to PetSpace Chat System Demo ===" << std::endl;
        std::cout << "Showcasing: Mediator, Command, Iterator Patterns & User Hierarchy" << std::endl;
    }
    
    ~ChatSystemDemo() {
        // Cleanup
        for (User* user : users) {
            delete user;
        }
        for (ChatRoom* room : rooms) {
            delete room;
        }
    }
    
    void pause() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    void printSeparator(const std::string& title = "") {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        if (!title.empty()) {
            std::cout << title << std::endl;
            std::cout << std::string(60, '=') << std::endl;
        }
    }
    
    void run() {
        while (true) {
            showMainMenu();
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1: userManagement(); break;
                case 2: roomManagement(); break;
                case 3: sendMessages(); break;
                case 4: adminFeatures(); break;
                case 5: commandPatternDemo(); break;
                case 6: systemSettings(); break;
                case 7: viewSystemStatus(); break;
                case 0: 
                    std::cout << "Thanks for using PetSpace Chat Demo!" << std::endl;
                    return;
                default: 
                    std::cout << "Invalid choice! Please try again." << std::endl;
                    pause();
                    break;
            }
        }
    }
    
private:
    void showMainMenu() {
        printSeparator("MAIN MENU");
        std::cout << "Current User: " << (currentUser ? currentUser->getName() + " (" + currentUser->getUserTypeString() + ")" : "None") << std::endl;
        std::cout << "\n1. User Management" << std::endl;
        std::cout << "2. Join/Leave Chat Rooms" << std::endl;
        std::cout << "3. Send Messages" << std::endl;
        std::cout << "4. Admin Features (Iterator Pattern)" << std::endl;
        std::cout << "5. Command Pattern Demo" << std::endl;
        std::cout << "6. System Settings" << std::endl;
        std::cout << "7. View System Status" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "\nChoice: ";
    }
    
    void userManagement() {
        while (true) {
            printSeparator("USER MANAGEMENT");
            std::cout << "Current User: " << (currentUser ? currentUser->getName() + " (" + currentUser->getUserTypeString() + ")" : "None") << std::endl;
            std::cout << "\n1. Register New User" << std::endl;
            std::cout << "2. Switch User" << std::endl;
            std::cout << "3. View All Users" << std::endl;
            std::cout << "4. Delete User" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "\nChoice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1: registerUser(); break;
                case 2: switchUser(); break;
                case 3: viewAllUsers(); break;
                case 4: deleteUser(); break;
                case 0: return;
                default: std::cout << "Invalid choice!" << std::endl; pause(); break;
            }
        }
    }
    
    void registerUser() {
        printSeparator("REGISTER NEW USER");
        
        std::cout << "Enter username: ";
        std::string username;
        std::getline(std::cin, username);
        
        // Check if username already exists
        for (User* user : users) {
            if (user->getName() == username) {
                std::cout << "Username already exists!" << std::endl;
                pause();
                return;
            }
        }
        
        std::cout << "\nSelect user type:" << std::endl;
        std::cout << "1. Free User (10 messages/day limit)" << std::endl;
        std::cout << "2. Premium User (unlimited messages)" << std::endl;
        std::cout << "3. Admin User (unlimited + admin privileges)" << std::endl;
        std::cout << "Choice: ";
        
        int type;
        std::cin >> type;
        std::cin.ignore();
        
        User* newUser = nullptr;
        switch (type) {
            case 1:
                newUser = new FreeUser(username);
                std::cout << "\nFree user created! Daily limit: " << dynamic_cast<FreeUser*>(newUser)->getDailyMessageLimit() << " messages" << std::endl;
                break;
            case 2:
                newUser = new PremiumUser(username);
                std::cout << "\nPremium user created! Unlimited messaging!" << std::endl;
                break;
            case 3:
                newUser = new AdminUser(username);
                std::cout << "\nAdmin user created! Full system access!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice! Creating Free user by default." << std::endl;
                newUser = new FreeUser(username);
                break;
        }
        
        users.push_back(newUser);
        
        if (!currentUser) {
            currentUser = newUser;
            std::cout << "Set as current user." << std::endl;
        }
        
        pause();
    }
    
    void switchUser() {
        if (users.empty()) {
            std::cout << "No users registered!" << std::endl;
            pause();
            return;
        }
        
        printSeparator("SWITCH USER");
        std::cout << "Select user:" << std::endl;
        
        for (size_t i = 0; i < users.size(); i++) {
            std::cout << (i + 1) << ". " << users[i]->getName() << " (" << users[i]->getUserTypeString() << ")" << std::endl;
        }
        
        std::cout << "Choice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice > 0 && choice <= static_cast<int>(users.size())) {
            currentUser = users[choice - 1];
            std::cout << "Switched to: " << currentUser->getName() << std::endl;
        } else {
            std::cout << "Invalid choice!" << std::endl;
        }
        
        pause();
    }
    
    void viewAllUsers() {
        printSeparator("ALL REGISTERED USERS");
        
        if (users.empty()) {
            std::cout << "No users registered." << std::endl;
        } else {
            for (User* user : users) {
                std::cout << "• " << user->getName() << " (" << user->getUserTypeString() << ")";
                if (user == currentUser) std::cout << " [CURRENT]";
                std::cout << std::endl;
                
                // Show daily message count for free users
                if (user->getUserType() == UserType::FREE) {
                    FreeUser* freeUser = dynamic_cast<FreeUser*>(user);
                    std::cout << "  Messages today: " << freeUser->getDailyMessageCount() << "/" << freeUser->getDailyMessageLimit() << std::endl;
                }
            }
        }
        
        pause();
    }
    
    void deleteUser() {
        if (users.empty()) {
            std::cout << "No users to delete!" << std::endl;
            pause();
            return;
        }
        
        printSeparator("DELETE USER");
        viewAllUsers();
        
        std::cout << "\nEnter username to delete (or 'cancel'): ";
        std::string username;
        std::getline(std::cin, username);
        
        if (username == "cancel") return;
        
        auto it = std::find_if(users.begin(), users.end(), 
                              [username](User* user) { return user->getName() == username; });
        
        if (it != users.end()) {
            User* userToDelete = *it;
            
            // Remove from all rooms first
            for (ChatRoom* room : rooms) {
                room->removeUser(userToDelete);
            }
            
            // Update current user if necessary
            if (currentUser == userToDelete) {
                currentUser = users.size() > 1 ? (users[0] == userToDelete ? users[1] : users[0]) : nullptr;
            }
            
            users.erase(it);
            delete userToDelete;
            
            std::cout << "User deleted successfully!" << std::endl;
        } else {
            std::cout << "User not found!" << std::endl;
        }
        
        pause();
    }
    
    void roomManagement() {
        if (!currentUser) {
            std::cout << "Please register/select a user first!" << std::endl;
            pause();
            return;
        }
        
        while (true) {
            printSeparator("ROOM MANAGEMENT");
            std::cout << "Current User: " << currentUser->getName() << " (" << currentUser->getUserTypeString() << ")" << std::endl;
            
            std::cout << "\n1. Join Chat Room" << std::endl;
            std::cout << "2. Leave Chat Room" << std::endl;
            std::cout << "3. View Available Rooms" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "\nChoice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1: joinRoom(); break;
                case 2: leaveRoom(); break;
                case 3: viewAllRooms(); break;
                case 0: return;
                default: std::cout << "Invalid choice!" << std::endl; pause(); break;
            }
        }
    }
    
    void joinRoom() {
        std::cout << "\nAvailable rooms:" << std::endl;
        std::cout << "1. CtrlCat (For cat lovers!)" << std::endl;
        std::cout << "2. Dogorithm (For dog enthusiasts!)" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1 || choice == 2) {
            ChatRoom* room = rooms[choice - 1];
            room->registerUser(currentUser);
            std::string roomName = (choice == 1) ? "CtrlCat" : "Dogorithm";
            std::cout << "Successfully joined " << roomName << "!" << std::endl;
        } else {
            std::cout << "Invalid choice!" << std::endl;
        }
        
        pause();
    }
    
    void leaveRoom() {
        std::cout << "\nSelect room to leave:" << std::endl;
        std::cout << "1. CtrlCat" << std::endl;
        std::cout << "2. Dogorithm" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1 || choice == 2) {
            ChatRoom* room = rooms[choice - 1];
            room->removeUser(currentUser);
            std::string roomName = (choice == 1) ? "CtrlCat" : "Dogorithm";
            std::cout << "Left " << roomName << "!" << std::endl;
        } else {
            std::cout << "Invalid choice!" << std::endl;
        }
        
        pause();
    }
    
    void viewAllRooms() {
        printSeparator("ALL CHAT ROOMS");
        std::cout << "1. CtrlCat - The place for cat lovers to share stories and photos" << std::endl;
        std::cout << "2. Dogorithm - Dog enthusiasts unite! Training tips and cute pics" << std::endl;
        pause();
    }
    
    void sendMessages() {
        if (!currentUser) {
            std::cout << "Please register/select a user first!" << std::endl;
            pause();
            return;
        }
        
        while (true) {
            printSeparator("SEND MESSAGES");
            std::cout << "Current User: " << currentUser->getName() << " (" << currentUser->getUserTypeString() << ")" << std::endl;
            
            // Show daily limit for free users
            if (currentUser->getUserType() == UserType::FREE) {
                FreeUser* freeUser = dynamic_cast<FreeUser*>(currentUser);
                std::cout << "Daily Messages: " << freeUser->getDailyMessageCount() << "/" << freeUser->getDailyMessageLimit() << std::endl;
            }
            
            std::cout << "\nSelect room to send message:" << std::endl;
            std::cout << "1. CtrlCat" << std::endl;
            std::cout << "2. Dogorithm" << std::endl;
            std::cout << "3. Send to both rooms" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "\nChoice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            if (choice == 0) return;
            
            if (choice >= 1 && choice <= 3) {
                std::cout << "\nEnter your message: ";
                std::string message;
                std::getline(std::cin, message);
                
                if (message.empty()) {
                    std::cout << "Empty message!" << std::endl;
                    pause();
                    continue;
                }
                
                std::cout << "\n--- MESSAGE SENT ---" << std::endl;
                
                if (choice == 1) {
                    rooms[0]->registerUser(currentUser);
                    currentUser->send(message, rooms[0]);
                } else if (choice == 2) {
                    rooms[1]->registerUser(currentUser);
                    currentUser->send(message, rooms[1]);
                } else if (choice == 3) {
                    rooms[0]->registerUser(currentUser);
                    rooms[1]->registerUser(currentUser);
                    currentUser->send(message, rooms[0]);
                    currentUser->send(message, rooms[1]);
                }
                
                pause();
            } else {
                std::cout << "Invalid choice!" << std::endl;
                pause();
            }
        }
    }
    
    void adminFeatures() {
        if (!currentUser) {
            std::cout << "Please register/select a user first!" << std::endl;
            pause();
            return;
        }
        
        if (currentUser->getUserType() != UserType::ADMIN) {
            std::cout << "Access Denied: Admin privileges required!" << std::endl;
            std::cout << "Current user (" << currentUser->getName() << ") is " << currentUser->getUserTypeString() << std::endl;
            pause();
            return;
        }
        
        while (true) {
            printSeparator("ADMIN FEATURES - Iterator Pattern Demo");
            std::cout << "Admin: " << currentUser->getName() << std::endl;
            
            std::cout << "\n1. View CtrlCat Chat History" << std::endl;
            std::cout << "2. View Dogorithm Chat History" << std::endl;
            std::cout << "3. Manual Iterator Control Demo" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "\nChoice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1: viewChatHistory(0); break;
                case 2: viewChatHistory(1); break;
                case 3: manualIteratorDemo(); break;
                case 0: return;
                default: std::cout << "Invalid choice!" << std::endl; pause(); break;
            }
        }
    }
    
    void viewChatHistory(int roomIndex) {
        ChatRoom* room = rooms[roomIndex];
        std::string roomName = (roomIndex == 0) ? "CtrlCat" : "Dogorithm";
        
        printSeparator("CHAT HISTORY - " + roomName);
        std::cout << "Using Iterator Pattern to traverse chat history...\n" << std::endl;
        
        currentUser->iterateChatHistory(room);
        pause();
    }
    
    void manualIteratorDemo() {
        printSeparator("MANUAL ITERATOR CONTROL DEMO");
        
        std::cout << "Select room for iterator demo:" << std::endl;
        std::cout << "1. CtrlCat" << std::endl;
        std::cout << "2. Dogorithm" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice != 1 && choice != 2) {
            std::cout << "Invalid choice!" << std::endl;
            pause();
            return;
        }
        
        ChatRoom* room = rooms[choice - 1];
        std::string roomName = (choice == 1) ? "CtrlCat" : "Dogorithm";
        
        Iterator* iterator = currentUser->requestChatHistoryIterator(room);
        
        if (!iterator) {
            std::cout << "No iterator available or access denied!" << std::endl;
            pause();
            return;
        }
        
        std::cout << "\n=== Manual Iterator Operations for " << roomName << " ===" << std::endl;
        
        while (true) {
            std::cout << "\nIterator Commands:" << std::endl;
            std::cout << "1. first() - Go to beginning" << std::endl;
            std::cout << "2. next() - Move to next item" << std::endl;
            std::cout << "3. currentItem() - View current message" << std::endl;
            std::cout << "4. isDone() - Check if at end" << std::endl;
            std::cout << "5. Show all remaining messages" << std::endl;
            std::cout << "0. Exit iterator demo" << std::endl;
            std::cout << "Choice: ";
            
            int cmd;
            std::cin >> cmd;
            std::cin.ignore();
            
            switch (cmd) {
                case 1:
                    iterator->first();
                    std::cout << "Iterator reset to first position." << std::endl;
                    break;
                case 2:
                    if (!iterator->isDone()) {
                        iterator->next();
                        std::cout << "Moved to next position." << std::endl;
                    } else {
                        std::cout << "Already at end - cannot move next!" << std::endl;
                    }
                    break;
                case 3: {
                    std::string current = iterator->currentItem();
                    if (!current.empty()) {
                        std::cout << "Current: \"" << current << "\"" << std::endl;
                    } else {
                        std::cout << "No current item (at end or empty collection)" << std::endl;
                    }
                    break;
                }
                case 4:
                    std::cout << "isDone(): " << (iterator->isDone() ? "true (at end)" : "false (more items)") << std::endl;
                    break;
                case 5:
                    std::cout << "\nShowing all remaining messages:" << std::endl;
                    while (!iterator->isDone()) {
                        std::cout << "• " << iterator->currentItem() << std::endl;
                        iterator->next();
                    }
                    std::cout << "Reached end of messages." << std::endl;
                    break;
                case 0:
                    delete iterator;
                    return;
                default:
                    std::cout << "Invalid command!" << std::endl;
                    break;
            }
        }
    }
    
    void commandPatternDemo() {
        if (!currentUser) {
            std::cout << "Please register/select a user first!" << std::endl;
            pause();
            return;
        }
        
        while (true) {
            printSeparator("COMMAND PATTERN DEMO");
            std::cout << "Current User: " << currentUser->getName() << std::endl;
            
            std::cout << "\n1. Manual Command Creation & Queuing" << std::endl;
            std::cout << "2. Batch Command Execution" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "\nChoice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1: manualCommandDemo(); break;
                case 2: batchCommandDemo(); break;
                case 0: return;
                default: std::cout << "Invalid choice!" << std::endl; pause(); break;
            }
        }
    }
    
    void manualCommandDemo() {
        printSeparator("MANUAL COMMAND CREATION");
        
        std::cout << "Select room for commands:" << std::endl;
        std::cout << "1. CtrlCat" << std::endl;
        std::cout << "2. Dogorithm" << std::endl;
        std::cout << "Choice: ";
        
        int roomChoice;
        std::cin >> roomChoice;
        std::cin.ignore();
        
        if (roomChoice != 1 && roomChoice != 2) {
            std::cout << "Invalid choice!" << std::endl;
            pause();
            return;
        }
        
        ChatRoom* room = rooms[roomChoice - 1];
        room->registerUser(currentUser);
        
        std::cout << "\nEnter message for manual command: ";
        std::string message;
        std::getline(std::cin, message);
        
        std::cout << "\nCreating commands manually..." << std::endl;
        
        // Create commands manually
        Command* sendCmd = new SendMessageCommand(room, currentUser, message);
        Command* saveCmd = new SaveMessageCommand(room, currentUser, message);
        
        std::cout << "Commands created! Adding to user's command queue..." << std::endl;
        
        currentUser->addCommand(sendCmd);
        currentUser->addCommand(saveCmd);
        
        std::cout << "Commands queued. Execute now? (y/n): ";
        char execute;
        std::cin >> execute;
        std::cin.ignore();
        
        if (execute == 'y' || execute == 'Y') {
            std::cout << "\nExecuting all queued commands:" << std::endl;
            currentUser->executeAll();
        }
        
        pause();
    }
    
    void batchCommandDemo() {
        printSeparator("BATCH COMMAND EXECUTION");
        
        ChatRoom* room = rooms[0];  // Use CtrlCat
        room->registerUser(currentUser);
        
        std::cout << "How many messages do you want to create commands for? ";
        int numMessages;
        std::cin >> numMessages;
        std::cin.ignore();
        
        if (numMessages <= 0 || numMessages > 10) {
            std::cout << "Invalid number! Using 3 messages." << std::endl;
            numMessages = 3;
        }
        
        for (int i = 1; i <= numMessages; i++) {
            std::cout << "Enter message #" << i << ": ";
            std::string message;
            std::getline(std::cin, message);
            
            Command* sendCmd = new SendMessageCommand(room, currentUser, message);
            Command* saveCmd = new SaveMessageCommand(room, currentUser, message);
            
            currentUser->addCommand(sendCmd);
            currentUser->addCommand(saveCmd);
        }
        
        std::cout << "Created " << (numMessages * 2) << " commands (send + save pairs)" << std::endl;
        std::cout << "Executing all at once...\n" << std::endl;
        
        currentUser->executeAll();
        
        pause();
    }
    
    void systemSettings() {
        while (true) {
            printSeparator("SYSTEM SETTINGS");
            
            std::cout << "Current Logger Level: ";
            LogLevel current = Logger::getLevel();
            switch (current) {
                case NONE: std::cout << "NONE (Silent)"; break;
                case USER_ONLY: std::cout << "USER_ONLY (Clean chat)"; break;
                case BASIC: std::cout << "BASIC (System info)"; break;
                case DEBUG: std::cout << "DEBUG (Full details)"; break;
            }
            std::cout << std::endl;
            
            std::cout << "\n1. Set Logging Level" << std::endl;
            std::cout << "2. Reset User Daily Counts" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "\nChoice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1: setLoggingLevel(); break;
                case 2: resetDailyCounts(); break;
                case 0: return;
                default: std::cout << "Invalid choice!" << std::endl; pause(); break;
            }
        }
    }
    
    void setLoggingLevel() {
        printSeparator("SET LOGGING LEVEL");
        
        std::cout << "Select logging level:" << std::endl;
        std::cout << "1. NONE - Complete silence (production mode)" << std::endl;
        std::cout << "2. USER_ONLY - Clean chat experience (demo mode)" << std::endl;
        std::cout << "3. BASIC - System operations visible" << std::endl;
        std::cout << "4. DEBUG - Full pattern implementation details" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: Logger::setLevel(NONE); break;
            case 2: Logger::setLevel(USER_ONLY); break;
            case 3: Logger::setLevel(BASIC); break;
            case 4: Logger::setLevel(DEBUG); break;
            default: std::cout << "Invalid choice!" << std::endl; pause(); return;
        }
        
        std::cout << "Logging level updated!" << std::endl;
        pause();
    }
    
    void resetDailyCounts() {
        std::cout << "Resetting daily message counts for all free users..." << std::endl;
        
        int resetCount = 0;
        for (User* user : users) {
            if (user->getUserType() == UserType::FREE) {
                FreeUser* freeUser = dynamic_cast<FreeUser*>(user);
                freeUser->resetDailyCount();
                resetCount++;
            }
        }
        
        std::cout << "Reset daily counts for " << resetCount << " free users." << std::endl;
        pause();
    }
    
    void viewSystemStatus() {
        printSeparator("SYSTEM STATUS");
        
        std::cout << "=== Chat Rooms ===" << std::endl;
        std::cout << "• CtrlCat (Cat lovers)" << std::endl;
        std::cout << "• Dogorithm (Dog enthusiasts)" << std::endl;
        
        std::cout << "\n=== Registered Users ===" << std::endl;
        if (users.empty()) {
            std::cout << "No users registered." << std::endl;
        } else {
            int freeCount = 0, premiumCount = 0, adminCount = 0;
            for (User* user : users) {
                switch (user->getUserType()) {
                    case UserType::FREE: freeCount++; break;
                    case UserType::PREMIUM: premiumCount++; break;
                    case UserType::ADMIN: adminCount++; break;
                }
            }
            std::cout << "• Free Users: " << freeCount << std::endl;
            std::cout << "• Premium Users: " << premiumCount << std::endl;
            std::cout << "• Admin Users: " << adminCount << std::endl;
            std::cout << "• Total Users: " << users.size() << std::endl;
        }
        
        std::cout << "\n=== Current User ===" << std::endl;
        if (currentUser) {
            std::cout << "• Name: " << currentUser->getName() << std::endl;
            std::cout << "• Type: " << currentUser->getUserTypeString() << std::endl;
            if (currentUser->getUserType() == UserType::FREE) {
                FreeUser* freeUser = dynamic_cast<FreeUser*>(currentUser);
                std::cout << "• Daily Messages: " << freeUser->getDailyMessageCount() 
                         << "/" << freeUser->getDailyMessageLimit() << std::endl;
            }
        } else {
            std::cout << "No current user selected." << std::endl;
        }
        
        std::cout << "\n=== Design Patterns Active ===" << std::endl;
        std::cout << "• Mediator Pattern: ChatRoom classes coordinate communication" << std::endl;
        std::cout << "• Command Pattern: SendMessageCommand & SaveMessageCommand" << std::endl;
        std::cout << "• Iterator Pattern: Admin access to chat history" << std::endl;
        std::cout << "• Strategy/Template: User hierarchy (Free, Premium, Admin)" << std::endl;
        
        pause();
    }
};

int main() {
    ChatSystemDemo demo;
    demo.run();
    return 0;
}