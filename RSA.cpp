#include "RSA.h"
#include <random>
#include <stdexcept>
#include <iostream>

RSA::RSA() {
    generateKeys();
}

bool RSA::isPrime(unsigned long long num) {
    if (num < 2) return false;
    for (unsigned long long i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

unsigned long long RSA::gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

unsigned long long RSA::modInverse(unsigned long long a, unsigned long long m) {
    unsigned long long m0 = m, x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        unsigned long long q = a / m;
        unsigned long long t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    return (x1 + m0) % m0;
}

unsigned long long RSA::modExp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

void RSA::generateKeys() {
    // Small prime numbers for p and q, use larger values in real-world applications
    p = 61;
    q = 53;
    n = p * q;
    phi = (p - 1) * (q - 1);

    // Choose e, a coprime of phi and 1 < e < phi
    e = 17;  // Common choice for e
    if (gcd(e, phi) != 1)
        throw std::runtime_error("e and phi are not coprime.");

    d = modInverse(e, phi);
    if (d == 0)
        throw std::runtime_error("Modular inverse calculation failed.");
}

std::pair<unsigned long long, unsigned long long> RSA::getPublicKey() const {
    return {e, n};
}

std::pair<unsigned long long, unsigned long long> RSA::getPrivateKey() const {
    return {d, n};
}

void RSA::setPrivateKey(unsigned long long privateKey, unsigned long long modulus) {
    d = privateKey;
    n = modulus;
}

unsigned long long RSA::encrypt(unsigned long long plaintext) {
    return modExp(plaintext, e, n);
}

unsigned long long RSA::decrypt(unsigned long long ciphertext) {
    return modExp(ciphertext, d, n);
}
