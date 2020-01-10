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
import time
import pandas as pd
import signal
import socket
import functools
from datetime import datetime

from PIL import Image
from DogBreedClassifier import DogBreedClassifier

class ServerApplicationProtocol():

    def __init__(self, dog_breed_classifier, stats_path="stats.csv"):
        self.__dog_breed_classifier = dog_breed_classifier
        self.__stats_path = stats_path
        if os.path.isfile(self.__stats_path):
            self.dataframe = pd.read_csv(self.__stats_path)
        else:
            self.dataframe = pd.DataFrame(columns=['Timestamp', 'Class', 'PercentageConfidence'])

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

        # current date and time
        now = datetime.now()
        timestamp = datetime.timestamp(now)

        # Receiving the Image:
        img_file_bytes = self.__get_image_file_bytes(client_socket)

        # Opening the received Image:
        img = Image.open(io.BytesIO(img_file_bytes))

        # Classyfing the Image:
        result = self.__dog_breed_classifier.classify(img)
        
        first_line = result.split('\n', 1)[0]
        splitted_first_line = first_line.split('%', 1)
        percentage = splitted_first_line[0]
        class_name = splitted_first_line[1].strip()

        print(timestamp, class_name, percentage)
        self.dataframe = self.dataframe.append(pd.Series(data={'Timestamp': timestamp, 'Class': class_name, 'PercentageConfidence': percentage}), ignore_index=True)
        print(self.dataframe)
        self.dataframe.to_csv(self.__stats_path, index=False)

        result = result.encode('utf-8')
        # Sending result dimension and the result itself to the Client
        client_socket.send(len(result).to_bytes(8, byteorder='little', signed=False))
        client_socket.send(result)
        print("[OK - ServerApplicationProtocol] Result correctly sent to the Client")

        client_socket.shutdown(socket.SHUT_RDWR)
        client_socket.close()
        print("[OK - ServerApplicationProtocol] Socket with the Client closed")
