
// // bool Blockchain::saveToFile(const string& filename) const {
// //     cout << "[DEBUG] Starting saveToFile function." << endl;

// //     ofstream outFile(filename, ios::binary | ios::trunc);
// //     if (!outFile.is_open()) {
// //         cerr << "[ERROR] Unable to open file for saving blockchain." << endl;
// //         throw runtime_error("Unable to open file for saving blockchain.");
// //     }

// //     Block* currentBlock = latestBlock;
// //     while (currentBlock != nullptr) {
// //         cout << "[DEBUG] Saving block: " << currentBlock->blockHash << endl;

// //         // Save block hash
// //         size_t hashSize = currentBlock->blockHash.size();
// //         outFile.write(reinterpret_cast<const char*>(&hashSize), sizeof(hashSize));
// //         outFile.write(currentBlock->blockHash.data(), hashSize);
// //         cout << "[DEBUG] Block hash saved (size: " << hashSize << ")." << endl;

// //         // Save previous hash
// //         bool hasPrevHash = currentBlock->prevhash != nullptr;
// //         outFile.write(reinterpret_cast<const char*>(&hasPrevHash), sizeof(hasPrevHash));
// //         if (hasPrevHash) {
// //             size_t prevHashSize = currentBlock->prevhash->blockHash.size();
// //             outFile.write(reinterpret_cast<const char*>(&prevHashSize), sizeof(prevHashSize));
// //             outFile.write(currentBlock->prevhash->blockHash.data(), prevHashSize);
// //             cout << "[DEBUG] Previous hash saved (size: " << prevHashSize << ")." << endl;
// //         } else {
// //             cout << "[DEBUG] No previous hash to save." << endl;
// //         }

// //         // Save Merkle root
// //         string merkleRoot = currentBlock->getMerkleRoot();
// //         size_t merkleSize = merkleRoot.size();
// //         outFile.write(reinterpret_cast<const char*>(&merkleSize), sizeof(merkleSize));
// //         outFile.write(merkleRoot.data(), merkleSize);
// //         cout << "[DEBUG] Merkle root saved (size: " << merkleSize << ")." << endl;

// //         // Save nonce
// //         outFile.write(reinterpret_cast<const char*>(&currentBlock->nonce), sizeof(currentBlock->nonce));
// //         cout << "[DEBUG] Nonce saved: " << currentBlock->nonce << endl;

// //         // Save timestamp
// //         time_t timestamp = chrono::system_clock::to_time_t(currentBlock->timestamp);
// //         outFile.write(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));
// //         cout << "[DEBUG] Timestamp saved: " << timestamp << endl;

// //         // Save transactions
// //         size_t txCount = currentBlock->transactions.size();
// //         outFile.write(reinterpret_cast<const char*>(&txCount), sizeof(txCount));
// //         cout << "[DEBUG] Number of transactions: " << txCount << endl;

// //         for (const auto& tx : currentBlock->transactions) {
// //             // Save sender
// //             size_t senderSize = tx.get_sender().size();
// //             outFile.write(reinterpret_cast<const char*>(&senderSize), sizeof(senderSize));
// //             outFile.write(tx.get_sender().data(), senderSize);
// //             cout << "[DEBUG] Sender saved: " << tx.get_sender() << endl;

// //             // Save receiver
// //             size_t receiverSize = tx.get_receiver().size();
// //             outFile.write(reinterpret_cast<const char*>(&receiverSize), sizeof(receiverSize));
// //             outFile.write(tx.get_receiver().data(), receiverSize);
// //             cout << "[DEBUG] Receiver saved: " << tx.get_receiver() << endl;

// //             // Save amount
// //             double amount = tx.get_amount();
// //             outFile.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
// //             cout << "[DEBUG] Amount saved: " << amount << endl;
// //         }

// //         currentBlock = currentBlock->prevhash; // Move to the previous block
// //     }

// //     outFile.close();
// //     cout << "[DEBUG] saveToFile completed successfully." << endl;
// //     return true;
// //  }
// // bool Blockchain::saveToFile(const string& filename) const {
// //     cout << "[DEBUG] Starting saveToFile function." << endl;

// //     ofstream outFile(filename, ios::binary | ios::trunc);
// //     if (!outFile.is_open()) {
// //         cerr << "[ERROR] Unable to open file for saving blockchain." << endl;
// //         throw runtime_error("Unable to open file for saving blockchain.");
// //     }

// //     try {
// //         Block* currentBlock = latestBlock;
// //         while (currentBlock != nullptr) {
// //             cout << "[DEBUG] Saving block: " << currentBlock->blockHash << endl;

// //             // Save block hash
// //             size_t hashSize = currentBlock->blockHash.size();
// //             outFile.write(reinterpret_cast<const char*>(&hashSize), sizeof(hashSize));
// //             outFile.write(currentBlock->blockHash.data(), hashSize);
// //             if (!outFile) throw runtime_error("Failed to write block hash.");

// //             // Save previous hash
// //             bool hasPrevHash = currentBlock->prevhash != nullptr;
// //             outFile.write(reinterpret_cast<const char*>(&hasPrevHash), sizeof(hasPrevHash));
// //             if (hasPrevHash) {
// //                 size_t prevHashSize = currentBlock->prevhash->blockHash.size();
// //                 outFile.write(reinterpret_cast<const char*>(&prevHashSize), sizeof(prevHashSize));
// //                 outFile.write(currentBlock->prevhash->blockHash.data(), prevHashSize);
// //                 if (!outFile) throw runtime_error("Failed to write previous block hash.");
// //             }

// //             // Save Merkle root
// //             string merkleRoot = currentBlock->getMerkleRoot();
// //             size_t merkleSize = merkleRoot.size();
// //             outFile.write(reinterpret_cast<const char*>(&merkleSize), sizeof(merkleSize));
// //             outFile.write(merkleRoot.data(), merkleSize);
// //             if (!outFile) throw runtime_error("Failed to write Merkle root.");

// //             // Save nonce
// //             outFile.write(reinterpret_cast<const char*>(&currentBlock->nonce), sizeof(currentBlock->nonce));
// //             if (!outFile) throw runtime_error("Failed to write nonce.");

// //             // Save timestamp
// //             time_t timestamp = chrono::system_clock::to_time_t(currentBlock->timestamp);
// //             outFile.write(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));
// //             if (!outFile) throw runtime_error("Failed to write timestamp.");

// //             // Save transactions
// //             size_t txCount = currentBlock->transactions.size();
// //             outFile.write(reinterpret_cast<const char*>(&txCount), sizeof(txCount));
// //             if (!outFile) throw runtime_error("Failed to write transaction count.");

// //             for (const auto& tx : currentBlock->transactions) {
// //                 size_t senderSize = tx.get_sender().size();
// //                 outFile.write(reinterpret_cast<const char*>(&senderSize), sizeof(senderSize));
// //                 outFile.write(tx.get_sender().data(), senderSize);

// //                 size_t receiverSize = tx.get_receiver().size();
// //                 outFile.write(reinterpret_cast<const char*>(&receiverSize), sizeof(receiverSize));
// //                 outFile.write(tx.get_receiver().data(), receiverSize);

// //                 double amount = tx.get_amount();
// //                 outFile.write(reinterpret_cast<const char*>(&amount), sizeof(amount));

// //                 if (!outFile) throw runtime_error("Failed to write transaction data.");
// //             }

// //             currentBlock = currentBlock->prevhash; // Move to the previous block
// //         }

// //     } catch (const exception& e) {
// //         cerr << "[ERROR] Exception occurred during saveToFile: " << e.what() << endl;
// //         outFile.close();
// //         throw; // Rethrow the exception for the caller to handle
// //     }

// //     outFile.close();
// //     cout << "[DEBUG] saveToFile completed successfully." << endl;
// //     return true;
// // }
// bool Blockchain::saveToFile(const string& filename) const {
//     cout << "[DEBUG] Starting saveToFile function." << endl;

//     // Open the file with truncation to ensure it starts fresh
//     ofstream outFile(filename, ios::binary | ios::trunc);
//     if (!outFile.is_open()) {
//         cerr << "[ERROR] Unable to open file for saving blockchain." << endl;
//         return false;
//     }

//     try {
//         for ( const auto& block : chain) {
//             // Save block hash
//             size_t hashSize = block.blockHash.size();
//             outFile.write(reinterpret_cast<const char*>(&hashSize), sizeof(hashSize));
//             outFile.write(block.blockHash.data(), hashSize);

//             // Save previous hash
//             bool hasPrevHash = block.prevhash != nullptr;
//             outFile.write(reinterpret_cast<const char*>(&hasPrevHash), sizeof(hasPrevHash));
//             if (hasPrevHash) {
//                 size_t prevHashSize = block.prevhash->blockHash.size();
//                 outFile.write(reinterpret_cast<const char*>(&prevHashSize), sizeof(prevHashSize));
//                 outFile.write(block.prevhash->blockHash.data(), prevHashSize);
//             }

//             // Save Merkle root
//             string merkleRoot = const_cast<Block&>(block).getMerkleRoot();
//             size_t merkleSize = merkleRoot.size();
//             outFile.write(reinterpret_cast<const char*>(&merkleSize), sizeof(merkleSize));
//             outFile.write(merkleRoot.data(), merkleSize);

//             // Save nonce
//             outFile.write(reinterpret_cast<const char*>(&block.nonce), sizeof(block.nonce));

//             // Save timestamp
//             time_t timestamp = chrono::system_clock::to_time_t(block.timestamp);
//             outFile.write(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));

//             // Save transactions
//             size_t txCount = block.transactions.size();
//             outFile.write(reinterpret_cast<const char*>(&txCount), sizeof(txCount));

//             for (const auto& tx : block.transactions) {
//                 size_t senderSize = tx.get_sender().size();
//                 outFile.write(reinterpret_cast<const char*>(&senderSize), sizeof(senderSize));
//                 outFile.write(tx.get_sender().data(), senderSize);

//                 size_t receiverSize = tx.get_receiver().size();
//                 outFile.write(reinterpret_cast<const char*>(&receiverSize), sizeof(receiverSize));
//                 outFile.write(tx.get_receiver().data(), receiverSize);

//                 double amount = tx.get_amount();
//                 outFile.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
//             }
//         }

//         cout << "[DEBUG] Blockchain saved successfully to " << filename << endl;
//     } catch (const exception& e) {
//         cerr << "[ERROR] Exception during saveToFile: " << e.what() << endl;
//         outFile.close();
//         return false;
//     }

//     outFile.close();
//     return true;
// }



// // bool Blockchain::loadFromFile(const string& filename) {
// //     cout << "[DEBUG] Starting loadFromFile function." << endl;

// //     ifstream inFile(filename, ios::binary);
// //     if (!inFile.is_open()) {
// //         cerr << "[ERROR] Unable to open file for loading blockchain." << endl;
// //         throw runtime_error("Unable to open file for loading blockchain.");
// //     }

// //     unordered_map<string, Block*> blockMap; // Map to store blocks by their hash
// //     vector<Block*> loadedBlocks;

// //     while (true) {
// //         cout << "[DEBUG] Reading block data." << endl;

// //         // Read block hash
// //         size_t hashSize;
// //         if (!inFile.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize))) break;
// //         string blockHash(hashSize, '\0');
// //         inFile.read(&blockHash[0], hashSize);
// //         cout << "[DEBUG] Block hash read: " << blockHash << " (size: " << hashSize << ")" << endl;

// //         // Read previous hash
// //         bool hasPrevHash;
// //         inFile.read(reinterpret_cast<char*>(&hasPrevHash), sizeof(hasPrevHash));
// //         string prevHash;
// //         if (hasPrevHash) {
// //             size_t prevHashSize;
// //             inFile.read(reinterpret_cast<char*>(&prevHashSize), sizeof(prevHashSize));
// //             prevHash.resize(prevHashSize);
// //             inFile.read(&prevHash[0], prevHashSize);
// //             cout << "[DEBUG] Previous hash read: " << prevHash << " (size: " << prevHashSize << ")" << endl;
// //         } else {
// //             cout << "[DEBUG] No previous hash found." << endl;
// //         }

// //         // Read Merkle root
// //         size_t merkleSize;
// //         inFile.read(reinterpret_cast<char*>(&merkleSize), sizeof(merkleSize));
// //         string merkleRoot(merkleSize, '\0');
// //         inFile.read(&merkleRoot[0], merkleSize);
// //         cout << "[DEBUG] Merkle root read: " << merkleRoot << " (size: " << merkleSize << ")" << endl;

// //         // Read nonce
// //         int nonce;
// //         inFile.read(reinterpret_cast<char*>(&nonce), sizeof(nonce));
// //         cout << "[DEBUG] Nonce read: " << nonce << endl;

// //         // Read timestamp
// //         time_t timestamp;
// //         inFile.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
// //         cout << "[DEBUG] Timestamp read: " << timestamp << endl;

// //         // Read transactions
// //         size_t txCount;
// //         inFile.read(reinterpret_cast<char*>(&txCount), sizeof(txCount));
// //         cout << "[DEBUG] Number of transactions read: " << txCount << endl;

// //         vector<Transaction> transactions;
// //         for (size_t i = 0; i < txCount; ++i) {
// //             cout << "[DEBUG] Reading transaction " << i + 1 << " of " << txCount << "." << endl;

// //             // Read sender
// //             size_t senderSize;
// //             inFile.read(reinterpret_cast<char*>(&senderSize), sizeof(senderSize));
// //             string sender(senderSize, '\0');
// //             inFile.read(&sender[0], senderSize);
// //             cout << "[DEBUG] Sender read: " << sender << endl;

// //             // Read receiver
// //             size_t receiverSize;
// //             inFile.read(reinterpret_cast<char*>(&receiverSize), sizeof(receiverSize));
// //             string receiver(receiverSize, '\0');
// //             inFile.read(&receiver[0], receiverSize);
// //             cout << "[DEBUG] Receiver read: " << receiver << endl;

// //             // Read amount
// //             double amount;
// //             inFile.read(reinterpret_cast<char*>(&amount), sizeof(amount));
// //             cout << "[DEBUG] Amount read: " << amount << endl;

// //             transactions.emplace_back(sender, receiver, amount);
// //         }

// //         // Create new block (temporary prevhash link is null)
// //         Block* newBlock = new Block(transactions, nullptr, nonce, blockHash, chrono::system_clock::from_time_t(timestamp));
// //         loadedBlocks.push_back(newBlock);
// //         blockMap[blockHash] = newBlock; // Store in map for later linking
// //         cout << "[DEBUG] Block reconstructed and added to map." << endl;

// //         // Link prevhash if available
// //         if (hasPrevHash && blockMap.find(prevHash) != blockMap.end()) {
// //             newBlock->prevhash = blockMap[prevHash];
// //         }
// //     }

// //     inFile.close();

// //     // Rebuild the blockchain
// //     if (!loadedBlocks.empty()) {
// //         cout << "[DEBUG] Rebuilding blockchain from loaded blocks." << endl;
// //         genesisBlock = loadedBlocks.back();
// //         latestBlock = loadedBlocks.front();
// //         chain.clear();

// //         for (auto block : loadedBlocks) {
// //             chain.push_back(*block);
// //         }
// //     }

// //     cout << "[DEBUG] loadFromFile completed successfully." << endl;
// //     return true;
// // // // }
// // bool Blockchain::loadFromFile(const string& filename) {
// //     cout << "[DEBUG] Starting loadFromFile function." << endl;

// //     ifstream inFile(filename, ios::binary);
// //     if (!inFile.is_open()) {
// //         cerr << "[ERROR] Unable to open file for loading blockchain." << endl;
// //         return false;
// //     }

// //     try {
// //         chain.clear(); // Clear any existing blockchain data

// //         while (inFile.peek() != EOF) {
// //             // Load block hash
// //             size_t hashSize;
// //             inFile.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
// //             string blockHash(hashSize, '\0');
// //             inFile.read(&blockHash[0], hashSize);

// //             // Load previous hash
// //             bool hasPrevHash;
// //             inFile.read(reinterpret_cast<char*>(&hasPrevHash), sizeof(hasPrevHash));
// //             string prevHash;
// //             if (hasPrevHash) {
// //                 size_t prevHashSize;
// //                 inFile.read(reinterpret_cast<char*>(&prevHashSize), sizeof(prevHashSize));
// //                 prevHash.resize(prevHashSize);
// //                 inFile.read(&prevHash[0], prevHashSize);
// //             }

// //             // Load Merkle root
// //             size_t merkleSize;
// //             inFile.read(reinterpret_cast<char*>(&merkleSize), sizeof(merkleSize));
// //             string merkleRoot(merkleSize, '\0');
// //             inFile.read(&merkleRoot[0], merkleSize);

// //             // Load nonce
// //             int nonce;
// //             inFile.read(reinterpret_cast<char*>(&nonce), sizeof(nonce));

// //             // Load timestamp
// //             time_t timestamp;
// //             inFile.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
// //             auto blockTimestamp = chrono::system_clock::from_time_t(timestamp);

// //             // Load transactions
// //             size_t txCount;
// //             inFile.read(reinterpret_cast<char*>(&txCount), sizeof(txCount));

// //             vector<Transaction> transactions;
// //             for (size_t i = 0; i < txCount; ++i) {
// //                 // Load sender
// //                 size_t senderSize;
// //                 inFile.read(reinterpret_cast<char*>(&senderSize), sizeof(senderSize));
// //                 string sender(senderSize, '\0');
// //                 inFile.read(&sender[0], senderSize);

// //                 // Load receiver
// //                 size_t receiverSize;
// //                 inFile.read(reinterpret_cast<char*>(&receiverSize), sizeof(receiverSize));
// //                 string receiver(receiverSize, '\0');
// //                 inFile.read(&receiver[0], receiverSize);

// //                 // Load amount
// //                 double amount;
// //                 inFile.read(reinterpret_cast<char*>(&amount), sizeof(amount));

// //                 transactions.emplace_back(sender, receiver, amount);
// //             }

// //             // Reconstruct block
// //             Block block(transactions, nullptr, nonce, blockHash, blockTimestamp);

// //             // Link to the previous block if available
// //             if (!chain.empty()) {
// //                 block.prevhash = &chain.back();
// //             }

// //             chain.push_back(block);
// //         }

// //         // Update genesisBlock and latestBlock pointers
// //         if (!chain.empty()) {
// //             genesisBlock = &chain.front();
// //             latestBlock = &chain.back();
// //         }

// //         cout << "[DEBUG] Blockchain loaded successfully from " << filename << endl;
// //     } catch (const exception& e) {
// //         cerr << "[ERROR] Exception during loadFromFile: " << e.what() << endl;
// //         inFile.close();
// //         return false;
// //     }

// //     inFile.close();
// //     return true;
// // // }
// // bool Blockchain::loadFromFile(const string& filename) {
// //     cout << "[DEBUG] Starting loadFromFile function." << endl;

// //     // Open the file in binary mode
// //     ifstream inFile(filename, ios::binary);
// //     if (!inFile.is_open()) {
// //         cerr << "[ERROR] Unable to open file for loading blockchain." << endl;
// //         return false;
// //     }

// //     try {
// //         vector<Block*> loadedBlocks; // Temporary storage for blocks to restore links
// //         Block* prevBlock = nullptr;

// //         while (inFile.peek() != EOF) { // Read until the end of the file
// //             // Read block hash
// //             size_t hashSize;
// //             inFile.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
// //             string blockHash(hashSize, '\0');
// //             inFile.read(&blockHash[0], hashSize);

// //             // Read previous hash (if exists)
// //             bool hasPrevHash;
// //             inFile.read(reinterpret_cast<char*>(&hasPrevHash), sizeof(hasPrevHash));
// //             string prevBlockHash;
// //             if (hasPrevHash) {
// //                 size_t prevHashSize;
// //                 inFile.read(reinterpret_cast<char*>(&prevHashSize), sizeof(prevHashSize));
// //                 prevBlockHash.resize(prevHashSize);
// //                 inFile.read(&prevBlockHash[0], prevHashSize);
// //             }

// //             // Read Merkle root
// //             size_t merkleSize;
// //             inFile.read(reinterpret_cast<char*>(&merkleSize), sizeof(merkleSize));
// //             string merkleRoot(merkleSize, '\0');
// //             inFile.read(&merkleRoot[0], merkleSize);

// //             // Read nonce
// //             int nonce;
// //             inFile.read(reinterpret_cast<char*>(&nonce), sizeof(nonce));

// //             // Read timestamp
// //             time_t timestamp;
// //             inFile.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
// //             auto blockTime = chrono::system_clock::from_time_t(timestamp);

// //             // Read transactions
// //             size_t txCount;
// //             inFile.read(reinterpret_cast<char*>(&txCount), sizeof(txCount));
// //             vector<Transaction> transactions;

// //             for (size_t i = 0; i < txCount; ++i) {
// //                 size_t senderSize;
// //                 inFile.read(reinterpret_cast<char*>(&senderSize), sizeof(senderSize));
// //                 string sender(senderSize, '\0');
// //                 inFile.read(&sender[0], senderSize);

// //                 size_t receiverSize;
// //                 inFile.read(reinterpret_cast<char*>(&receiverSize), sizeof(receiverSize));
// //                 string receiver(receiverSize, '\0');
// //                 inFile.read(&receiver[0], receiverSize);

// //                 double amount;
// //                 inFile.read(reinterpret_cast<char*>(&amount), sizeof(amount));

// //                 transactions.emplace_back(sender, receiver, amount);
// //             }

// //             // Reconstruct the block
// //             Block* block = new Block(transactions, prevBlock, nonce, blockHash, blockTime);

// //             // Restore previous hash link
// //             block->prevhash = prevBlock;

// //             // Add to the list of loaded blocks
// //             loadedBlocks.push_back(block);

// //             // Update prevBlock for the next block
// //             prevBlock = block;
// //         }

// //         // Update latestBlock to the last block loaded
// //         if (!loadedBlocks.empty()) {
// //             latestBlock = loadedBlocks.back();
// //         } else {
// //             latestBlock = nullptr;
// //         }

// //         cout << "[DEBUG] loadFromFile completed successfully." << endl;
// //     } catch (const exception& e) {
// //         cerr << "[ERROR] Exception during loadFromFile: " << e.what() << endl;
// //         inFile.close();
// //         return false;
// //     }

// //     inFile.close();
// //     return true;
// // }
// bool Blockchain::loadFromFile(const string& filename) {
//     cout << "[DEBUG] Starting loadFromFile function." << endl;

//     // Open the file in binary mode
//     ifstream inFile(filename, ios::binary);
//     if (!inFile.is_open()) {
//         cerr << "[ERROR] Unable to open file for loading blockchain." << endl;
//         return false;
//     }

//     try {
//         vector<Block*> loadedBlocks; // Temporary storage for blocks
//         unordered_map<string, string> blockToPrevHash; // Map to track block and its previous hash
//         unordered_map<string, Block*> hashToBlock; // Map to associate hashes with blocks for linking
//         Block* lastBlock = nullptr;

//         while (inFile.peek() != EOF) { // Read until the end of the file
//             // Read block hash
//             size_t hashSize;
//             inFile.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
//             string blockHash(hashSize, '\0');
//             inFile.read(&blockHash[0], hashSize);

//             // Read previous hash (if exists)
//             bool hasPrevHash;
//             inFile.read(reinterpret_cast<char*>(&hasPrevHash), sizeof(hasPrevHash));
//             string prevBlockHash;
//             if (hasPrevHash) {
//                 size_t prevHashSize;
//                 inFile.read(reinterpret_cast<char*>(&prevHashSize), sizeof(prevHashSize));
//                 prevBlockHash.resize(prevHashSize);
//                 inFile.read(&prevBlockHash[0], prevHashSize);
//             }

//             // Read Merkle root
//             size_t merkleSize;
//             inFile.read(reinterpret_cast<char*>(&merkleSize), sizeof(merkleSize));
//             string merkleRoot(merkleSize, '\0');
//             inFile.read(&merkleRoot[0], merkleSize);

//             // Read nonce
//             int nonce;
//             inFile.read(reinterpret_cast<char*>(&nonce), sizeof(nonce));

//             // Read timestamp
//             time_t timestamp;
//             inFile.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
//             auto blockTime = chrono::system_clock::from_time_t(timestamp);

//             // Read transactions
//             size_t txCount;
//             inFile.read(reinterpret_cast<char*>(&txCount), sizeof(txCount));
//             vector<Transaction> transactions;

//             for (size_t i = 0; i < txCount; ++i) {
//                 size_t senderSize;
//                 inFile.read(reinterpret_cast<char*>(&senderSize), sizeof(senderSize));
//                 string sender(senderSize, '\0');
//                 inFile.read(&sender[0], senderSize);

//                 size_t receiverSize;
//                 inFile.read(reinterpret_cast<char*>(&receiverSize), sizeof(receiverSize));
//                 string receiver(receiverSize, '\0');
//                 inFile.read(&receiver[0], receiverSize);

//                 double amount;
//                 inFile.read(reinterpret_cast<char*>(&amount), sizeof(amount));

//                 transactions.emplace_back(sender, receiver, amount);
//             }

//             // Create block
//             Block* block = new Block(transactions, nullptr, nonce, blockHash, blockTime);

//             // Track the block and its previous hash
//             if (!prevBlockHash.empty()) {
//                 blockToPrevHash[blockHash] = prevBlockHash;
//             }

//             // Add block to map
//             hashToBlock[blockHash] = block;
//             loadedBlocks.push_back(block);
//         }

//         // Relink blocks using the hash map
//         for (const auto& [currentHash, prevHash] : blockToPrevHash) {
//             if (hashToBlock.find(prevHash) != hashToBlock.end()) {
//                 hashToBlock[currentHash]->prevhash = hashToBlock[prevHash];
//             }
//         }

//         // Update latestBlock to the last block loaded
//         if (!loadedBlocks.empty()) {
//             latestBlock = loadedBlocks.back();
//         } else {
//             latestBlock = nullptr;
//         }

//         cout << "[DEBUG] loadFromFile completed successfully." << endl;
//     } catch (const exception& e) {
//         cerr << "[ERROR] Exception during loadFromFile: " << e.what() << endl;
//         inFile.close();
//         return false;
//     }

//     inFile.close();
//     return true;
// }
