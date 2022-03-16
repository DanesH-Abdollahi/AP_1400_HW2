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
    std::string get_id() const;
    std::string get_publickey() const;
    double get_wallet() const;
    std::string sign(std::string txt) const;
    bool transfer_money(std::string receiver, double value) const;
    size_t generate_nonce() const;

private:
    Server const* const server;
    const std::string id;
    std::string public_key;
    std::string private_key;
};

static void show_wallets(const Server& server)
{
    auto ptr { (std::map<std::shared_ptr<Client>, double>*)&server };
    std::cout << std::string(20, '*') << std::endl;
    for (auto client = ptr->begin(); client != ptr->end(); ++client)
        std::cout << client->first->get_id() << " : " << client->second << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}

#endif // CLIENT_H