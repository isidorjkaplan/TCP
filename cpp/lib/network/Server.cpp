#include "network/Connection.h"
#include "network/Server.h"

Server::Server(const char *ip, const int port) : Connection(ip, port, true) {
    listener = socket(AF_INET, SOCK_STREAM, 0);
    //Print the output data
    if (listener == -1) {
        std::cerr << "Cannot create a socket" << std::endl;
        error = true;
        return;
    } else if (debug) {
        std::cout << "Created socket for sending, descriptor = " << sock << std::endl;
    }

    // Forcefully attaching socket to the correct IP/port by binding it
    if (bind(listener, (struct sockaddr *)&ip_address,
             sizeof(ip_address))<0) {
        perror("bind failed");
        error=true;
        return;
    } else if (debug) {
        std::cout << "Server socket bound" << std::endl;
    }

    //Put the server into listening mode for incoming connections and then print the result of that
    if (listen(listener, 3) < 0) {
        perror("Error setting listener to listen");
        exit(EXIT_FAILURE);
    } else if (debug) {
        std::cout << "Server socket now listening" << std::endl;
    }
    //Assign the port in the event that we got assigned a different port (such as if port=0 starting)

    //this->port = ip_address.sin_port;
    //if (debug) std::cout << "Port = " << this->port << std::endl;
}

Server::~Server() {
    close(listener);
}

void Server::acceptClient() {
    if ((sock = accept(listener, (struct sockaddr *)&ip_address,
                             (socklen_t*)&ip_address))<0) {
        perror("Error Accpeting");
        error=true;
    } else if (debug) {
        std::cout << "Successfully accepted connection!" << std::endl;
    }
}
