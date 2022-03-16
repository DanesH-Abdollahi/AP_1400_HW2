#include "server.h"

Server::Server() { }

//-----------------------------------------------------------------------------------

std::shared_ptr<Client> Server::add_client(std::string _id)
{
    for (auto jj = clients.begin(); jj != clients.end(); jj++)
        if (jj->first->get_id() == _id) {
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_real_distribution<double> distr(1000, 9999); // Range is 1000 to 9999
            _id = _id + std::to_string(static_cast<int>(distr(eng)));
        }

    Client new_client { _id, *this };
    std::shared_ptr<Client> client_pointer { std::make_shared<Client>(new_client) };
    clients.insert(std::pair<std::shared_ptr<Client>, double>(client_pointer, 5.0));

    return client_pointer;
}
//------------------------------------------------------------------------------------

std::shared_ptr<Client> Server::get_client(std::string _id) const
{
    for (auto ii = clients.begin(); ii != clients.end(); ii++) {
        if (ii->first->get_id() == _id)
            return ii->first;
    }
    return nullptr;
}

//------------------------------------------------------------------------------------

double Server::get_wallet(std::string _id) const
{
    for (auto jj = clients.begin(); jj != clients.end(); jj++) {
        if (jj->first->get_id() == _id)
            return jj->second;
    }
    throw std::logic_error("ID Does Not Exist !");
}

//-----------------------------------------------------------------------------------

// void show_wallets(const Server& server)
// {
//     std::shared_ptr<Server> ptr = &server ;

//     class FYM {
//     public:
//         std::map<std::shared_ptr<Client>, double> FYM_clients;
//     };
//     reinterpret_cast<FYM*>( &server ) ;

//     // std::shared_ptr<Server> ptr = (std::shared_ptr<Server>)&server;
//     std::cout << FYM_client.first << std::endl;

//     //     Server Temp { server };
//     //     std::cout << std::string(20, '*') << std::endl;
//     //     for (const auto& client : server.clients)
//     //         std::cout << client.first->get_id() << " : " << client.second << std::endl;
//     //     std::cout << std::string(20, '*') << std::endl;
//     //
// }

//----------------------------------------------------------------------------------

bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value)
{
    long int count { std::count(trx.begin(), trx.end(), '-') };
    if (count != 2)
        throw std::runtime_error(" Runtime Error! ");
    size_t first = trx.find('-');
    size_t second = trx.find('-', first + 1);
    sender = trx.substr(0, first);
    receiver = trx.substr(first + 1, second - first - 1);
    value = std::stod(trx.substr(second + 1));

    return true;
}

//----------------------------------------------------------------------------------

bool Server::add_pending_trx(std::string trx, std::string signature)
{
    size_t first = trx.find('-');
    size_t second = trx.find('-', first + 1);
    std::string sender = trx.substr(0, first);
    double value = std::stod(trx.substr(second + 1));
    std::string public_key {};
    double money {};

    for (auto ii = clients.begin(); ii != clients.end(); ii++)
        if (ii->first->get_id() == sender) {
            public_key = ii->first->get_publickey();
            money = ii->second;
        }

    bool authentic = crypto::verifySignature(public_key, trx, signature);

    if (authentic && money >= value) {
        pending_trxs.push_back(trx);
        return true;
    } else
        return false;
}

//----------------------------------------------------------------------------------
