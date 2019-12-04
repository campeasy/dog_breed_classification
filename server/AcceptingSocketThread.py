import os
import signal
import sys
import time
import socket

from threading import Thread
from concurrent.futures import ThreadPoolExecutor

HOST = '' # all availabe interfaces
PORT = 9999 # arbitrary non privileged port 

class AcceptingSocketThread(Thread):
    def __init__(self, response):
        Thread.__init__(self)
        self.is_closing = False
        try:
            self.connection_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as msg:
            print("Could not create socket. Error Code: ", str(msg[0]), "Error: ", msg[1])
            sys.exit(1)

        print("[-] Socket Created")

        # bind socket
        try:
            self.connection_socket.bind((HOST, PORT))
            print("[-] Socket Bound to port " + str(PORT))
        except socket.error as msg:
            print("Bind Failed. Error Code: {} Error: {}".format(str(msg[0]), msg[1]))
            sys.exit(1)

        self.pool = ThreadPoolExecutor(200)
        self.response = response

    def run(self):
        self.connection_socket.listen(128)
        print("Listening...")
        while not self.is_closing:
            try:
                conn, addr = self.connection_socket.accept()
                print("[-] Connected to " + addr[0] + ":" + str(addr[1]))
                self.pool.submit(self.response, (conn))
            except socket.error as msg:
                print("Stop listening")

        print("Closing clients connections...")
        self.pool.shutdown(wait=True)

    def close(self):
        print("Closing...")
        self.is_closing = True
        self.connection_socket.shutdown(socket.SHUT_RDWR)
        self.connection_socket.close()
