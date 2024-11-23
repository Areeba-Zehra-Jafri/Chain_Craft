#include "login.h"
#include "SHA256.h"  // Assuming SHA256.h is implemented elsewhere in your project
#include <iostream>
#include <conio.h>  // For getch()

// Mock database to store users
std::unordered_map<std::string, std::string> Login::usersDB;

// Hash the password using SHA-256
std::string Login::hashPassword(const std::string& password) {
    return SHA256::hash(password);
}

// Register a new user
bool Login::registerUser(const std::string& username, const std::string& password) {
    if (usersDB.find(username) != usersDB.end()) {
        std::cout << "Username already exists. Please choose a different username.\n";
        return false;
    }
    usersDB[username] = hashPassword(password);
    return true; // Return true if registration is successful
}

// Login function
bool Login::loginUser(const std::string& username, const std::string& password) {
    auto it = usersDB.find(username);
    if (it != usersDB.end() && it->second == hashPassword(password)) {
        std::cout << "Login successful!\n";
        return true;
    }
    std::cout << "Invalid username or password.\n";
    return false;
}

// Utility function to get hidden password input
std::string Login::getHiddenPassword() {
    std::string password;
    char ch;
    std::cout << "Enter password: ";
    while ((ch = getch()) != '\r') { // '\r' is the Enter key
        if (ch == '\b') { // Handle backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; // Erase character from console
            }
        } else {
            password.push_back(ch);
            std::cout << '*'; // Print '*' instead of the actual character
        }
    }
    std::cout << std::endl;
    return password;
}
