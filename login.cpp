#include <thread>
#include <chrono>
#include "login.h"
#include "Cblock.h"
#include "CChain.h"
#include <iostream>
#include <conio.h>
#include <fstream>

using namespace std;

// Constructor to initialize the filename
Login::Login(const string& file) : filename(file) {
    cout << "\n--- Loading Blockchain from File ---\n";
    char choice;
    cout << "Do you want to load the blockchain from the file? (y/n): ";
    cin >> choice;
    const string blockchainFile = "blockchain.dat";
    if (choice == 'y' || choice == 'Y') {
        if (myBlockchain.loadFromFile(blockchainFile)) {
            myBlockchain.printChain();
            cout << "Blockchain successfully loaded from file.\n";
        } else {
            cout << "Failed to load blockchain. Starting with a new blockchain.\n";
        }
    } 
    else if (choice == 'n' || choice == 'N') {
        cout << "Starting with a new blockchain.\n";
        myBlockchain = Blockchain();
    } else {
        cout << "Invalid choice. Defaulting to a new blockchain.\n";
    }
    const string walletsFile = "wallets.dat";
    Wallet w = Wallet("temp");
    wallets= w.loadAllFromFile(walletsFile);
    for (auto wallet : wallets) { 
        cout << "Loaded wallet: " << wallet->getId() << endl; // Debug print
         }
    loadUsers();
}
// Destructor to clean up dynamic memory
Login::~Login() {
    for (auto wallet : wallets) {
        delete wallet;
    }
}

// Function to check if the file exists
bool Login::fileExists() {
    ifstream file(filename);
    return file.good();
}

// Function to load users from the file
void Login::loadUsers() {
    if (!fileExists()) return;

    ifstream file(filename);
    string username, password;
    while (file >> username >> password) {
        users[username] = password;
        cout << "Loaded user: " << username << " with password: " << password << endl; // Debug print
    }
}


bool isValidPassword(const string& password) {
    if (password.length() < 8) return false; // Minimum length

    bool hasDigit = false, hasLetter = false, hasSpecialChar = false;
    for (char c : password) {
        if (isdigit(c)) hasDigit = true;
        if (isalpha(c)) hasLetter = true;
        if (ispunct(c)) hasSpecialChar = true;
    }

    return hasDigit && hasLetter && hasSpecialChar;
}

// Function to handle password masking
string getPasswordInput() {
    string password = "";
    char ch;

    while (true) {
        ch = _getch();  // Read a character from the console without displaying it
        if (ch == 13) break;  // Enter key to break the loop
        if (ch == 8 && password.length() > 0) {  // Backspace to remove character
            password.pop_back();
            cout << "\b \b";  // Move back and delete the character
        } else if (ch != 8) {
            password += ch;
            cout << "*";  // Display asterisk for the character
        }
    }

    cout << endl;  // Move to the next line after password input
    return password;
}

// Function to handle signup
void Login::signup() {
    string firstName, lastName, username, password;

    cout << "Enter First Name: ";
    cin >> firstName;
    cout << "Enter Last Name: ";
    cin >> lastName;

    username = firstName + "_" + lastName;

    if (users.find(username) != users.end()) {
        cout << "Error: Username already exists!\n";
        return;
    }

 while (true) {
    cout << "Create a Password (must contain a number, letter, and special character): ";
     password = getPasswordInput();  // Get masked password input

        if (isValidPassword(password)) {
            break;  // If valid, break the loop
        } else {
            cout << "Error: Password must contain at least one number, one letter, and one special character.\n";
        }
    }

    // Save user credentials to the file
    ofstream file(filename, ios::app);
    file << username << " " << password << endl;
    file.close();


    // Create a new wallet and save it to a binary file
    Wallet* newWallet = new Wallet(username);
    ofstream walletFile("wallets.dat", ios::binary | ios::app);
    newWallet->saveToFile(walletFile);
    walletFile.close();

    // Add to map
    users[username] = password;

    cout << "Signup successful! Your username is: " << username << "\n";
}

// Function to handle login
Wallet* Login::login() {
    string username, password;
    int retryCount = 3; 

while (retryCount > 0) {
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    
     password = getPasswordInput(); 

    if (users.find(username) != users.end() && users[username] == password) {
        cout << "Login successful! Welcome, " << username << "!\n";
        ifstream walletFile("wallets.dat", ios::binary);
        Wallet* userWallet = nullptr;

         if (walletFile.is_open()) {
            while (!walletFile.eof()) {
            userWallet = Wallet::loadFromFile(walletFile);
            if (userWallet && userWallet->getId() == username) {
                break;
            }
           delete userWallet; // Free memory if this wallet does not match
                userWallet = nullptr; // Avoid using deleted pointer
            }
            walletFile.close(); // Close the file
        } else {
            cout << "Error: Unable to open wallet file.\n";
        }

        return userWallet; // Return the found wallet

    } else {
        cout << "Error: Invalid username or password.\n";
          retryCount--;  // Decrease the retry count
            if (retryCount > 0) {
                cout << "You have " << retryCount << " attempts remaining.\n";
            }
        }
    }

    cout << "Maximum login attempts exceeded. Please try again later.\n";
    return nullptr;  // Return null if the retry limit is exceeded
}
        


// Function to handle transactions
void Login::performTransaction(Wallet* sender) {

 if (sender->getBalance() == 0) {
     sender->setBalance(100); // Set initial balance to 100 for demonstration purposes
      }

    string receiverName;
    float amount;

    cout << "Enter Receiver's Username: ";
    cin >> receiverName;

if (users.find(receiverName) == users.end()) {
        cout << "Error: Receiver not found! Make sure the receiver has signed up.\n";
        return;
    }

    // Check if receiver exists
    Wallet* receiver = nullptr;
    for (auto wallet : wallets) {
     cout << "Checking wallet: " << wallet->getId() << endl; // Debug print
        if (wallet && wallet->getId() == receiverName) {
            receiver = wallet;
            if (receiver->getBalance() == 0) { 
                receiver->setBalance(0); // Set initial balance to zero
                 }
            break;
        }
    }

    if (!receiver) {
        cout<<"reciever does not exist"<<endl;
        return;
    }

    cout << "Sender Initial Balance: " << sender->getBalance() << endl;
cout << "Receiver Initial Balance: " << receiver->getBalance() << endl;

    cout << "Enter Amount to Send: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Error: Invalid amount entered.\n";
        return;
    }

    if (sender->getBalance() < amount) {
        cout << "Error: Insufficient balance.\n";
        return;
    }


    // Create and add transaction
    Transaction tx = sender->sendFunds(*receiver, amount);
    if (tx.get_sender().empty()) {
        cout << "Transaction failed due to insufficient balance.\n";
        return;
    }
     // Update balances directly after transaction creation
    sender->updateBalance({tx}); // Deduct from sender
    receiver->updateBalance({tx}); // Add to receiver

    cout << "Sender Balance After Transaction: " << sender->getBalance() << endl;
cout << "Receiver Balance After Transaction: " << receiver->getBalance() << endl;

    myBlockchain.createTransaction(tx);
    cout << "Transaction successfully added to the blockchain queue.\n";
}

// Function to mine a block
void Login::mineBlock() {
    cout << "Mining in progress...\n";
     int duration = 5;  // seconds
    auto start = chrono::high_resolution_clock::now(); // Get current time
    auto end = start;

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < duration) {
        // Simulate work by keeping the loop running for 5 seconds
        end = chrono::high_resolution_clock::now(); // Update end time
    }

    myBlockchain.minePendingTransactions();

      myBlockchain.notifyWallets(wallets);

    cout << "Mining successful! A new block has been added to the blockchain.\n";

}

// Function to view the blockchain
void Login::viewChain() {
    cout << "Blockchain Details:\n";
    myBlockchain.printChain();

    cout << "Wallet Balances:\n";
    for (const auto& wallet : wallets) {
        wallet->printWalletData();
    }

    // Check validity
    if (myBlockchain.isChainValid()) {
        cout << "The blockchain is valid.\n";
    } else {
        cout << "Warning: The blockchain is invalid!\n";
    }
}

// Function to display the user menu after login
void Login::userMenu(Wallet* userWallet) {
    while (true) {
        cout << "-----------------------------------\n";
        cout << "1. Perform Transaction\n2. Mine a Block\n3. View Blockchain\n4. Logout\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            performTransaction(userWallet);
        } else if (choice == 2) {
            mineBlock();
        } else if (choice == 3) {
            viewChain();
        } else if (choice == 4) {
            cout << "Logging out...\n";
            break;
        } else {
            cout << "Invalid choice, try again.\n";
        }
    }
}

// Function to display the main menu
void Login::displayMenu() {
    Wallet* userWallet = nullptr;

    while (true) {
        cout << "-----------------------------------\n";
        cout << "Welcome to Blockchain Simulation!\n";
        cout << "1. Login\n2. Signup\n3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            userWallet = login();
            if (userWallet) {
                userMenu(userWallet); // After login, show user-specific menu
            }
        } else if (choice == 2) {
            signup();
        } else if (choice == 3) {
            const string walletsFile = "wallets.dat";
            userWallet->saveAllToFile(wallets,walletsFile);
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice, try again.\n";
        }
    }
}
