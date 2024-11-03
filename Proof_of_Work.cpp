#include "Proof_Of_Work.h"
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>       // EVP API for key generation and signing
#include <openssl/sha.h>       // SHA hashing functions
#include <iostream>
#include <vector>

using namespace std;

ProofOfWork::ProofOfWork() : evpKey(nullptr) {
    generateKeys();
}

ProofOfWork::~ProofOfWork() {
    if (evpKey) {
        EVP_PKEY_free(evpKey);
    }
}

void ProofOfWork::generateKeys() {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx) {
        cerr << "Failed to create EVP_PKEY_CTX." << endl;
        return;
    }

    if (EVP_PKEY_keygen_init(ctx) <= 0 || EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
        cerr << "Failed to initialize RSA key generation." << endl;
        EVP_PKEY_CTX_free(ctx);
        return;
    }

    if (EVP_PKEY_keygen(ctx, &evpKey) <= 0) {
        cerr << "Failed to generate RSA keys." << endl;
        EVP_PKEY_CTX_free(ctx);
        return;
    }
    EVP_PKEY_CTX_free(ctx);

    // Convert keys to PEM format
    BIO *pub = BIO_new(BIO_s_mem());
    BIO *priv = BIO_new(BIO_s_mem());

    if (!PEM_write_bio_PUBKEY(pub, evpKey) || !PEM_write_bio_PrivateKey(priv, evpKey, nullptr, nullptr, 0, nullptr, nullptr)) {
        cerr << "Failed to write RSA keys to BIO." << endl;
        BIO_free_all(pub);
        BIO_free_all(priv);
        return;
    }

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
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        cerr << "Failed to create EVP_MD_CTX." << endl;
        return "";
    }

    if (EVP_DigestSignInit(mdctx, nullptr, EVP_sha256(), nullptr, evpKey) <= 0) {
        cerr << "DigestSignInit failed." << endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    if (EVP_DigestSignUpdate(mdctx, hash, SHA256_DIGEST_LENGTH) <= 0) {
        cerr << "DigestSignUpdate failed." << endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    size_t sig_len = 0;
    if (EVP_DigestSignFinal(mdctx, nullptr, &sig_len) <= 0) {
        cerr << "DigestSignFinal (length) failed." << endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    vector<unsigned char> signature(sig_len);
    if (EVP_DigestSignFinal(mdctx, signature.data(), &sig_len) <= 0) {
        cerr << "DigestSignFinal failed." << endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    EVP_MD_CTX_free(mdctx);
    return string(signature.begin(), signature.begin() + sig_len);
}

bool ProofOfWork::verifyData(const std::string &data, const std::string &signature) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        cerr << "Failed to create EVP_MD_CTX." << endl;
        return false;
    }

    if (EVP_DigestVerifyInit(mdctx, nullptr, EVP_sha256(), nullptr, evpKey) <= 0) {
        cerr << "DigestVerifyInit failed." << endl;
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    if (EVP_DigestVerifyUpdate(mdctx, hash, SHA256_DIGEST_LENGTH) <= 0) {
        cerr << "DigestVerifyUpdate failed." << endl;
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    bool result = EVP_DigestVerifyFinal(mdctx, reinterpret_cast<const unsigned char*>(signature.c_str()), signature.size()) == 1;
    EVP_MD_CTX_free(mdctx);
    return result;
}

string ProofOfWork::getPublicKey() const {
    return publicKey;
}

string ProofOfWork::getPrivateKey() const {
    return privateKey;
}