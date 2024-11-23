#include "MiningHandler.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>


// Simulate mining a block (a simple puzzle like finding a number divisible by 10)
bool MiningHandler::mineBlock(const std::string& minerId) {
    std::cout << "Mining started for miner: " << minerId << "\n";

    // Simulate mining delay using a simple loop (inefficient)
    for (volatile int i = 0; i < 1000000000; ++i) {
        // Empty loop to waste time (delays the program execution)
    }
   

    // Simple mining logic (finding a number divisible by 10)
    int blockNumber = 0;
    while (blockNumber % 10 != 0) {
        blockNumber++;
    }

    return true;  // Mining successful
}

// Display the result of the mining process
void MiningHandler::displayMiningResult(const std::string& minerId, bool success) {
    if (success) {
        std::cout << "Mining successful for " << minerId << "! Block mined.\n";
    } else {
        std::cout << "Mining failed for " << minerId << ". Try again.\n";
    }
}
