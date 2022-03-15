#include "server.h"
Server::Server()
{
}

std::shared_ptr<Client> Server::add_client(std::string _id)
{
    Client new_client { _id, *this };
    std::shared_ptr<Client> client_pointer { std::make_shared<Client>(new_client) };
    clients.insert(std::pair<std::shared_ptr<Client>, double>(client_pointer, 5.0));

    return client_pointer;
}

std::shared_ptr<Client> Server::get_client(std::string _id)
{
    for (std::map<std::shared_ptr<Client>, double>::iterator ii = clients.begin(); ii != clients.end(); ++ii) {
        if (ii->first->get_id() == _id)
            return ii->first;
    }
    throw std::logic_error("ID Does Not Exist !");
}
