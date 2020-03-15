#include "node.hh"
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <iostream>


Check::Check(uint8_t from, uint8_t to, int amount)
{
    _from = from;
    _to = to;
    _amount = amount;
}

Check::Check(std::string str)
{
    std::stringstream ss(str);
    char ch;
    ss >> _from >> ch >> _to >> ch >> _amount;
}

std::string Check::ToString()
{
    std::stringstream ss;
    ss << _from << "," << _to << "," << _amount;
}


Message::Message(uint8_t _from, uint8_t _to, uint8_t _type, std::string _msg)
{
    from = _from;
    to = _to;
    type = _type;
    payload = _msg;
}

Message::Message(const std::string &str)
{
    auto pos = str.find(" ");
    type = atoi(str.substr(0, pos).c_str());
    auto pre_pos = pos + 1;
    pos = str.find(" ", pre_pos);
    from = atoi(str.substr(pre_pos, pos - pre_pos).c_str());
    pre_pos = pos + 1;
    pos = str.find(" ", pre_pos);
    to = atoi(str.substr(pre_pos, pos - pre_pos).c_str());
    payload = str.substr(pos + 1, str.size() - pos - 3);
}

std::string Message::ToString()
{
    return std::to_string(type) + " " + std::to_string(from) + " " + std::to_string(to) + " " + payload + "\r\n";
}

Node::Node(uint8_t _id, std::string _ip, uint16_t _port)
{
    id = _id;
    _state = INIT;
    loop = handy::EventLoop::GetInstance();
    server = new handy::TcpServer(_ip, _port);
    server->OnConnMsg([this](handy::TcpConn *conn,const std::string &res) {
        Message m(res);
        switch (m.type)
        {
        case T_INT:
            /* code */
            break;
        case T_INTACK:
            /* code */
            break;
        case T_CHK:
            /* code */
            break;
        case T_BLK:
            if(this->_state == MINE) {

            }
            break;
        case T_BC_REQ:
            /* code */
            break;
        case T_BC_RES:
            /* code */
            break;
        default:
            break;
        }
    });
    server->Bind();
    
    this->mine_check_timer = loop->CreateRepeatTask([this]() {
        time_t cur;
        time(&cur);
        auto t = localtime(&cur);
        if(t->tm_min % 2 == 0){
            if(this->mine_timeout_timer == NULL) {
                this->mine_timeout_timer = this->loop->CreateDelayTask([this]() {
                    this->_state = IDLE;
                }, 60 * 1000);
            }
            std::cout << "挖矿开始" << std::endl;
            this->_state = MINE;
            this->MineBlock();
        }
    }, 5000);
}

void Node::MineBlock()
{
    auto chk = this->checks;
    this->checks.clear();
    std::string data;
    for(auto ch : chk) {
        data += ch.ToString() + "#";
    }
    if(!data.empty()) {
        data.pop_back();
        bc->AddBlock(Block(bc->len() + 1, data));
        auto res = bc->_GetLastBlock().ToString();
        this->Broadcast(res, T_BLK);
    }
}

void Node::Broadcast(const std::string &data, uint8_t type)
{
    for(auto nei : peers) {
        Message msg(this->id, nei.first, type, data);
        auto m = msg.ToString();
        handy::TcpClient cli(nei.second.first, nei.second.second, this->ip);
        cli.send(m);
    }
}

void Node::init(bool is_genesis)
{
    bc = new BlockChain(is_genesis);
}

void Node::run()
{
    loop->RunLoop();
}

void Node::AddPeer(uint8_t _id, std::string _ip, uint16_t _port)
{
    this->peers[_id] = std::pair<std::string, uint16_t>(_ip, _port);
}

void Node::AddCheck(Check chk)
{
    this->checks.push_back(chk);
    Broadcast(chk.ToString(), T_CHK);
}