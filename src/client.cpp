#include "client.h"

Client::Client(std::string _id, const Server& _server)
    : id { _id }
    , server { &_server }
{
}

//--------------------------------------------------------

std::string Client::get_id() { return id; }

//--------------------------------------------------------

double Client::get_wallet()
{
    return server->get_wallet(id);
}

//--------------------------------------------------------

std::string Client::get_publickey()
{
    std::string public_key {}, private_key {};
    crypto::generate_key(public_key, private_key);
    return public_key;
}

//--------------------------------------------------------