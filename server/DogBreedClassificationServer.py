from DogBreedClassifier import DogBreedClassifier
from AcceptingSocketThread import AcceptingSocketThread
from DogBreedClassificationServerProtocol import DogBreedClassificationServerProtocol
from concurrent.futures import ThreadPoolExecutor

class DogBreedClassificationServer:

    def __init__(self, port, labels_path, weights_path):
        self.thread_pool = ThreadPoolExecutor(200)
        self.dog_breed_classifier = DogBreedClassifier(labels_path=labels_path, weights_path=weights_path)
        self.server_protocol = DogBreedClassificationServerProtocol(self.dog_breed_classifier)
        self.accepting_socket_thread = AcceptingSocketThread(port, self.manage_request)

    def manage_request(self, client_connection):
        self.thread_pool.submit(self.server_protocol.manage_request, (client_connection))

    def start(self):
        self.accepting_socket_thread.start()

    def close(self):
        self.thread_pool.shutdown(wait=True)
        self.accepting_socket_thread.close()

    def join(self):
        self.accepting_socket_thread.join()
