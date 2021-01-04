# TCP
A library with C++ and Python interfaces for communicating data over TCP. I created this after several machine learning projects I was working on required having core code in C++ but used python for the machine learning. I also included a simple set of C++ test executables and Python test script as a demo where you can send strings back and forth between C++ and Python using this library. 

# Using Demo
Note, you need to start 2 sessions, one "Server" and one "Client." The server and the client can both be C++ or Python depending on your use case. 
1. Navitate to the directory 'TCP/cpp' and then type the command 'make all'
2. To execute the C++ server or client demo execute the executables located at './cpp/bin/client' or './cpp/bin/server' and then you will be prompted for a port
3. To execute the Python server/client demo run the python script with 'python3 python/demo.py' and then you will be prompted to enter a mode "server" or "client"
4. Select either "recv" or "send." If you enter "recv" it will print the next message it recieves. If you enter "send" you will be prompted for a message to send. 
5. Repeat step 4 until you are done and then exit the program.

# File Structure
```
├── LICENSE
├── README.md
├── cpp
│   ├── Makefile
│   ├── include
│   │   └── network
│   │       ├── Client.h
│   │       ├── Connection.h  <- Server and client both extend from Connection which has base functions like "Send" and "Recieve"
│   │       └── Server.h
│   ├── lib
│   │   └── network
│   │       ├── Client.cpp
│   │       ├── Connection.cpp
│   │       └── Server.cpp
│   └── src
│       ├── client.cpp
│       └── server.cpp
└── python
    ├── ConnectionManager.py
    └── demo.py 
```
# Credits
I used a makefile from https://github.com/zakharykaplan/clip
I used some of this code for interfacing between C++ and Python for multiple projets while I was working with iQua research group. This code was adapted from the code I used there. 
