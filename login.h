#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <unordered_map>

class Login {
private:
    // Mock database to store users' hashed passwords
    static std::unordered_map<std::string, std::string> usersDB;

    // Hash function for passwords
    static std::string hashPassword(const std::string& password);

public:
    // Register a new user
    static bool registerUser(const std::string& username, const std::string& password);

    // Login existing user
    static bool loginUser(const std::string& username, const std::string& password);

    // Utility function to get hidden password input (password masking)
    static std::string getHiddenPassword();
};

#endif // LOGIN_H
