import sys
import socket

from threading import Thread

class AcceptingSocketThread(Thread):

    def __init__(self, port, response):
        Thread.__init__(self)

        self.is_closing = False
        self.port = port

        self.accepting_socket = self.__init_accepting_socket(port)

        self.response = response

    def __init_accepting_socket(self, port):
        try:
            accepting_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as error_message:
            print("Socket Creation Failed. Error Code: {} Error: {}".format(str(error_message[0]), error_message[1]))
            sys.exit(1)

        print("[-] Socket Created")

        try:
            accepting_socket.bind(('', self.port)) # all availabe interfaces
            print("[-] Socket Bound to port " + str(self.port))
        except socket.error as error_message:
            print("Socket Bind Failed. Error Code: {} Error: {}".format(str(error_message[0]), error_message[1]))
            sys.exit(1)
            
        return accepting_socket

    def run(self):
        self.accepting_socket.listen(128)
        print("Listening...")
        while not self.is_closing:
            try:
                client_connection, addr = self.accepting_socket.accept()
                print("[-] Connected to {} : {}".format(addr[0], str(addr[1])))
                self.response(client_connection)
            except socket.error as msg:
                print("Stop listening")

    def close(self):
        print("Closing...")
        if not self.is_closing:
            self.is_closing = True
            self.accepting_socket.shutdown(socket.SHUT_RDWR)
            self.accepting_socket.close()
