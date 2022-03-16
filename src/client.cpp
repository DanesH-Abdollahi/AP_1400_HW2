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
    std::string public_key1 {}, private_key1 {};
    crypto::generate_key(public_key1, private_key1);
    public_key = public_key1;
    private_key = private_key1;
    return public_key;
}

//--------------------------------------------------------

std::string Client::sign(std::string txt) const
{
    std::string signature = crypto::signMessage(private_key, txt);
    std::cout << "Haaaaaaaaaaamid to nabegheEEEE nakhaaaaaaaaaaaaaab2" << std::endl;
    return signature;
}

//--------------------------------------------------------

bool Client::transfer_money(std::string receiver, double value)
{
    Server server {};
    std::string transaction { id + '-' + receiver + '-' + std::to_string(value) };
    std::string signature { sign(transaction) };
    // bool athen { server.add_pending_trx(transaction, signature) };
    return server.add_pending_trx(transaction, signature);
}

//--------------------------------------------------------