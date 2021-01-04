#pragma once


#include "network/Connection.h"
#include "network/Client.h"
#include <sys/ioctl.h>

class Client : public Connection {
    public:
        Client(const char *ip, const int port);
        
};