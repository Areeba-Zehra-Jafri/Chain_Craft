#include "Transaction.h"
#include "SHA256.h"
#include "RSA.h"
#include <sstream>
#include <iomanip>
#include <functional>
#include<iostream>

using namespace std;

// Constructor to initialize a Transaction
Transaction::Transaction(const std::string &sender, const std::string &receiver, double amount)
    : sender(sender), receiver(receiver), amount(amount)
{
    hash = calculateHash();
}

// Method to get the transaction data as a string
std::string Transaction::getTransactionData() const
{
    std::ostringstream ss;
    ss << sender << receiver << std::fixed << std::setprecision(2) << amount;
    return ss.str();
}

// Calculate the SHA-256 hash of the transaction data
std::string Transaction::calculateHash() const
{
    return SHA256::hash(getTransactionData());
}

// Sign the transaction using the RSA class
unsigned long long Transaction::signTransaction(RSA &rsa)
{
    cout << "Calling Sign transaction function: " << endl;
    // std::pair<unsigned long long, unsigned long long> private_key;
    // private_key = rsa.getPrivateKey();
    // cout << "Private key of sender is: " << private_key.first << " " << private_key.second << endl;
    // std::pair<unsigned long long, unsigned long long> public_key;
    // public_key = rsa.getPublicKey();
    // cout << "Public key of sender is: " << public_key.first << " " << public_key.second << endl;
    // Hash the transaction data to a numeric value for signing
    //     std::hash<std::string> str_hash;
    //     unsigned long long dataNumeric = str_hash(getTransactionData());
    //    // signature = rsa.encrypt(dataNumeric); // Store the encrypted signature
    //     signature=dataNumeric;
    //     //cout<<"Signature after encryption: "<<signature<<endl;
    //     return signature;

    std::string transactionData = getTransactionData();
    std::string hashValue = SHA256::hash(transactionData);  // Assuming SHA256::hash() returns the hash as a string
    unsigned long long dataNumeric = std::stoull(hashValue.substr(0, 16), nullptr, 16); // Take the first 16 characters of the hash 
    //unsigned long long encrypted_data=rsa.encrypt(dataNumeric);
    //cout<<"Encrypted Data using RSA: "<<encrypted_data<<endl;
    signature=dataNumeric;
    //signature=encrypted_data;
    return signature;
}



bool Transaction::verifySignature(RSA &rsa) const
{
    cout << "Calling verify signature: " << endl;
    // std::pair<unsigned long long, unsigned long long> private_key;
    // private_key = rsa.getPrivateKey();
    // cout << "Private key of sender is: " << private_key.first << " " << private_key.second << endl;
    // std::pair<unsigned long long, unsigned long long> public_key;
    // public_key = rsa.getPublicKey();
    // cout << "Public key of sender is: " << public_key.first << " " << public_key.second << endl;
    // unsigned long long decryptedData = rsa.decrypt(signature);
   //-- unsigned long long expectedDataNumeric = std::hash<std::string>{}(getTransactionData());
    // cout<<"DECRYPTED Data: "<<decryptedData<<endl;
    //  cout<<"expected  Data numeric : "<<expectedDataNumeric<<endl;
    //  unsigned long long decryptedData = rsa.decrypt(expectedDataNumeric);
    //  cout<<"Decrypting signature: "<<decryptedData<<endl;


    // cout<<"Signature: "<<signature<<endl;
    //--return signature == expectedDataNumeric;
   // unsigned long long decrypted_data=rsa.decrypt(signature);

    std::string transactionData = getTransactionData();
    std::string hashValue = SHA256::hash(transactionData);  // Assuming SHA256::hash() returns the hash as a string
    unsigned long long dataNumeric = std::stoull(hashValue.substr(0, 16), nullptr, 16);
    //cout<<"Value of encrypted signature: "<<signature<<endl;
    //cout<<"Value of decrypted data: "<<decrypted_data<<endl;
    //cout<<"Value of data numeric: "<<dataNumeric<<endl;

    return signature==dataNumeric;
}
// New method to check if the transaction is valid
bool Transaction::isValid(RSA &publicKey)
{
    // Verify the signature using the public key
    return verifySignature(publicKey);
}

// Getters for sender, receiver, and amount
std::string Transaction::get_sender() const

{
    return sender;
}

std::string Transaction::get_receiver() const
{
    return receiver;
}

double Transaction::get_amount() const
{
    return amount;
}

unsigned long long Transaction::getSignature() const
{
    return signature;
}
