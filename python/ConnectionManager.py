import socket
import copy


class ConnectionManagerTCP:
    def __init__(self, ip='127.0.0.1', is_server=True, port=0, debug=True, verify='END', header_size=4, byteorder='little', encoding='unicode_escape', timeout=1000):
        self.ip = ip
        self.is_server = is_server
        self.port = port
        self.debug = debug
        self.verify = verify
        self.byteorder = byteorder
        self.encoding = encoding
        self.header_size = header_size
        self.sock = None
        self.timeout = timeout
        self.verify_bytes = bytes(verify, 'ascii') + b'\x00'

        if self.is_server:
            # create an INET, STREAMing socket
            self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            #Bind to the socket. Normally we use localhost, ca use ip='' to allow from anywhere (but we ushally intend the agent to be on the same device as the CPP so just use localhost)
            self.serversocket.bind((self.ip, self.port))
            self.port = self.serversocket.getsockname()[1]
            #1 is the number of connections. Since this is intended to be dedicated TCP for python to cpp only 1 is needed
            self.serversocket.listen(1)
            if self.debug:print("Starting Python TCP Server at %s:%d" % (self.ip, self.port))
        else:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def get_port(self):
        return self.port

    def get_ip(self):
        return self.ip

    def get_connection(self):
        # accept connections from outside
        (self.sock, self.clientaddress) = self.serversocket.accept()
        if self.debug:print("Accepted connection from: " + str(self.clientaddress))

    def connect(self):
        self.sock.connect((self.ip, self.port))
        if self.debug:print("Connected to " + str(self.ip) + ":" + str(self.port))

    def setup(self):
        if self.debug:print("Setup called!\n")
        if self.is_server:
            #If it is the server, accept the connection
            self.get_connection()
        else:
            #If it is the client, connect ot the server
            self.connect()
        self.sock.settimeout(self.timeout)

    #A pfunction to send a bytearray msg across the socket. If it recieves a string it will auto-convert to a byte-array
    def send(self, msg):
        if self.sock == None:self.setup()
        packet = bytearray()
        #Encode the size as a 4-byte integer
        msg_len_bytes = len(msg).to_bytes(self.header_size, byteorder = self.byteorder, signed=True)
        packet.extend(msg_len_bytes)
        #Add the encoded message
        packet.extend(msg)
        #The verify bytes is the verify token at the end of the C message that we last received. This is to ensure formatting is correct
        #assert self.verify_bytes is not None
        #Add the verify packet
        packet.extend(self.verify_bytes)
        #if self.debug:print("Sending: " + str(packet))
        #A function that sends all the packets, keeps trying util all bytes are sent and throws an error if something goes wrong
        self.sock.sendall(packet)

    #A function to receive the next packet from a socket
    def receive(self):
        if self.sock==None:self.setup()
        #Get the size from the packet header as the first 4 bytes
        size = int.from_bytes(self.myreceive(self.header_size), byteorder=self.byteorder, signed=True)
        #Obtain the actual msg as a bytearray
        msg = self.myreceive(size)
        #The reason that I have the +1 is for the null-terminating-character which is sent from C
        self.verify_bytes = self.myreceive(len(self.verify)+1)
        #The reason I cut it to len(self.verify) is that in python we do not need the null terminating character
        verify = self.verify_bytes[:len(self.verify)].decode(self.encoding)
        assert str(verify) == str(self.verify)
        return msg

    #A helper function to get a certain number of bytes from the socket
    def myreceive(self, num_bytes):
        chunks = []
        bytes_recd = 0
        while bytes_recd < num_bytes:
            #Blocking by default
            chunk = self.sock.recv(min(num_bytes - bytes_recd, 2048))
            if chunk == b'':
                raise RuntimeError("socket connection broken")
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        return b''.join(chunks)

    def __exit__(self, exc_type, exc_value, traceback):
        self.cleanup()

    def cleanup(self):
        print("Cleaning up Python TCP Connection!")
        self.sock.close()
        if self.is_server:self.serversocket.close()
