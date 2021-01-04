#include "network/Connection.h"
#include "network/Client.h"
Client::Client(const char *ip, const int port) : Connection(ip, port, false) {
    if (debug) std::cout << "Setting up sender!" << std::endl;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sock, (struct sockaddr *)&ip_address, sizeof(ip_address)) < 0)
    {
        printf("\nConnection Failed \n");
        error=true;
        return;
    }
    else if (debug) std::cout << "Successfully connected to server!" << std::endl;  
}

