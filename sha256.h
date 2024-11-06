#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <vector>

class SHA256 {
public:
    SHA256();
    void update(const unsigned char* data, size_t length);
    std::string final();
    static std::string hash(const std::string& input);

private:
    void transform(const unsigned char* chunk);
    void pad();
    void process();

    uint32_t h[8];
    uint64_t bitLength;
    unsigned char buffer[64];
    size_t bufferLength;
    std::vector<uint32_t> schedule;

    static uint32_t rotr(uint32_t x, uint32_t n);
    static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);
    static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);
    static uint32_t sig0(uint32_t x);
    static uint32_t sig1(uint32_t x);
};

#endif // SHA256_H
