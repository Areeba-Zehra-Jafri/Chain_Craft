#include <iostream>
#include "Transaction.h" 
#include "TransactionHandler.h"
#include "MiningHandler.h"
#include "wallet.h"
#include "RSA.h"
#include "login.h"  // Include the login functionality
#include <string>
#include <thread>
#include <chrono>

int main() {
    // Create RSA instance for signing and verifying transactions
    RSA rsa;
    rsa.generateKeys(); // Generate RSA keys for the user

    // Create a Login instance
    Login loginSystem;
    std::string username, password;
    int choice;

    // Login or Register loop
    bool loggedIn = false;
    while (!loggedIn) {
        std::cout << "Welcome to the Blockchain System!\n";
        std::cout << "Please choose an option:\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        std::cin.ignore();  // Clear the input buffer before getting strings

        std::cout << "Enter username: ";
        std::cin >> username;
        
        // Get hidden password input
        password = loginSystem.getHiddenPassword();

        if (choice == 1) {
            // Register the user
            if (loginSystem.registerUser(username, password)) {
                std::cout << "Registration successful!\n";
                loggedIn = true;  // Automatically log in after registration
            } else {
                std::cout << "Registration failed. Username might already exist.\n";
            }
        } else if (choice == 2) {
            // Attempt login
            loggedIn = loginSystem.loginUser(username, password);
            if (!loggedIn) {
                std::cout << "Login failed! Please try again.\n";
            }
        } else {
            std::cout << "Invalid option. Please try again.\n";
        }
    }

    // Once logged in, show options
    int option;
    std::cout << "\nLogin successful! \n Choose an option:\n";
    std::cout << "1. Perform a Transaction\n";
    std::cout << "2. Start Mining\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice: ";
    std::cin >> option;

    if (option == 1) {
        // Perform a transaction
        std::string sender, receiver;
        double amount;

        std::cout << "Enter your username (Sender): ";
        std::cin >> sender;
        std::cout << "Enter the recipient's username (Receiver): ";
        std::cin >> receiver;
        std::cout << "Enter the amount to send: ";
        std::cin >> amount;

        // Create a transaction and sign it
        Transaction tx = TransactionHandler::createTransaction(sender, receiver, amount, rsa);
        TransactionHandler::displayTransactionDetails(tx);

        // Verify the transaction signature
        if (TransactionHandler::verifyTransaction(tx, rsa)) {
            std::cout << "Transaction Verified!\n";
        } else {
            std::cout << "Transaction verification failed.\n";
        }
    } else if (option == 2) {
        // Start mining
        std::string minerId;
        std::cout << "Enter your miner ID: ";
        std::cin >> minerId;

        bool success = MiningHandler::mineBlock(minerId);
        MiningHandler::displayMiningResult(minerId, success);
    } else if (option == 0) {
        std::cout << "Exiting the system...\n";
    } else {
        std::cout << "Invalid option. Please try again.\n";
    }

    return 0;
}
