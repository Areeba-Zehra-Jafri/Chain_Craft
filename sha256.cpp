#include "SHA256.h"
#include <cstring>
#include <sstream>
#include <iomanip>

const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

SHA256::SHA256() {
    h[0] = 0x6a09e667;
    h[1] = 0xbb67ae85;
    h[2] = 0x3c6ef372;
    h[3] = 0xa54ff53a;
    h[4] = 0x510e527f;
    h[5] = 0x9b05688c;
    h[6] = 0x1f83d9ab;
    h[7] = 0x5be0cd19;
    bitLength = 0;
    bufferLength = 0;
    schedule.resize(64);
}

uint32_t SHA256::rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

uint32_t SHA256::choose(uint32_t e, uint32_t f, uint32_t g) {
    return (e & f) ^ (~e & g);
}

uint32_t SHA256::majority(uint32_t a, uint32_t b, uint32_t c) {
    return (a & b) ^ (a & c) ^ (b & c);
}

uint32_t SHA256::sig0(uint32_t x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

uint32_t SHA256::sig1(uint32_t x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

void SHA256::transform(const unsigned char* chunk) {
    for (size_t i = 0; i < 16; ++i)
        schedule[i] = (chunk[i * 4] << 24) | (chunk[i * 4 + 1] << 16) | (chunk[i * 4 + 2] << 8) | (chunk[i * 4 + 3]);
    for (size_t i = 16; i < 64; ++i)
        schedule[i] = sig1(schedule[i - 2]) + schedule[i - 7] + sig0(schedule[i - 15]) + schedule[i - 16];

    uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
    uint32_t e = h[4], f = h[5], g = h[6], h_val = h[7];

    for (size_t i = 0; i < 64; ++i) {
        uint32_t temp1 = h_val + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) + choose(e, f, g) + K[i] + schedule[i];
        uint32_t temp2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) + majority(a, b, c);
        h_val = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
    h[5] += f;
    h[6] += g;
    h[7] += h_val;
}

void SHA256::update(const unsigned char* data, size_t length) {
    bitLength += length * 8;
    for (size_t i = 0; i < length; ++i) {
        buffer[bufferLength++] = data[i];
        if (bufferLength == 64) {
            transform(buffer);
            bufferLength = 0;
        }
    }
}

void SHA256::pad() {
    unsigned char padding[64] = { 0x80 };
    size_t paddingLength = (bufferLength < 56) ? (56 - bufferLength) : (64 + 56 - bufferLength);
    update(padding, paddingLength);
    unsigned char lengthBytes[8];
    for (int i = 0; i < 8; ++i)
        lengthBytes[7 - i] = bitLength >> (i * 8);
    update(lengthBytes, 8);
}

std::string SHA256::final() {
    pad();
    std::stringstream result;
    for (uint32_t val : h)
        result << std::hex << std::setw(8) << std::setfill('0') << val;
    return result.str();
}

std::string SHA256::hash(const std::string& input) {
    SHA256 sha256;
    sha256.update(reinterpret_cast<const unsigned char*>(input.c_str()), input.size());
    return sha256.final();
}