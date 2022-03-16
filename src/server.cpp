#include "server.h"

Server::Server()
    : clients {}
{
}

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

bool Server::add_pending_trx(std::string trx, std::string signature) const
{

    std::string sender {}, receiver {};
    double value {};
    Server::parse_trx(trx, sender, receiver, value);

    std::shared_ptr<Client> client_sender { get_client(sender) };
    std::shared_ptr<Client> client_receiver { get_client(receiver) };

    double sender_money { get_wallet(sender) };
    std::string sender_public_key { client_sender->get_publickey() };
    bool authentic = crypto::verifySignature(sender_public_key, trx, signature);

    if ((client_sender == nullptr) || (client_receiver == nullptr)) {
        return false;
    }

    if (authentic && (sender_money > value)) {
        pending_trxs.push_back(trx);
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------

size_t Server::mine()
{
    std::vector<std::string> Senders {}, Receivers {};
    std::vector<double> Values {};
    std::string mempool {};
    std::string sender {}, receiver {};
    double value {};

    for (auto trx : pending_trxs) {
        mempool = mempool + trx;
        parse_trx(trx, sender, receiver, value);
        Senders.push_back(sender);
        Receivers.push_back(receiver);
        Values.push_back(value);
    }

    std::vector<std::shared_ptr<Client>> Client_Senders {};
    std::vector<std::shared_ptr<Client>> Client_Receivers {};
    for (auto i : Senders)
        Client_Senders.push_back(get_client(i));

    for (auto i : Receivers)
        Client_Receivers.push_back(get_client(i));

    for (auto j : Client_Senders) {
        size_t nonce_temp { j->generate_nonce() };
        std::string hash { crypto::sha256(mempool
            + std::to_string(nonce_temp)) };

        if (hash.substr(0, 10).find("000") != std::string::npos) {
            std::cout << "MINER is: " + j->get_id() << std::endl;
            clients[j] += 6.25;
            size_t counter {};
            counter = 0;
            for (auto k : Client_Senders) {
                clients[k] -= Values[counter];
                counter++;
            }
            counter = 0;
            for (auto k : Client_Receivers) {
                clients[k] += Values[counter];
                counter++;
            }
            pending_trxs.clear();
            return nonce_temp;
        }
    }
    return mine();
}

//----------------------------------The End--------------------------------------