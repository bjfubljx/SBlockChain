#include "sblockchain.hh"
#include "isha256.hh"
#include <sstream>
#include <iostream>

Block::Block(uint32_t index, const std::string &data)
{
    _index = index;
    _data = data;
    _nonce = -1;
    _time = time(nullptr);
}

std::string Block::GetHash()
{
    return _hash;
}

void Block::MineBlock(uint32_t nDifficulty)
{
    std::string str(nDifficulty, '0');
    do {
        _nonce++;
        _hash = _calculateHash();
    } while(_hash.substr(0, nDifficulty) != str);

    std::cout << _hash << std::endl;
}

std::string Block::_calculateHash() const
{
    std::stringstream ss;
    ss << _index << _time << _data << _nonce << pre_hash;
    return sha256(ss.str());
}

BlockChain::BlockChain(bool is_genesis, uint32_t nDifficulty) {
    _nDifficulty = nDifficulty;
    if(is_genesis) {
        Block b(0, "Genesis Block");
        b.pre_hash = "";
        _chain.push_back(b);
    }
}

void BlockChain::AddBlock(Block b)
{
    b.pre_hash = _GetLastBlock().GetHash();
    b.MineBlock(_nDifficulty);
    _chain.push_back(b);
}

Block BlockChain::_GetLastBlock() const
{
    return _chain.back();
}