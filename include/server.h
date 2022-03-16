#ifndef SERVER_H
#define SERVER_H

#include "client.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

class Client;

class Server {
public:
    Server(); // Constructor
    std::shared_ptr<Client> add_client(std::string id);
    std::shared_ptr<Client> get_client(std::string id) const;
    double get_wallet(std::string id) const;
    static bool parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value);
    bool add_pending_trx(std::string trx, std::string signature) const;
    size_t mine();

private:
    std::map<std::shared_ptr<Client>, double> clients;
};

inline std::vector<std::string> pending_trxs;

#endif // SERVER_H