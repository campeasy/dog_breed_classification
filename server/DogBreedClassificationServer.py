from DogBreedClassifier import DogBreedClassifier
from ClientConnectionHandler import ClientConnectionHandler
from DogBreedClassificationServerProtocol import DogBreedClassificationServerProtocol

class DogBreedClassificationServer:

    def __init__(self, port, labels_path, weights_path):
        self.dog_breed_classifier = DogBreedClassifier(labels_path=labels_path, weights_path=weights_path)
        self.server_protocol = DogBreedClassificationServerProtocol(self.dog_breed_classifier)
        self.client_connection_handler = ClientConnectionHandler(port, self.server_protocol.manage_request)

    def start(self):
        self.client_connection_handler.start()

    def close(self):
        self.client_connection_handler.close()

    def join(self):
        self.client_connection_handler.join()
