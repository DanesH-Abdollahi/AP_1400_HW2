#ifndef CLIENT_H
#define CLIENT_H

#include "crypto.h"
#include "server.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Server;

class Client {
public:
    Client(std::string id, const Server& server); // Constructor
    std::string get_id();
    std::string get_publickey();
    double get_wallet();
    std::string sign(std::string txt) const;
    bool transfer_money(std::string receiver, double value);
    // size_t generate_nonce();

private:
    Server const* const server;
    const std::string id;
    std::string public_key;
    std::string private_key;
};

#endif // CLIENT_H