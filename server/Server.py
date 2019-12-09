#
#    December 2019
#    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
#    Advanced Programming Languages
#
#    Server
#

from DogBreedClassifier import DogBreedClassifier
from ClientConnectionHandler import ClientConnectionHandler
from ServerApplicationProtocol import ServerApplicationProtocol
from concurrent.futures import ThreadPoolExecutor

class Server:

    def __init__(self, port, labels_path, weights_path):
        self.__thread_pool = ThreadPoolExecutor(200)
        self.__dog_breed_classifier = DogBreedClassifier(labels_path=labels_path, weights_path=weights_path)
        self.__server_protocol = ServerApplicationProtocol(self.__dog_breed_classifier)
        self.__client_connection_handler = ClientConnectionHandler(port, self.__server_protocol.manage_request)

    def manage_request(client_connection):
        self.__thread_pool.submit(self.server_protocol.manage_request, (client_connection))

    def start(self):
        self.__client_connection_handler.start()

    def close(self):
        self.__client_connection_handler.close()
        self.__thread_pool.shutdown(wait=True)

    def join(self):
        self.__client_connection_handler.join()
