import sys
import socket

from threading import Thread

class AcceptingClientConnection(Thread):

    def __init__(self, port, function_to_run):
        Thread.__init__(self)
        self.__is_closing = False
        self.__port = port

        self.server_socket = self.__initializing_socket(port)

        self.function_to_run = function_to_run

    def __initializing_socket(self, port):
        try:
            server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as error_message:
            print("[FAIL - AcceptingClientConnection] Can't create Accepting Socket - {} {}".format(str(error_message[0]), error_message[1]))
            sys.exit(1)

        print("[OK - AcceptingClientConnection] Accepting Socket correctly created")

        try:
            server_socket.bind(('', port))
            print("[OK - AcceptingClientConnection] Accepting Socket correctly bounded to port " + str(port))
        except socket.error as error_message:
            print("[Fail - AcceptingClientConnection] Can't bind Accepting Socket to port {} - {} {}".format(str(port), str(error_message[0]), error_message[1]))
            sys.exit(1)

        return server_socket

    def run(self):
        self.server_socket.listen(128)

        while not self.__is_closing:
            print("\n[AcceptingClientConnection] Waiting for Clients connection attempts ...")
            try:
                client_connection, client_address = self.server_socket.accept()
                print("[OK - AcceptingClientConnection] Accepting Socket correctly Connected to {} : {}".format(client_address[0], str(client_address[1])))

                self.function_to_run(client_connection)
            except socket.error as msg:
                print("[FAIL] Error while connection socket")

    def close(self):
        if not self.__is_closing:
            self.__is_closing = True

            self.accepting_socket.shutdown(socket.SHUT_RDWR)
            self.accepting_socket.close()

            print("[AcceptingClientConnection] Socket correctly closed")
