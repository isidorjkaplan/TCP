#pragma once

#include <ctime>
#include <iostream>
#include <errno.h>
#include <sys/socket.h>  //available in all linux systems
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>


//TO DO: As a sender, I know the port I should put my data on
//TO DO: As a receiver, I know the port and IP I should send to

//Constructor with nothing sent is the sender, it choose a port to send on
//Constructor with port and IP sent is the

class Connection {
    public:
        //This is the constructor. It initializes the IP addresses and port numbers for the sending of messages and receival of responses 
        Connection(const char *ip, const int port, const bool is_server);
        ~Connection();
        void setDebug();
        bool isDebug();
        bool isServer();


        //An internal helper function to send to a socket
        void sendSocket(int, unsigned char *);
        //An internal helper function to receive from a socket
        void readSocket(int *received_length, unsigned char *message_buffer, bool timeout);
        //Wait until there is the expected number of bytes in the buffer before reading


    protected:
        bool waitForRead(int, int);
        void readMessage(void*, int);
        long int getTimeMs();
        bool hasBytesWaiting();

        int port;
        bool is_server;
        bool error;
        bool debug;

        int sock;
        
        int timeout;//In ms
        char *end_of_message;
        int verify_len;
        //IP addresses and port numbers of senders and receivers
        struct sockaddr_in ip_address;
};