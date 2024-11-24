#include "Wallet.h"
#include <iostream>      // For std::cout
#include "RSA.h"         // For the custom RSA class
#include "Transaction.h" // For the Transaction class
#include "CBlock.h"
#include <chrono>
#include<random>
#include <fstream>
// Constructor to initialize a Wallet with a given ID
Wallet::Wallet(const std::string &id) : id(id), balance(0.0f)
{
    generateKeys(); // Generate RSA keys upon wallet creation
}

Wallet::Wallet(const std::string &id, const std::pair<long long, long long> &pubKey,
               const std::pair<long long, long long> &privKey, float initialBalance)
    : id(id), publicKey(pubKey), privateKey(privKey), balance(initialBalance) {}
    // No key generation here

// Destructor
Wallet::~Wallet()
{
    // Destructor logic (no special cleanup needed for std::string keys)
}

// Method to generate RSA key pair using the custom RSA class
void Wallet::generateKeys()
{
    RSA rsa;
    rsa.generateKeys(); // Generates the key pair

    // Retrieve and store the public and private keys as pairs
    publicKey = rsa.getPublicKey();
    privateKey = rsa.getPrivateKey();

    // Debug output to verify key generation (optional)
    // std::cout << "Public Key: (" << publicKey.first << ", " << publicKey.second << ")" << std::endl;
    // std::cout << "Private Key: (" << privateKey.first << ", " << privateKey.second << ")" << std::endl;
}

// Method to send funds to another wallet, signing the transaction with the private key
Transaction Wallet::sendFunds(Wallet &receiver, float amount)
{
    if (this->getBalance() >= amount)
    {
        // Generate a nonce (can be randomized or incremented as needed)
        //int nonce = 12345; // Replace with a more dynamic method if desired

         // Generate a dynamic nonce using a random number generator
        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 gen(rd()); // Seed the generator
        std::uniform_int_distribution<> distr(10000, 99999); // Define the range
        int nonce = distr(gen);

        // Create a transaction object with sender, receiver, amount, and nonce
        Transaction tx(id, receiver.getId(), amount);

        // Sign the transaction with the private key
        RSA rsa;
        rsa.setPrivateKey(privateKey.first, privateKey.second); // Set private key for signing
        unsigned long long signature = tx.signTransaction(rsa);
        std::cout << "Transaction Signature: " << signature << std::endl;

        return tx;
    } // Return the signed transaction
    else
    {
        // If the balance is insufficient, print an error message
        std::cout << "Insufficient balance!" << std::endl;
        // You may also choose to return an empty transaction or handle this case differently
        return Transaction("", "", 0.0f); // Return an invalid or empty transaction
    }
}

// Method to update balance based on the blockchain transactions
void Wallet::updateBalance(const std::vector<Transaction> &transactions)
{
    for (const auto &tx : transactions)
    {
        if (tx.get_sender() == id)
        {
            balance -= tx.get_amount(); // Deduct balance for sent funds
        }
        if (tx.get_receiver() == id)
        {
            balance += tx.get_amount(); // Add balance for received funds
        }
    }
}

// Method to print wallet details
void Wallet::printWalletData() const
{
    std::cout << "Wallet ID: " << id << std::endl;
    std::cout << "Balance: " << balance << std::endl;
    // std::cout << "Public Key: (" << publicKey.first << ", " << publicKey.second << ")" << std::endl;
    //  Note: Avoid logging sensitive private key information
}

// Getter for Wallet ID
std::string Wallet::getId() const
{
    return id;
}

// Getter for Wallet balance
float Wallet::getBalance() const
{
    return balance;
}
void Wallet::setBalance(float newBalance)
{
    balance = newBalance;
}

std::pair<unsigned long long, unsigned long long> Wallet::getPublicKey() const {
    return publicKey;  // Assuming publicKey is a member variable of Wallet
}
// Save wallet data to binary file
void Wallet::saveToFile(std::ofstream &outFile) const {
    size_t idLength = id.size();
    outFile.write(reinterpret_cast<const char*>(&idLength), sizeof(idLength));
    outFile.write(id.c_str(), idLength);
    
    outFile.write(reinterpret_cast<const char*>(&publicKey.first), sizeof(publicKey.first));
    outFile.write(reinterpret_cast<const char*>(&publicKey.second), sizeof(publicKey.second));
    
    outFile.write(reinterpret_cast<const char*>(&privateKey.first), sizeof(privateKey.first));
    outFile.write(reinterpret_cast<const char*>(&privateKey.second), sizeof(privateKey.second));
    
    outFile.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
}

// Static method to load wallet from binary file
Wallet* Wallet::loadFromFile(std::ifstream &inFile) {
    size_t idLength;
    inFile.read(reinterpret_cast<char*>(&idLength), sizeof(idLength));
    
    char* idBuffer = new char[idLength + 1];
    inFile.read(idBuffer, idLength);
    idBuffer[idLength] = '\0';
    
    std::string id(idBuffer);
    delete[] idBuffer;
    
    std::pair<long long, long long> pubKey, privKey;
    inFile.read(reinterpret_cast<char*>(&pubKey.first), sizeof(pubKey.first));
    inFile.read(reinterpret_cast<char*>(&pubKey.second), sizeof(pubKey.second));
    
    inFile.read(reinterpret_cast<char*>(&privKey.first), sizeof(privKey.first));
    inFile.read(reinterpret_cast<char*>(&privKey.second), sizeof(privKey.second));
    
    float balance;
    inFile.read(reinterpret_cast<char*>(&balance), sizeof(balance));
    
    return new Wallet(id, pubKey, privKey, balance);

  
}