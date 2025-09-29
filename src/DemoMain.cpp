/**
 * @file main.cpp
 * @brief Interactive demo showcasing PetSpace chat application capabilities
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-29
 */

#include "Users.h"
#include "ChatRoom.h"
#include "CtrlCat.h"
#include "Dogorithm.h"
#include "Logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>

// Global storage for users and chat rooms
std::vector<User*> allUsers;
std::vector<ChatRoom*> allRooms;

// Forward declarations
void clearScreen();
void pauseScreen();
void displayMainMenu();
void createUserMenu();
void sendMessageMenu();
void viewUsersMenu();
void joinRoomMenu();
void leaveRoomMenu();
void viewChatHistoryMenu();
void logLevelMenu();
void resetDailyCountMenu();
void viewUserDetailsMenu();
void cleanup();

int main() {
    // Initialize chat rooms
    CtrlCat* ctrlCatRoom = new CtrlCat();
    Dogorithm* dogorithmRoom = new Dogorithm();
    
    allRooms.push_back(ctrlCatRoom);
    allRooms.push_back(dogorithmRoom);
    
    Logger::setLevel(BASIC); // Start with INFO level
    
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "   Welcome to PetSpace Chat System!    \n";
    std::cout << "========================================\n\n";
    pauseScreen();
    
    int choice = 0;
    
    while (choice != 10) {
        displayMainMenu();
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;
        
        // Handle invalid input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input! Please enter a number.\n";
            pauseScreen();
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                createUserMenu();
                break;
            case 2:
                joinRoomMenu();
                break;
            case 3:
                sendMessageMenu();
                break;
            case 4:
                viewChatHistoryMenu();
                break;
            case 5:
                viewUsersMenu();
                break;
            case 6:
                viewUserDetailsMenu();
                break;
            case 7:
                leaveRoomMenu();
                break;
            case 8:
                resetDailyCountMenu();
                break;
            case 9:
                logLevelMenu();
                break;
            case 10:
                std::cout << "\nThank you for using PetSpace! Goodbye!\n";
                break;
            default:
                std::cout << "\nInvalid choice! Please try again.\n";
                pauseScreen();
        }
    }
    
    cleanup();
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void displayMainMenu() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         PetSpace Main Menu            \n";
    std::cout << "========================================\n";
    std::cout << "1.  Create New User\n";
    std::cout << "2.  Join Chat Room\n";
    std::cout << "3.  Send Message\n";
    std::cout << "4.  View Chat History (Admin Only)\n";
    std::cout << "5.  View All Users\n";
    std::cout << "6.  View User Details\n";
    std::cout << "7.  Leave Chat Room\n";
    std::cout << "8.  Reset Daily Count (Free User)\n";
    std::cout << "9.  Change Log Level\n";
    std::cout << "10. Exit\n";
    std::cout << "========================================\n";
}

void createUserMenu() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         Create New User               \n";
    std::cout << "========================================\n";
    
    std::string userName;
    int userType;
    
    std::cout << "Enter username: ";
    std::getline(std::cin, userName);
    
    if (userName.empty()) {
        std::cout << "\nUsername cannot be empty!\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\nSelect user type:\n";
    std::cout << "1. Free User (10 msgs/day, 100 char limit, no profanity)\n";
    std::cout << "2. Premium User (unlimited, mild profanity allowed)\n";
    std::cout << "3. Admin User (full privileges, can view history)\n";
    std::cout << "\nEnter choice (1-3): ";
    std::cin >> userType;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    User* newUser = nullptr;
    
    switch (userType) {
        case 1:
            newUser = new FreeUser(userName);
            break;
        case 2:
            newUser = new PremiumUser(userName);
            break;
        case 3:
            newUser = new AdminUser(userName);
            break;
        default:
            std::cout << "\nInvalid user type!\n";
            pauseScreen();
            return;
    }
    
    allUsers.push_back(newUser);
    std::cout << "\nUser created successfully!\n";
    pauseScreen();
}

void joinRoomMenu() {
    if (allUsers.empty()) {
        clearScreen();
        std::cout << "No users exist! Please create a user first.\n";
        pauseScreen();
        return;
    }
    
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         Join Chat Room                \n";
    std::cout << "========================================\n";
    
    std::cout << "\nAvailable Users:\n";
    for (size_t i = 0; i < allUsers.size(); i++) {
        std::cout << i + 1 << ". " << allUsers[i]->getName() 
                  << " (" << allUsers[i]->getUserTypeString() << ")\n";
    }
    
    int userChoice;
    std::cout << "\nSelect user (1-" << allUsers.size() << "): ";
    std::cin >> userChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (userChoice < 1 || userChoice > static_cast<int>(allUsers.size())) {
        std::cout << "\nInvalid user selection!\n";
        pauseScreen();
        return;
    }
    
    User* selectedUser = allUsers[userChoice - 1];
    
    std::cout << "\nAvailable Chat Rooms:\n";
    std::cout << "1. CtrlCat\n";
    std::cout << "2. Dogorithm\n";
    
    int roomChoice;
    std::cout << "\nSelect room (1-2): ";
    std::cin >> roomChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (roomChoice == 1) {
        allRooms[0]->registerUser(selectedUser);
    } else if (roomChoice == 2) {
        allRooms[1]->registerUser(selectedUser);
    } else {
        std::cout << "\nInvalid room selection!\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\nUser joined room successfully!\n";
    pauseScreen();
}

void sendMessageMenu() {
    if (allUsers.empty()) {
        clearScreen();
        std::cout << "No users exist! Please create a user first.\n";
        pauseScreen();
        return;
    }
    
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         Send Message                  \n";
    std::cout << "========================================\n";
    
    std::cout << "\nAvailable Users:\n";
    for (size_t i = 0; i < allUsers.size(); i++) {
        std::cout << i + 1 << ". " << allUsers[i]->getName() 
                  << " (" << allUsers[i]->getUserTypeString() << ")\n";
    }
    
    int userChoice;
    std::cout << "\nSelect user (1-" << allUsers.size() << "): ";
    std::cin >> userChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (userChoice < 1 || userChoice > static_cast<int>(allUsers.size())) {
        std::cout << "\nInvalid user selection!\n";
        pauseScreen();
        return;
    }
    
    User* selectedUser = allUsers[userChoice - 1];
    
    std::cout << "\nSelect destination room:\n";
    std::cout << "1. CtrlCat\n";
    std::cout << "2. Dogorithm\n";
    
    int roomChoice;
    std::cout << "\nSelect room (1-2): ";
    std::cin >> roomChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (roomChoice < 1 || roomChoice > 2) {
        std::cout << "\nInvalid room selection!\n";
        pauseScreen();
        return;
    }
    
    ChatRoom* selectedRoom = allRooms[roomChoice - 1];
    
    std::string message;
    std::cout << "\nEnter message: ";
    std::getline(std::cin, message);
    
    bool success = selectedUser->send(message, selectedRoom);
    
    if (!success) {
        std::cout << "\nMessage failed to send! Check validation rules or daily limits.\n";
    }
    
    pauseScreen();
}

void viewChatHistoryMenu() {
    if (allUsers.empty()) {
        clearScreen();
        std::cout << "No users exist! Please create an admin user first.\n";
        pauseScreen();
        return;
    }
    
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      View Chat History (Admin)        \n";
    std::cout << "========================================\n";
    
    std::cout << "\nAvailable Admin Users:\n";
    std::vector<AdminUser*> admins;
    for (size_t i = 0; i < allUsers.size(); i++) {
        if (allUsers[i]->getUserType() == UserType::ADMIN) {
            admins.push_back(dynamic_cast<AdminUser*>(allUsers[i]));
            std::cout << admins.size() << ". " << allUsers[i]->getName() << "\n";
        }
    }
    
    if (admins.empty()) {
        std::cout << "\nNo admin users available! Please create an admin user.\n";
        pauseScreen();
        return;
    }
    
    int adminChoice;
    std::cout << "\nSelect admin (1-" << admins.size() << "): ";
    std::cin >> adminChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (adminChoice < 1 || adminChoice > static_cast<int>(admins.size())) {
        std::cout << "\nInvalid admin selection!\n";
        pauseScreen();
        return;
    }
    
    AdminUser* selectedAdmin = admins[adminChoice - 1];
    
    std::cout << "\nSelect room to view history:\n";
    std::cout << "1. CtrlCat\n";
    std::cout << "2. Dogorithm\n";
    
    int roomChoice;
    std::cout << "\nSelect room (1-2): ";
    std::cin >> roomChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (roomChoice < 1 || roomChoice > 2) {
        std::cout << "\nInvalid room selection!\n";
        pauseScreen();
        return;
    }
    
    ChatRoom* selectedRoom = allRooms[roomChoice - 1];
    
    std::cout << "\n";
    selectedAdmin->iterateChatHistory(selectedRoom);
    
    pauseScreen();
}

void viewUsersMenu() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         All Registered Users          \n";
    std::cout << "========================================\n\n";
    
    if (allUsers.empty()) {
        std::cout << "No users registered yet.\n";
    } else {
        for (size_t i = 0; i < allUsers.size(); i++) {
            std::cout << i + 1 << ". " << allUsers[i]->getName() 
                      << " (" << allUsers[i]->getUserTypeString() << ")\n";
        }
    }
    
    pauseScreen();
}

void viewUserDetailsMenu() {
    if (allUsers.empty()) {
        clearScreen();
        std::cout << "No users exist!\n";
        pauseScreen();
        return;
    }
    
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         View User Details             \n";
    std::cout << "========================================\n";
    
    std::cout << "\nAvailable Users:\n";
    for (size_t i = 0; i < allUsers.size(); i++) {
        std::cout << i + 1 << ". " << allUsers[i]->getName() 
                  << " (" << allUsers[i]->getUserTypeString() << ")\n";
    }
    
    int userChoice;
    std::cout << "\nSelect user (1-" << allUsers.size() << "): ";
    std::cin >> userChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (userChoice < 1 || userChoice > static_cast<int>(allUsers.size())) {
        std::cout << "\nInvalid user selection!\n";
        pauseScreen();
        return;
    }
    
    User* selectedUser = allUsers[userChoice - 1];
    
    std::cout << "\n" << selectedUser->toString() << "\n";
    
    pauseScreen();
}

void leaveRoomMenu() {
    if (allUsers.empty()) {
        clearScreen();
        std::cout << "No users exist!\n";
        pauseScreen();
        return;
    }
    
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         Leave Chat Room               \n";
    std::cout << "========================================\n";
    
    std::cout << "\nAvailable Users:\n";
    for (size_t i = 0; i < allUsers.size(); i++) {
        std::cout << i + 1 << ". " << allUsers[i]->getName() 
                  << " (" << allUsers[i]->getUserTypeString() << ")\n";
    }
    
    int userChoice;
    std::cout << "\nSelect user (1-" << allUsers.size() << "): ";
    std::cin >> userChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (userChoice < 1 || userChoice > static_cast<int>(allUsers.size())) {
        std::cout << "\nInvalid user selection!\n";
        pauseScreen();
        return;
    }
    
    User* selectedUser = allUsers[userChoice - 1];
    
    std::cout << "\nSelect room to leave:\n";
    std::cout << "1. CtrlCat\n";
    std::cout << "2. Dogorithm\n";
    
    int roomChoice;
    std::cout << "\nSelect room (1-2): ";
    std::cin >> roomChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (roomChoice < 1 || roomChoice > 2) {
        std::cout << "\nInvalid room selection!\n";
        pauseScreen();
        return;
    }
    
    ChatRoom* selectedRoom = allRooms[roomChoice - 1];
    selectedRoom->removeUser(selectedUser);
    
    pauseScreen();
}

void resetDailyCountMenu() {
    if (allUsers.empty()) {
        clearScreen();
        std::cout << "No users exist!\n";
        pauseScreen();
        return;
    }
    
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      Reset Daily Count (Free User)    \n";
    std::cout << "========================================\n";
    
    std::cout << "\nAvailable Free Users:\n";
    std::vector<FreeUser*> freeUsers;
    for (size_t i = 0; i < allUsers.size(); i++) {
        if (allUsers[i]->getUserType() == UserType::FREE) {
            freeUsers.push_back(dynamic_cast<FreeUser*>(allUsers[i]));
            std::cout << freeUsers.size() << ". " << allUsers[i]->getName() << "\n";
        }
    }
    
    if (freeUsers.empty()) {
        std::cout << "\nNo free users available!\n";
        pauseScreen();
        return;
    }
    
    int userChoice;
    std::cout << "\nSelect user (1-" << freeUsers.size() << "): ";
    std::cin >> userChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (userChoice < 1 || userChoice > static_cast<int>(freeUsers.size())) {
        std::cout << "\nInvalid user selection!\n";
        pauseScreen();
        return;
    }
    
    FreeUser* selectedUser = freeUsers[userChoice - 1];
    selectedUser->resetDailyCount();
    
    std::cout << "\nDaily count reset successfully!\n";
    pauseScreen();
}

void logLevelMenu() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "         Change Log Level              \n";
    std::cout << "========================================\n";
    std::cout << "\nSelect log level:\n";
    std::cout << "1. DEBUG (Show all messages)\n";
    std::cout << "2. INFO (Show important events)\n";
    std::cout << "3. USER (Show only user-facing messages)\n";
    
    int choice;
    std::cout << "\nEnter choice (1-3): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            Logger::setLevel(DEBUG);
            std::cout << "\nLog level set to DEBUG\n";
            break;
        case 2:
            Logger::setLevel(BASIC);
            std::cout << "\nLog level set to INFO\n";
            break;
        case 3:
            Logger::setLevel(USER_ONLY);
            std::cout << "\nLog level set to USER\n";
            break;
        default:
            std::cout << "\nInvalid choice!\n";
    }
    
    pauseScreen();
}

void cleanup() {
    // Clean up all users
    for (size_t i = 0; i < allUsers.size(); i++) {
        delete allUsers[i];
    }
    allUsers.clear();
    
    // Clean up all rooms
    for (size_t i = 0; i < allRooms.size(); i++) {
        delete allRooms[i];
    }
    allRooms.clear();
}