#include "server.h"

Server::Server() // Constructor (default)
    : clients {}
{
}
//-----------------------------------------------------------------------------------
// Defining add_client function
std::shared_ptr<Client> Server::add_client(std::string _id)
{
    for (auto jj = clients.begin(); jj != clients.end(); jj++)
        if (jj->first->get_id() == _id) {
            // if The ID is already exist -> Generate a Random Number (integer)
            std::random_device rd;
            std::default_random_engine eng(rd());
            std::uniform_real_distribution<double> distr(1000, 9999); // Range is 1000 to 9999
            _id = _id + std::to_string(static_cast<int>(distr(eng)));
        }
    // Definig The New Client With Specific ID and Server
    Client new_client { _id, *this };
    // A shared_pointer which points to The New_Client
    std::shared_ptr<Client> client_pointer { std::make_shared<Client>(new_client) };
    // Insert The Client Pointer & it's Wallet to The Server Private Member (clients)
    clients.insert(std::pair<std::shared_ptr<Client>, double>(client_pointer, 5.0));

    return client_pointer;
}
//------------------------------------------------------------------------------------
// Defining get_client function
std::shared_ptr<Client> Server::get_client(std::string _id) const
{
    // Exploring The client's to find The client with specifi ID
    for (auto ii = clients.begin(); ii != clients.end(); ii++) {
        if (ii->first->get_id() == _id)
            // retunrn client's pointer
            return ii->first;
    }
    // If The given ID does not exist , return NULL Pointer
    return nullptr;
}
//------------------------------------------------------------------------------------
// Defining get_wallet function
double Server::get_wallet(std::string _id) const
{
    // Exploring The client's to find The client with specifi ID
    for (auto jj = clients.begin(); jj != clients.end(); jj++) {
        if (jj->first->get_id() == _id)
            // retunrn client's Wallet Money
            return jj->second;
    }
    // If The given ID does not exist , return a Logic Error
    throw std::logic_error("ID Does Not Exist !");
}
//----------------------------------------------------------------------------------
// Definig parse_trx function
bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value)
{
    // Counting The '-' char in given String (trx)
    long int count { std::count(trx.begin(), trx.end(), '-') };
    // If the given String (trx) has not 2 '-' , throw a Runtime Error
    if (count != 2)
        throw std::runtime_error(" Runtime Error! ");
    // Finding Each Part (Sender, Receiver , Value) Of The given String (TRX)
    size_t first = trx.find('-');
    size_t second = trx.find('-', first + 1);
    sender = trx.substr(0, first);
    receiver = trx.substr(first + 1, second - first - 1);
    value = std::stod(trx.substr(second + 1));
    // If the given String (trx) is written in the standard waay , return true
    return true;
}
//----------------------------------------------------------------------------------
// Defining add_pending_trx
bool Server::add_pending_trx(std::string trx, std::string signature) const
{
    std::string sender {}, receiver {};
    double value {};
    Server::parse_trx(trx, sender, receiver, value);

    std::shared_ptr<Client> client_sender { get_client(sender) };
    std::shared_ptr<Client> client_receiver { get_client(receiver) };

    double sender_money { get_wallet(sender) };
    std::string sender_public_key { client_sender->get_publickey() };
    // Transaction Athentification
    bool authentic = crypto::verifySignature(sender_public_key, trx, signature);
    // Check The Existance Of The Sender & Receiver ID In The Server
    if ((client_sender == nullptr) || (client_receiver == nullptr)) {
        return false;
    }
    // if Athentification gone well & The sender wallet has the money of the transaction
    if (authentic && (sender_money > value)) {
        pending_trxs.push_back(trx);
        return true;
    }
    // if Athentification failed OR The sender wallet has'nt the money of the transaction
    return false;
}
//----------------------------------------------------------------------------------
// Defining The mine function
size_t Server::mine()
{
    // Defining Senders , Receivers & Values vectors to store The pending tarnsaction's Property
    std::vector<std::string> Senders {}, Receivers {};
    std::vector<double> Values {};
    std::string mempool {};
    std::string sender {}, receiver {};
    double value {};
    // Constructing The "mempopl" String
    for (auto trx : pending_trxs) {
        mempool = mempool + trx;
        parse_trx(trx, sender, receiver, value);
        // Insert Transaction's Property to Defined Vectors
        Senders.push_back(sender);
        Receivers.push_back(receiver);
        Values.push_back(value);
    }

    std::vector<std::shared_ptr<Client>> Client_Senders {};
    std::vector<std::shared_ptr<Client>> Client_Receivers {};
    // Store Senders & Receivers Pointer in Defined Vectors
    for (auto i : Senders)
        Client_Senders.push_back(get_client(i));

    for (auto i : Receivers)
        Client_Receivers.push_back(get_client(i));

    // Starting The Mining Process
    for (auto j : clients) {
        size_t nonce_temp { j.first->generate_nonce() };
        std::string hash { crypto::sha256(mempool
            + std::to_string(nonce_temp)) };

        // Getting Random Nonce From All Server Clients
        if (hash.substr(0, 10).find("000") != std::string::npos) {
            std::cout << "MINER is: " + j.first->get_id() << std::endl;
            // Rewarding The Miner
            clients[j.first] += 6.25;
            size_t counter {};
            // Doing Pending Transaction's
            for (auto k : Client_Senders) {
                clients[k] -= Values[counter];
                counter++;
            }
            counter = 0;
            for (auto k : Client_Receivers) {
                clients[k] += Values[counter];
                counter++;
            }
            // Clear The Pending Transaction's Vector
            pending_trxs.clear();
            // Return The Nonce Which Mined The Block
            return nonce_temp;
        }
    }
    return mine();
}
//----------------------------------The End----------------------------------------