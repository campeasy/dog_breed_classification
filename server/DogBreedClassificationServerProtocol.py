import os
import io
import sys
import signal
import socket
import functools

from PIL import Image

from DogBreedClassifier import DogBreedClassifier

class DogBreedClassificationServerProtocol():

    def __init__(self, dog_breed_classifier):
        self.dog_breed_classifier = dog_breed_classifier

    def __get_image_file_bytes(self, client_socket):
        file_size = client_socket.recv(8)
        file_size = int.from_bytes(file_size, byteorder='little', signed=True)
        print("File size to reveice {} bytes".format(file_size))
        bytes_received = 0
        file_received = bytearray()
        while bytes_received < file_size:
            bytes_to_receive = min(file_size - bytes_received, 1024)
            file_received.extend(client_socket.recv(bytes_to_receive))
            bytes_received += bytes_to_receive
        print("File received, {} bytes".format(len(file_received)))
        return file_received
        
    def manage_request(self, client_socket):
        print("Responding...")

        img_file_bytes = self.__get_image_file_bytes(client_socket)

        # TODO response
        img = Image.open(io.BytesIO(img_file_bytes))
        print("Image Size: {}".format(img.size))
        self.dog_breed_classifier.classify(img)

        client_socket.shutdown(socket.SHUT_RDWR)
        client_socket.close()
        print("Client Connection Closed")
