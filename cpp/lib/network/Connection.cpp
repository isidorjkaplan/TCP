#include <sys/ioctl.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <assert.h>

#include "network/Connection.h"

Connection::Connection(const char *ip, const int port, const bool is_server) {
    error=false;
    debug=true;
    timeout = 10;
    end_of_message = "END";

    verify_len = strlen(end_of_message)+1;
    this->port = port;
    this->is_server = is_server;

    memset((char *) &ip_address, 0, sizeof(ip_address));
    ip_address.sin_addr.s_addr = inet_addr(ip);
    ip_address.sin_family = AF_INET;
    ip_address.sin_port = htons(port);
}

Connection::~Connection() {
    //TODO
    close(sock);
}

///ReadSocket will receive a single packet from the socket and store the size in the input buffer
void Connection::readSocket(int *received_length, unsigned char *message_buffer, bool timeout) {
    //The format of all packets [4 bytes specifying message length] => [N bytes of the message] => [end_of_message string]
    //Before trying to read make sure that there are enough bytes received to read a packet (no partial packets)
    if (timeout){
        if (!waitForRead(sizeof(int), timeout)) {
            //It timed out
            //printf("Timed out while reading socket from TCP!\n");
            *received_length=0;
            return;
        }
    } else {
        waitForRead(sizeof(int),0);
    }
    //Read the first 4 bytes of the message and store them in the received length variable
    readMessage(received_length, sizeof(int));
    //Once we know how many bytes we are expecting wait until that many bytes are in the buffer and then read them into the message buffer
    waitForRead(*received_length, 0);
    readMessage(message_buffer, *received_length);

    //After that create a char array to store the verify token at the end of the message
    char verify[verify_len];
    //Wait until there are enough bytes in the buffer and then read the verify token from the array
    waitForRead(verify_len, 0);
    readMessage(verify, verify_len);
    verify[verify_len-1]='\0';
    //If the verify token does not equal what it is expected to equal then the packet sent incorrectly and the program should terminate
    if (strcmp(verify, end_of_message) == 0) {
        //printf("Packet Verified with end_of_message\n");
    } else {
        printf("Packet was not verified properly!\nVERIFY=%s\n", verify);
        exit(EXIT_FAILURE);
    }
}


///Program to read a specified number of bytes into the variable passed with a known length.
void Connection::readMessage(void*buffer, int len) {
    int result = recv(sock, buffer, len, MSG_WAITALL);
    if (result < 0) {
        printf("An error occured reading a message!\n");
        exit(EXIT_FAILURE);
    } else if (result != len) {
        printf("Got less then full packet %d/%d!\n", result,len);
    }
}

///This function sends a message through the socket with our custom protocol
void Connection::sendSocket(int message_size, unsigned char *message) {
    //The total length of data being sent is the integer head, the message itself, and the verify token at the end
    int buffer_size = sizeof(int) + message_size + verify_len;
    //std::cout << "Total size = " << buffer_size << std::endl;
    unsigned char buffer[buffer_size];
    //First add the message size, then copy the message (offset by the header size), and then add the verify token (properly offset)
    for (int i = 0; i < sizeof(int); i++) {buffer[i] = ((unsigned char*)&message_size)[i];}
    for (int i = 0; i < message_size; i++){buffer[sizeof(int)+i]=message[i];}
    for (int i = 0; i < verify_len; i++){buffer[sizeof(int)+message_size+i]=end_of_message[i];}
    //Send the fully wrapped packet through the stream
    int result = write(sock, buffer, buffer_size);
    if (result <= 0) {
        printf("Failed to write with error code %d\n", result);
    }
}


///A function that causes the thread to wait until there are enough bytes in the input buffer to read. This is used to prevent partial packet reading.
bool Connection::waitForRead(int bytes, int timeout_ms) {
    int bytesAvailable = 0;
    long int start = getTimeMs();
    while (bytesAvailable < bytes) {
        //If timeout_ms is 0 then we ignore timeout
        if (timeout_ms >= 0 && getTimeMs() - start > timeout_ms) {
            return false;
        }
        ioctl(sock, FIONREAD, &bytesAvailable);
        //printf("Waiting for %d/%d bytes\n", bytesAvailable, bytes);
    }
    return true;
}

long int Connection::getTimeMs() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

bool Connection::hasBytesWaiting() {
    int bytesAvailable;
    ioctl(sock, FIONREAD, &bytesAvailable);
    return bytesAvailable > 0;
}