#include "client.h"

Client::Client(std::string _id, const Server& _server)
    : id { _id }
    , server { &_server }

{
    crypto::generate_key(public_key, private_key);
}

//--------------------------------------------------------

std::string Client::get_id() { return id; }

//--------------------------------------------------------

double Client::get_wallet()
{
    return server->get_wallet(id);
}

//--------------------------------------------------------

std::string Client::get_publickey() const
{
    return public_key;
}

//--------------------------------------------------------

std::string Client::sign(std::string txt) const
{
    std::string signature = crypto::signMessage(private_key, txt);
    return signature;
}

//--------------------------------------------------------

bool Client::transfer_money(std::string receiver, double value)
{
    std::string transaction { id + '-' + receiver + '-' + std::to_string(value) };
    std::string signature { sign(transaction) };

    return server->add_pending_trx(transaction, signature);
}

//--------------------------------------------------------

size_t Client::generate_nonce()
{
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<double> distr(0, 1e6); // Range is 0 to 1e6
    return static_cast<size_t>(distr(eng));
}

//--------------------------------------------------------