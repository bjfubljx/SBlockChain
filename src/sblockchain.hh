#ifndef BLOCK_CHAIN_H
#define BLOCK_CHAIN_H
#include <cstdint>
#include <string>
#include <vector>

class Block {
private:
    uint32_t _index;
    int64_t _nonce;
    std::string _data;
    std::string _hash;
    time_t _time;

    std::string _calculateHash() const;
public:
    std::string pre_hash;

    Block(uint32_t index, const std::string &data);
    Block(const std::string &str);
    std::string GetHash();
    void MineBlock(uint32_t nDifficulty);
    std::string ToString();
};

class BlockChain {
private:
    uint32_t _nDifficulty;
    std::vector<Block> _chain;

    

public:
    Block _GetLastBlock() const;
    BlockChain(bool is_genesis, uint32_t nDifficulty = 5);

    void AddBlock(Block b);


    int len();
};

#endif