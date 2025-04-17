
#include <iostream>                  // For std::cout and std::endl
#include <fstream>                   // For reading/writing seed file
#include <vector>                    // For dynamic arrays
#include <array>                     // For fixed-size entropy pool array
#include <cstdint>                   // For fixed-size integer types
#include <cstring>                   // For memory operations
#include <openssl/evp.h>            // For AES encryption
#include <openssl/rand.h>           // For secure random bytes
#include <openssl/sha.h>            // For SHA-256 hash

// Helper: SHA-256 hash function
std::vector<uint8_t> sha256(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> hash(SHA256_DIGEST_LENGTH);                 // Output buffer for 32-byte hash
    SHA256(data.data(), data.size(), hash.data());                   // Hash the input data
    return hash;                                                     // Return hash as byte vector
}

// Helper: AES-256-CTR encryption of a 16-byte counter block using a 32-byte key
std::vector<uint8_t> encryptCounter(const std::vector<uint8_t>& key, const std::vector<uint8_t>& counterBlock) {
    std::vector<uint8_t> out(counterBlock.size());                  // Output buffer same size as input block
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();                     // Create new encryption context
    EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), nullptr, key.data(), nullptr); // Initialize AES-CTR with key, default IV (null)
    int outlen;                                                     // Length of output buffer
    EVP_EncryptUpdate(ctx, out.data(), &outlen, counterBlock.data(), counterBlock.size()); // Encrypt the counter block
    EVP_EncryptFinal_ex(ctx, out.data() + outlen, &outlen);         // Finalize encryption (not really needed in CTR mode)
    EVP_CIPHER_CTX_free(ctx);                                       // Clean up
    return out;                                                     // Return encrypted block
}

// Class: EntropyAccumulator - collects entropy into multiple pools (32 total)
class EntropyAccumulator {
    static const int POOL_COUNT = 32;                               // Total number of entropy pools
    std::array<std::vector<uint8_t>, POOL_COUNT> pools;            // Array of entropy pools

public:
    // Add entropy to a specific pool, chosen by modulo source
    void addEntropy(const std::vector<uint8_t>& data, int source = 0) {
        int poolIndex = source % POOL_COUNT;                        // Choose pool by source ID
        pools[poolIndex].insert(pools[poolIndex].end(), data.begin(), data.end()); // Append data to the pool
    }

    // Generate seed from all entropy pools using SHA-256
    std::vector<uint8_t> getReseedEntropy() {
        std::vector<uint8_t> combined;                              // Combined data from all pools
        for (const auto& pool : pools) {
            combined.insert(combined.end(), pool.begin(), pool.end()); // Append each pool to combined data
        }
        auto seed = sha256(combined);                               // Hash the combined data
        clearPools();                                               // Clear pools after reseed
        return seed;                                                // Return new seed
    }

    // Clear all entropy pools
    void clearPools() {
        for (auto& pool : pools) {
            pool.clear();                                           // Empty each pool
        }
    }
};

// Class: SeedManager - responsible for loading/saving seed to local file
class SeedManager {
    const std::string path = "seed.dat";                            // File path for storing seed

public:
    // Load seed from file or create a new one if file doesn't exist
    std::vector<uint8_t> loadSeed() {
        std::ifstream in(path, std::ios::binary);                   // Open file for binary reading
        if (in) {
            return std::vector<uint8_t>((std::istreambuf_iterator<char>(in)), {}); // Read contents to vector
        } else {
            std::vector<uint8_t> seed(32);                          // Create empty buffer for 32-byte seed
            RAND_bytes(seed.data(), 32);                            // Fill buffer with random bytes
            saveSeed(seed);                                         // Save seed to file
            return seed;                                            // Return newly created seed
        }
    }

    // Save seed to file
    void saveSeed(const std::vector<uint8_t>& seed) {
        std::ofstream out(path, std::ios::binary);                  // Open file for binary writing
        out.write(reinterpret_cast<const char*>(seed.data()), seed.size()); // Write seed to file
    }
};

// Class: Generator - handles AES-CTR stream generation and key rekeying
class Generator {
    std::vector<uint8_t> key;                                       // 32-byte AES key
    uint64_t counter = 0;                                           // Counter for AES-CTR mode
    const size_t dataLimit = 1024 * 1024;                           // Limit before rekeying (1 MiB)
    size_t dataGenerated = 0;                                       // Total data generated since last rekey

public:
    // Constructor: generate random key
    Generator() {
        key.resize(32);                                             // Allocate 32 bytes for key
        RAND_bytes(key.data(), 32);                                 // Fill key with secure random bytes
    }

    // Generate a 16-byte random block using AES-CTR
    std::vector<uint8_t> generateBlock() {
        std::vector<uint8_t> counterBlock(16, 0);                   // AES block initialized to zero
        uint64_t* counterPtr = reinterpret_cast<uint64_t*>(&counterBlock[8]); // Pointer to second half of block
        *counterPtr = htobe64(counter);                             // Set counter (big-endian)
        
        auto block = encryptCounter(key, counterBlock);             // Encrypt counter block
        counter++;                                                  // Increment counter for next block
        dataGenerated += block.size();                              // Track data generated

        if (dataGenerated >= dataLimit) {                           // If limit exceeded, rekey
            rekey();                                                // Rekey using SHA-256 of current key
        }

        return block;                                               // Return generated block
    }

    // Rekey: derive new key by hashing current key
    void rekey() {
        key = sha256(key);                                          // Replace key with SHA-256 hash of current key
        dataGenerated = 0;                                          // Reset data counter
    }

    // Set generator key manually (for seeding)
    void setKey(const std::vector<uint8_t>& newKey) {
        key = newKey;                                               // Set internal key to given value
    }
};

// Class: Fortuna - combines all parts: entropy accumulator, seed manager and generator
class Fortuna {
    Generator generator;                                            // AES-CTR generator
    EntropyAccumulator accumulator;                                 // Entropy pools
    SeedManager seedManager;                                       // Seed file manager

public:
    // Constructor: initialize generator with seed
    Fortuna() {
        auto initialSeed = seedManager.loadSeed();                  // Load seed from disk (or generate)
        generator.setKey(initialSeed);                              // Set generator key with loaded seed
    }

    // Reseed generator using entropy from accumulator
    void reseed() {
        auto newSeed = accumulator.getReseedEntropy();              // Get fresh entropy
        generator.setKey(newSeed);                                  // Set new key
        seedManager.saveSeed(newSeed);                              // Save updated seed to disk
    }

    // Generate arbitrary number of random bytes
    std::vector<uint8_t> getRandomBytes(size_t numBytes) {
        std::vector<uint8_t> result;                                // Result buffer
        while (result.size() < numBytes) {                          // Loop until enough bytes generated
            auto block = generator.generateBlock();                 // Generate next block
            result.insert(result.end(), block.begin(), block.end()); // Append block to result
        }
        result.resize(numBytes);                                    // Trim to requested size
        return result;                                              // Return result
    }

    // Get reference to accumulator (to add entropy externally)
    EntropyAccumulator& getAccumulator() { return accumulator; }
};

// Entry point: simple test to demonstrate Fortuna
int main() {
    Fortuna fortuna;                                                // Create Fortuna PRNG instance

    // Add some manual entropy (simulating sensor input or user activity)
    std::vector<uint8_t> testEntropy = {0x01, 0x02, 0x03, 0x04};    // Example entropy data
    fortuna.getAccumulator().addEntropy(testEntropy);              // Add to entropy pool

    // Reseed the generator using collected entropy
    fortuna.reseed();

    // Generate 32 bytes of secure random data
    auto randomData = fortuna.getRandomBytes(32);

    // Print generated bytes in hexadecimal
    std::cout << "Generated random data: ";
    for (uint8_t byte : randomData) {
        printf("%02x", byte);
    }
    std::cout << std::endl;

    return 0;                                                       // Exit successfully
}
