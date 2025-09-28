/**
 * @file demoMain.cpp
 * @brief Interactive demonstration of PetSpace Chat System
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-28
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>

#include "Users.h"
#include "ChatRoom.h"
#include "CtrlCat.h"
#include "Dogorithm.h"
#include "Iterator.h"

class PetSpaceDemo {
private:
    std::map<std::string, User*> users;
    std::map<std::string, ChatRoom*> chatRooms;
    User* currentUser;
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void pause() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    
    void printHeader() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "           WELCOME TO PETSPACE CHAT SYSTEM           " << std::endl;
        std::cout << "    Demonstrating Mediator, Command & Iterator Patterns    " << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    void printCurrentUser() {
        if (currentUser) {
            std::cout << "\n[Current User: " << currentUser->getName() 
                      << " (" << currentUser->getUserTypeString() << ")]" << std::endl;
        }
    }
    
public:
    PetSpaceDemo() : currentUser(nullptr) {
        // Initialize available chat rooms
        chatRooms["CtrlCat"] = new CtrlCat();
        chatRooms["Dogorithm"] = new Dogorithm();
    }
    
    ~PetSpaceDemo() {
        // Clean up users
        for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it) {
            delete it->second;
        }
        
        // Clean up chat rooms
        for (std::map<std::string, ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); ++it) {
            delete it->second;
        }
    }
    
    void run() {
        bool running = true;
        
        while (running) {
            clearScreen();
            printHeader();
            printCurrentUser();
            
            if (!currentUser) {
                running = handleMainMenu();
            } else {
                running = handleUserMenu();
            }
        }
        
        std::cout << "\nThank you for using PetSpace Chat System!" << std::endl;
        std::cout << "Goodbye!" << std::endl;
    }
    
private:
    bool handleMainMenu() {
        std::cout << "\n--- MAIN MENU ---" << std::endl;
        std::cout << "1. Create New User" << std::endl;
        std::cout << "2. Login as Existing User" << std::endl;
        std::cout << "3. View System Status" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "\nEnter your choice (1-4): ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear input buffer
        
        switch (choice) {
            case 1:
                createUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                viewSystemStatus();
                pause();
                break;
            case 4:
                return false;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                pause();
                break;
        }
        
        return true;
    }
    
    bool handleUserMenu() {
        std::cout << "\n--- USER MENU ---" << std::endl;
        std::cout << "1. Join Chat Room" << std::endl;
        std::cout << "2. Send Message" << std::endl;
        std::cout << "3. View Available Rooms" << std::endl;
        
        // User type specific options
        if (currentUser->getUserType() == UserType::FREE) {
            FreeUser* freeUser = static_cast<FreeUser*>(currentUser);
            std::cout << "4. Check Message Limit Status" << std::endl;
            std::cout << "5. Reset Daily Message Count (Simulate New Day)" << std::endl;
        }
        
        if (currentUser->getUserType() == UserType::ADMIN) {
            std::cout << "6. Admin: Browse Chat History" << std::endl;
            std::cout << "7. Admin: Manual Iterator Control" << std::endl;
        }
        
        std::cout << "8. Switch User" << std::endl;
        std::cout << "9. Exit" << std::endl;
        std::cout << "\nEnter your choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                joinChatRoom();
                break;
            case 2:
                sendMessage();
                break;
            case 3:
                viewAvailableRooms();
                break;
            case 4:
                if (currentUser->getUserType() == UserType::FREE) {
                    checkMessageStatus();
                } else {
                    std::cout << "Invalid choice." << std::endl;
                    pause();
                }
                break;
            case 5:
                if (currentUser->getUserType() == UserType::FREE) {
                    resetDailyMessages();
                } else {
                    std::cout << "Invalid choice." << std::endl;
                    pause();
                }
                break;
            case 6:
                if (currentUser->getUserType() == UserType::ADMIN) {
                    adminBrowseHistory();
                } else {
                    std::cout << "Invalid choice." << std::endl;
                    pause();
                }
                break;
            case 7:
                if (currentUser->getUserType() == UserType::ADMIN) {
                    adminManualIterator();
                } else {
                    std::cout << "Invalid choice." << std::endl;
                    pause();
                }
                break;
            case 8:
                currentUser = nullptr;
                break;
            case 9:
                return false;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                pause();
                break;
        }
        
        return true;
    }
    
    void createUser() {
        std::cout << "\n--- CREATE NEW USER ---" << std::endl;
        std::cout << "Enter username: ";
        std::string username;
        std::getline(std::cin, username);
        
        if (users.find(username) != users.end()) {
            std::cout << "Username already exists! Please choose a different name." << std::endl;
            pause();
            return;
        }
        
        std::cout << "Choose user type:" << std::endl;
        std::cout << "1. Free User (10 messages per day)" << std::endl;
        std::cout << "2. Premium User (unlimited messages)" << std::endl;
        std::cout << "3. Admin User (unlimited messages + admin privileges)" << std::endl;
        std::cout << "Enter choice (1-3): ";
        
        int userType;
        std::cin >> userType;
        std::cin.ignore();
        
        User* newUser = nullptr;
        
        switch (userType) {
            case 1:
                newUser = new FreeUser(username);
                break;
            case 2:
                newUser = new PremiumUser(username);
                break;
            case 3:
                newUser = new AdminUser(username);
                break;
            default:
                std::cout << "Invalid choice. Creating Free User by default." << std::endl;
                newUser = new FreeUser(username);
                break;
        }
        
        users[username] = newUser;
        currentUser = newUser;
        
        std::cout << "\nUser created successfully!" << std::endl;
        pause();
    }
    
    void loginUser() {
        if (users.empty()) {
            std::cout << "\nNo users exist. Please create a user first." << std::endl;
            pause();
            return;
        }
        
        std::cout << "\n--- LOGIN ---" << std::endl;
        std::cout << "Available users:" << std::endl;
        
        int index = 1;
        std::vector<std::string> usernames;
        for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it) {
            std::cout << index << ". " << it->first 
                      << " (" << it->second->getUserTypeString() << ")" << std::endl;
            usernames.push_back(it->first);
            index++;
        }
        
        std::cout << "Enter user number: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice >= 1 && choice <= static_cast<int>(usernames.size())) {
            currentUser = users[usernames[choice - 1]];
            std::cout << "Logged in as " << currentUser->getName() << std::endl;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
        
        pause();
    }
    
    void joinChatRoom() {
        std::cout << "\n--- JOIN CHAT ROOM ---" << std::endl;
        std::cout << "Available chat rooms:" << std::endl;
        
        int index = 1;
        std::vector<std::string> roomNames;
        for (std::map<std::string, ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); ++it) {
            std::cout << index << ". " << it->first << std::endl;
            roomNames.push_back(it->first);
            index++;
        }
        
        std::cout << "Enter room number: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice >= 1 && choice <= static_cast<int>(roomNames.size())) {
            std::string roomName = roomNames[choice - 1];
            ChatRoom* room = chatRooms[roomName];
            room->registerUser(currentUser);
            std::cout << "Successfully joined " << roomName << "!" << std::endl;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
        
        pause();
    }
    
    void sendMessage() {
        std::cout << "\n--- SEND MESSAGE ---" << std::endl;
        std::cout << "Available chat rooms:" << std::endl;
        
        int index = 1;
        std::vector<std::string> roomNames;
        for (std::map<std::string, ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); ++it) {
            std::cout << index << ". " << it->first << std::endl;
            roomNames.push_back(it->first);
            index++;
        }
        
        std::cout << "Enter room number: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice >= 1 && choice <= static_cast<int>(roomNames.size())) {
            std::string roomName = roomNames[choice - 1];
            ChatRoom* room = chatRooms[roomName];
            
            std::cout << "Enter your message: ";
            std::string message;
            std::getline(std::cin, message);
            
            bool success = currentUser->send(message, room);
            if (!success) {
                std::cout << "\nMessage not sent. Check your daily limit!" << std::endl;
            }
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
        
        pause();
    }
    
    void viewAvailableRooms() {
        std::cout << "\n--- AVAILABLE CHAT ROOMS ---" << std::endl;
        for (std::map<std::string, ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); ++it) {
            std::cout << "- " << it->first << std::endl;
        }
        pause();
    }
    
    void checkMessageStatus() {
        if (currentUser->getUserType() == UserType::FREE) {
            FreeUser* freeUser = static_cast<FreeUser*>(currentUser);
            std::cout << "\n--- MESSAGE STATUS ---" << std::endl;
            std::cout << "Messages used today: " << freeUser->getDailyMessageCount() 
                      << "/" << freeUser->getDailyMessageLimit() << std::endl;
            std::cout << "Remaining messages: " 
                      << (freeUser->getDailyMessageLimit() - freeUser->getDailyMessageCount()) << std::endl;
        }
        pause();
    }
    
    void resetDailyMessages() {
        if (currentUser->getUserType() == UserType::FREE) {
            FreeUser* freeUser = static_cast<FreeUser*>(currentUser);
            freeUser->resetDailyCount();
            std::cout << "Daily message count reset! You now have " 
                      << freeUser->getDailyMessageLimit() << " messages available." << std::endl;
        }
        pause();
    }
    
    void adminBrowseHistory() {
        if (currentUser->getUserType() != UserType::ADMIN) {
            std::cout << "Access denied. Admin privileges required." << std::endl;
            pause();
            return;
        }
        
        AdminUser* adminUser = static_cast<AdminUser*>(currentUser);
        
        std::cout << "\n--- ADMIN: BROWSE CHAT HISTORY ---" << std::endl;
        std::cout << "Select chat room to browse:" << std::endl;
        
        int index = 1;
        std::vector<std::string> roomNames;
        for (std::map<std::string, ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); ++it) {
            std::cout << index << ". " << it->first << std::endl;
            roomNames.push_back(it->first);
            index++;
        }
        
        std::cout << "Enter room number: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice >= 1 && choice <= static_cast<int>(roomNames.size())) {
            std::string roomName = roomNames[choice - 1];
            ChatRoom* room = chatRooms[roomName];
            
            std::cout << "\nBrowsing chat history for " << roomName << ":" << std::endl;
            adminUser->iterateChatHistory(room);
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
        
        pause();
    }
    
    void adminManualIterator() {
        if (currentUser->getUserType() != UserType::ADMIN) {
            std::cout << "Access denied. Admin privileges required." << std::endl;
            pause();
            return;
        }
        
        std::cout << "\n--- ADMIN: MANUAL ITERATOR CONTROL ---" << std::endl;
        std::cout << "Select chat room:" << std::endl;
        
        int index = 1;
        std::vector<std::string> roomNames;
        for (std::map<std::string, ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); ++it) {
            std::cout << index << ". " << it->first << std::endl;
            roomNames.push_back(it->first);
            index++;
        }
        
        std::cout << "Enter room number: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice >= 1 && choice <= static_cast<int>(roomNames.size())) {
            std::string roomName = roomNames[choice - 1];
            ChatRoom* room = chatRooms[roomName];
            
            Iterator* iterator = currentUser->requestChatHistoryIterator(room);
            
            if (iterator) {
                manualIteratorControl(iterator, roomName);
                delete iterator;
            } else {
                std::cout << "Failed to create iterator." << std::endl;
                pause();
            }
        } else {
            std::cout << "Invalid choice." << std::endl;
            pause();
        }
    }
    
    void manualIteratorControl(Iterator* iterator, const std::string& roomName) {
        bool controlling = true;
        
        std::cout << "\n--- MANUAL ITERATOR CONTROL FOR " << roomName << " ---" << std::endl;
        std::cout << "Iterator created. Starting at first element." << std::endl;
        iterator->first();
        
        while (controlling) {
            std::cout << "\n--- Iterator Control Menu ---" << std::endl;
            std::cout << "Current status: " << (iterator->isDone() ? "DONE" : "ACTIVE") << std::endl;
            
            if (!iterator->isDone()) {
                std::cout << "Current message: \"" << iterator->currentItem() << "\"" << std::endl;
            }
            
            std::cout << "\n1. Next" << std::endl;
            std::cout << "2. First (Reset)" << std::endl;
            std::cout << "3. Check if Done" << std::endl;
            std::cout << "4. Current Item" << std::endl;
            std::cout << "5. Exit Iterator Control" << std::endl;
            std::cout << "Enter choice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                case 1:
                    iterator->next();
                    if (iterator->isDone()) {
                        std::cout << "Reached end of chat history." << std::endl;
                    }
                    break;
                case 2:
                    iterator->first();
                    std::cout << "Iterator reset to first element." << std::endl;
                    break;
                case 3:
                    std::cout << "Iterator status: " << (iterator->isDone() ? "DONE" : "ACTIVE") << std::endl;
                    break;
                case 4:
                    if (!iterator->isDone()) {
                        std::cout << "Current item: \"" << iterator->currentItem() << "\"" << std::endl;
                    } else {
                        std::cout << "No current item - iterator is done." << std::endl;
                    }
                    break;
                case 5:
                    controlling = false;
                    break;
                default:
                    std::cout << "Invalid choice." << std::endl;
                    break;
            }
            
            if (controlling && choice != 5) {
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
            }
        }
    }
    
    void viewSystemStatus() {
        std::cout << "\n--- SYSTEM STATUS ---" << std::endl;
        
        std::cout << "\nRegistered Users (" << users.size() << "):" << std::endl;
        for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it) {
            std::cout << "- " << it->first << " (" << it->second->getUserTypeString() << ")";
            if (it->second->getUserType() == UserType::FREE) {
                FreeUser* freeUser = static_cast<FreeUser*>(it->second);
                std::cout << " [" << freeUser->getDailyMessageCount() 
                          << "/" << freeUser->getDailyMessageLimit() << " messages]";
            }
            std::cout << std::endl;
        }
        
        std::cout << "\nAvailable Chat Rooms (" << chatRooms.size() << "):" << std::endl;
        for (std::map<std::string, ChatRoom*>::iterator it = chatRooms.begin(); it != chatRooms.end(); ++it) {
            std::cout << "- " << it->first << std::endl;
        }
    }
};

int main() {
    PetSpaceDemo demo;
    demo.run();
    return 0;
}