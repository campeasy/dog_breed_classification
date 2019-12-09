import sys
import socket

from threading import Thread
from concurrent.futures import ThreadPoolExecutor

# ClientConnectionHandler must be treated as a Singleton Class
class ClientConnectionHandler(Thread):

    def __init__(self, port, function_to_run):
        Thread.__init__(self)

        self.__is_closing = False
        self.__port = port
        self.__server_socket = self.__initializing_socket(port)

        self.__function_to_run = function_to_run
        self.__thread_pool = ThreadPoolExecutor(200)

    def __initializing_socket(self, port):
        try:
            server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as error_message:
            print("[FAIL - ClientConnectionHandler] Can't create Accepting Socket - {} {}".format(str(error_message[0]), error_message[1]))
            sys.exit(1)

        print("[OK - ClientConnectionHandler] Accepting Socket correctly created")

        try:
            server_socket.bind(('', port))
            print("[OK - ClientConnectionHandler] Accepting Socket correctly bounded to port " + str(port))
        except socket.error as error_message:
            print("[FAIL - ClientConnectionHandler] Can't bind Accepting Socket to port {} - {} {}".format(str(port), str(error_message[0]), error_message[1]))
            sys.exit(1)

        return server_socket

    def run(self):
        queue_max_size = 128
        self.__server_socket.listen(queue_max_size)

        while not self.__is_closing:
            print("\n[ClientConnectionHandler] Waiting for Clients connection attempts ...")
            try:
                client_connection, client_address = self.__server_socket.accept()
                print("[OK - ClientConnectionHandler] New Socket connected with the Client at ({} : {})".format(client_address[0], str(client_address[1])))

                self.__thread_pool.submit(self.__function_to_run, (client_connection))
            except socket.error as msg:
                print("[FAIL - ClientConnectionHandler] Error while accepting Client connection")

        print("[ClientConnectionHandler] Completing the remaining requests and Closing ...")
        self.__thread_pool.shutdown(wait=True)

    def close(self):
        if not self.__is_closing:
            self.__is_closing = True

            self.__accepting_socket.shutdown(socket.SHUT_RDWR)
            self.__accepting_socket.close()

            print("[ClientConnectionHandler] Accepting Socket correctly closed")
