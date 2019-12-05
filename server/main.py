import signal
import argparse
import functools

from DogBreedClassificationServer import DogBreedClassificationServer

def main():
    parser = argparse.ArgumentParser(description='DogBreedClassificationServerMain')
    parser.add_argument('-w', '--weights', default="parameters/dog_classification_parameters.pth", type=str, help='weights path to load')
    parser.add_argument('-l', '--labels', default="labels.txt", type=str, help='human redable labels path')
    parser.add_argument('-p', '--port', default=9999, type=int, help='server connection port')
    args = parser.parse_args()
    server = DogBreedClassificationServer(port=args.port, labels_path=args.labels, weights_path=args.weights)
    server.start()
    signal.signal(signal.SIGINT, lambda x, y: server.close())
    server.join()

if __name__ == "__main__":
    main()
