#include <iostream>
using namespace std;
#include "network/Server.h"

int main() {
    cout << "Starting C++ Server" << endl;
    cout << "Enter (Server) Port: ";
    int port;
    cin >> port;
    Server server("127.0.0.1", port);
    server.acceptClient();

    string action = "send";
    while(action == "send" || action == "recv") {
        cout << "Enter an action (send | recv): ";
        cin >> action;
        if (action == "send") {
            cout << "Enter a message: ";
            string message;

            std::cin.clear();
            std::cin.ignore(100,'\n');
            std::getline(cin,message);
            server.sendSocket(message.size() + 1, (unsigned char *)message.c_str());
            //cout << "[" << message.size() << "] " << message << " == " << message.c_str() << endl;
        } else if (action == "recv") {
            unsigned char message[100];
            int size;
            server.readSocket(&size, &message[0], false);
            cout << message << endl;;
        }
    }
    return 0;
}