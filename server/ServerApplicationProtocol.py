#
#    December 2019
#    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
#    Advanced Programming Languages
#
#    Server Application Protocol
#

import os
import io
import sys
import signal
import socket
import functools

from PIL import Image
from DogBreedClassifier import DogBreedClassifier

class ServerApplicationProtocol():

    def __init__(self, dog_breed_classifier):
        self.__dog_breed_classifier = dog_breed_classifier

    def __get_image_file_bytes(self, client_socket):
        file_size = client_socket.recv(8)
        file_size = int.from_bytes(file_size, byteorder='little', signed=True)

        print("[INFO - ServerApplicationProtocol] File size to receive {} bytes".format(file_size))
        bytes_received = 0
        file_received = bytearray()

        while bytes_received < file_size:
            bytes_to_receive = min(file_size - bytes_received, 1024)
            file_received.extend(client_socket.recv(bytes_to_receive))
            bytes_received += bytes_to_receive

        print("[OK - ServerApplicationProtocol] File received, {} bytes".format(len(file_received)))
        return file_received

    def manage_request(self, client_socket):
        print("[INFO - ServerApplicationProtocol] Start managing the Client request")

        # Receiving the Image:
        img_file_bytes = self.__get_image_file_bytes(client_socket)

        # Opening the received Image:
        img = Image.open(io.BytesIO(img_file_bytes))

        # Classyfing the Image:
        result = self.__dog_breed_classifier.classify(img)
        result = result.encode('utf-8')
        
        # Sending result dimension and the result itself to the Client
        client_socket.send(len(result).to_bytes(8, byteorder='little', signed=False))
        client_socket.send(result)
        print("[OK - ServerApplicationProtocol] Result correctly sent to the Client")

        client_socket.shutdown(socket.SHUT_RDWR)
        client_socket.close()
        print("[OK - ServerApplicationProtocol] Socket with the Client closed")