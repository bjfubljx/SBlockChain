#include "node.hh"
#include <cstdlib>
#include <sstream>
#include <ctime>


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

Node::Node(uint8_t _id, std::string _ip, int _port)
{
    id = _id;
    _state = INIT;
    loop = handy::EventLoop::GetInstance();
    server = new handy::TcpServer(_ip, _port);
    server->OnConnMsg([this](handy::TcpConn *conn,const std::string &res) {
        
    });
    server->Bind();
    
    this->mine_check_timer = loop->CreateRepeatTask([this]() {
        time_t cur;
        time(&cur);
        auto t = localtime(&cur);
        if(t->tm_min % 2 == 0){
            this->_state = MINE;
            if(this->mine_timeout_timer == NULL) {
                
            }
        }
    }, 5000);
}