#include "client.h"

Client::Client(std::string _id, const Server& _server)
    : id { _id }
    , server { &_server }
{
}
//--------------------------------------------------------
std::string Client::get_id() { return id; }
