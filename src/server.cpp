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