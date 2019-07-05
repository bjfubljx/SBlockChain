#include <cstdint>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include "sblockchain.hh"
#include "handy.h"

#define T_INT 0
#define T_INTACK 1
#define T_CHK 2
#define T_BLK 3
#define T_BC_REQ 4
#define T_BC_RES 5


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
    std::string ip;
    std::map<uint8_t, std::pair<std::string, uint16_t>> peers;
    BlockChain *bc;
    void AddPeer(uint8_t _id, std::string _ip, uint16_t _port);
    Node(uint8_t _id, std::string _ip, uint16_t _port);
    void init(bool is_genesis);
    void run();

    enum {
        INIT,
        IDLE,
        MINE
    } _state;

    handy::EventLoop *loop;
    handy::TcpServer *server;
    int _wallet = 0;
    std::shared_ptr<handy::Timer> mine_check_timer;
    std::shared_ptr<handy::Timer> mine_timeout_timer;

    std::vector<Check> checks;
    void MineBlock();
    void Broadcast(const std::string &msg, uint8_t type);
    void AddCheck(Check chk);

};