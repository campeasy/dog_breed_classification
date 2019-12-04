import os
import io
import sys
import signal
import socket
import functools

from PIL import Image

from DogBreedClassifier import DogBreedClassifier
from AcceptingSocketThread import AcceptingSocketThread

def response(dbc, client_connection):
    print("Responding...")
    file_size = client_connection.recv(8)
    file_size = int.from_bytes(file_size, byteorder='little', signed=True)
    print("File size to reveice {} bytes".format(file_size))
    bytes_received = 0
    file_received = bytearray()
    while bytes_received < file_size:
        bytes_to_receive = min(file_size - bytes_received, 1024)
        file_received.extend(client_connection.recv(bytes_to_receive))
        bytes_received += bytes_to_receive
    print("File received, {} bytes".format(len(file_received)))
    img = Image.open(io.BytesIO(file_received))
    print("Image Size: {}".format(img.size))
    dbc.classify(img)
    client_connection.shutdown(socket.SHUT_RDWR)
    client_connection.close()
    print("Client Connection Closed")

def handler(ast, signum, frame):
    print()
    ast.close()

def main():
    dbc = DogBreedClassifier(labels_path="labels.txt")
    resp = functools.partial(response, dbc)
    ast = AcceptingSocketThread(resp)

    signal.signal(signal.SIGINT, functools.partial(handler, ast))

    ast.start()
    ast.join()

if __name__ == "__main__":
    main()
