#include "Proof_Of_Work.h"
#include <openssl/bio.h>
#include <openssl/err.h>
#include <iostream>

using namespace std;

ProofOfWork::ProofOfWork() : rsa(nullptr) {
    generateKeys();
}

ProofOfWork::~ProofOfWork() {
    if (rsa) {
        RSA_free(rsa);
    }
}

void ProofOfWork::generateKeys() {
    rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
    BIO *pub = BIO_new(BIO_s_mem());
    BIO *priv = BIO_new(BIO_s_mem());
    
    PEM_write_bio_RSAPublicKey(pub, rsa);
    PEM_write_bio_RSAPrivateKey(priv, rsa, nullptr, nullptr, 0, nullptr, nullptr);

    // Read the keys into strings
    char *pub_key;
    char *priv_key;
    long pub_len = BIO_get_mem_data(pub, &pub_key);
    long priv_len = BIO_get_mem_data(priv, &priv_key);

    publicKey.assign(pub_key, pub_len);
    privateKey.assign(priv_key, priv_len);

    BIO_free_all(pub);
    BIO_free_all(priv);
}

string ProofOfWork::signData(const std::string &data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)data.c_str(), data.size(), hash);

    unsigned char *signature = new unsigned char[RSA_size(rsa)];
    unsigned int signature_len;

    if (RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, signature, &signature_len, rsa) != 1) {
        delete[] signature;
        return ""; // Sign failed
    }

    string sig(reinterpret_cast<char*>(signature), signature_len);
    delete[] signature;
    return sig;
}

bool ProofOfWork::verifyData(const std::string &data, const std::string &signature) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)data.c_str(), data.size(), hash);

    return RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH,
                      (unsigned char*)signature.c_str(), signature.size(), rsa) == 1;
}

string ProofOfWork::getPublicKey() const {
    return publicKey;
}

string ProofOfWork::getPrivateKey() const {
    return privateKey;
}
