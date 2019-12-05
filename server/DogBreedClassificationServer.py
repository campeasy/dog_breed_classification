from DogBreedClassifier import DogBreedClassifier
from AcceptingSocketThread import AcceptingSocketThread
from DogBreedClassificationServerProtocol import DogBreedClassificationServerProtocol

class DogBreedClassificationServer:

    def __init__(self, port, labels_path, weights_path):
        self.dog_breed_classifier = DogBreedClassifier(labels_path=labels_path, weights_path=weights_path)
        self.server_protocol = DogBreedClassificationServerProtocol(self.dog_breed_classifier)
        self.accepting_socket_thread = AcceptingSocketThread(port, self.server_protocol.manage_request)

    def start(self):
        self.accepting_socket_thread.start()

    def close(self):
        self.accepting_socket_thread.close()

    def join(self):
        self.accepting_socket_thread.join()
