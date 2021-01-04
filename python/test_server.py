from ConnectionManager import ConnectionManagerTCP 

if __name__ == "__main__":
    print("Starting Python Program")
    mode = input("Enter (server | client): ")
    if mode == "server":
        #Create Server
        conn = ConnectionManagerTCP()
        #Accept connection from client
    elif mode == "client":
        conn = ConnectionManagerTCP(is_server=False, port=int(input("Enter port: ")))
    else: 
        print("Invalid mode!")
        assert False
    
    conn.setup()

    while True:
        mode = input("Enter an action (send | recv): ")
        if (mode == "recv"):
            recv = conn.receive()
            print("Recieved: " + str(recv))
        elif (mode == "send"):
            message = input("Enter a string: \0").encode("ascii")
            print("Sending: " + str(message))
            conn.send(message)