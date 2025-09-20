/**
 * @file Dogorithm.h
 * @brief Defines the Dogorithm chat room
 * @author Megan Azmanov & Kyle McCalgan
 * @date 2025-09-19
 */

#ifndef DOGORITHM_H
#define DOGORITHM_H

#include "ChatRoom.h"

/**
 * @class Dogorithm
 * @brief Concrete mediator, dog themed chat room 
 */
class Dogorithm : public ChatRoom {
public:
    /**
     * @brief Register a user with Dogorithm room
     * @param user Pointer to user to register
     */
    void registerUser(User* user) override;
    
    /**
     * @brief Remove a user from Dogorithm room
     * @param user Pointer to user to remove
     */
    void removeUser(User* user) override;
};

#endif