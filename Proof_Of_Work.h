#ifndef PROOFOFWORK_H
#define PROOFOFWORK_H

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <string>

using namespace std;

class ProofOfWork {
public:
    ProofOfWork();
    ~ProofOfWork();

    // Generate public and private keys
    void generateKeys();

    // Sign the data with the private key
    string signData(const string &data);

    // Verify the signature with the public key
    bool verifyData(const string &data, const string &signature);

    // Get the public key as a string
    string getPublicKey() const;

    // Get the private key as a string
    string getPrivateKey() const;

private:
    RSA *rsa;                   // RSA key pair
    string publicKey;      // Public key string
    string privateKey;     // Private key string
};

#endif // PROOFOFWORK_H
