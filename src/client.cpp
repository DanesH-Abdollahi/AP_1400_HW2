#include "client.h"

Client::Client(std::string _id, const Server& _server) // Constructor
    : id { _id }
    , server { &_server }
{
    // Generating Public_Key & Private_Key For Added Client
    crypto::generate_key(public_key, private_key);
}
//--------------------------------------------------------
// Defining get_id function
std::string Client::get_id() const { return id; }
//--------------------------------------------------------
// Defining get_wallet function
double Client::get_wallet() const
{
    // Use The get_wallet Server Class Method
    return server->get_wallet(id);
}
//--------------------------------------------------------
// Defining get_publickey function
std::string Client::get_publickey() const
{
    return public_key;
}
//--------------------------------------------------------
// Defining sign function
std::string Client::sign(std::string txt) const
{
    std::string signature = crypto::signMessage(private_key, txt);
    return signature;
}
//--------------------------------------------------------
// Definign transfer_money function
bool Client::transfer_money(std::string receiver, double value) const
{
    // Generating Transaction String From Sender & Receiver ID & The Value
    std::string transaction { id + '-' + receiver + '-' + std::to_string(value) };
    // Generating Signature for The Transaction
    std::string signature { sign(transaction) };
    // Athenteficartion The Transaction
    bool Athen { server->add_pending_trx(transaction, signature) };

    return Athen;
}
//--------------------------------------------------------
// Defining generate_nonce function
size_t Client::generate_nonce() const
{ // Generate a Random Number (Integer)
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<double> distr(0, 1e3); // Range is 0 to 1e3
    return static_cast<size_t>(distr(eng));
}
//----------------------------------The End--------------------------------------
