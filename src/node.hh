#include <cstdint>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include "sblockchain.hh"
#include "handy.h"

class Check {
private:
    uint8_t _from;
    uint8_t _to;
    int _amount;
public:
    Check(uint8_t from, uint8_t to, int amount);
    Check(std::string str);
    std::string ToString();
};

class Message {
public:
    uint8_t type;
    uint8_t from;
    uint8_t to;
    std::string payload;

    Message(uint8_t _from, uint8_t _to, uint8_t _type, std::string _msg);
    Message(const std::string &str);

    std::string ToString();
};

class Node {
public:
    uint8_t id;
    std::map<uint8_t, std::pair<std::string, int>> peers;
    BlockChain *bc;
    void AddPeer(uint8_t _id, std::string _ip, int _port);
    Node(uint8_t _id, std::string _ip, int _port);
    void init(bool is_genesis);
    void run();

    enum {
        INIT,
        IDLE,
        MINE
    } _state;

private:
    handy::EventLoop *loop;
    handy::TcpServer *server;
    int _wallet = 0;
    std::shared_ptr<handy::Timer> mine_check_timer;
    std::shared_ptr<handy::Timer> mine_timeout_timer;
    std::vector<Check> checks;
};