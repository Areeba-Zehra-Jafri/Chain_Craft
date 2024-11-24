#ifndef RSA_H
#define RSA_H

#include <utility>

class RSA
{
private:
    unsigned long long p, q, n, phi, e, d;

    bool isPrime(unsigned long long num);
    unsigned long long gcd(unsigned long long a, unsigned long long b);
    unsigned long long modInverse(unsigned long long a, unsigned long long m);
    unsigned long long modExp(unsigned long long base, unsigned long long exp, unsigned long long mod);
    std::pair<unsigned long long, unsigned long long> publicKey;

public:
    RSA();

    // Generate public and private keys
    void generateKeys();

    // Get the public key as a pair (e, n)
    std::pair<unsigned long long, unsigned long long> getPublicKey() const;

    // Get the private key as (d, n)
    std::pair<unsigned long long, unsigned long long> getPrivateKey() const;

    // Encrypt a message with the public key
    unsigned long long encrypt(unsigned long long plaintext);

    // Decrypt a message with the private key
    unsigned long long decrypt(unsigned long long ciphertext);

    // Set the private key (d, n)
    void setPrivateKey(unsigned long long privateKey, unsigned long long modulus);

    void set_PublicKey(unsigned long long ex, unsigned long long modulus);

    void setPublicKey(const std::pair<unsigned long long, unsigned long long> &keyPair)
    {
        publicKey = keyPair;
    }
};

#endif // RSA_H
