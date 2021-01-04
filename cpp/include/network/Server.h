#pragma once

#include "network/Connection.h"

class Server : public Connection {
    public:
        Server(const char *ip, const int port);
        ~Server();
        void acceptClient();
    private:
        int listener;
};