#ifndef PROOFOFWORK_H
#define PROOFOFWORK_H

#include <openssl/evp.h>    // EVP for cryptographic operations
#include <string>

class ProofOfWork {
public:
    // Constructor that generates the RSA key pair
    ProofOfWork();
    
    // Destructor to free allocated resources
    ~ProofOfWork();

    // Generate public and private keys
    void generateKeys();

    // Sign the data with the private key
    std::string signData(const std::string &data);

    // Verify the signature with the public key
    bool verifyData(const std::string &data, const std::string &signature);

    // Get the public key as a string
    std::string getPublicKey() const;

    // Get the private key as a string
    std::string getPrivateKey() const;

private:
    EVP_PKEY *evpKey;        // EVP_PKEY key pair (replacing RSA *rsa)
    std::string publicKey;   // Public key in PEM format
    std::string privateKey;  // Private key in PEM format
};

#endif // PROOFOFWORK_H
