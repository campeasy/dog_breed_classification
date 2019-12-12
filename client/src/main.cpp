#include <string>

#include "Client.h"

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "DogBreedClassifier - Usage: ");
        fprintf(stderr, "%s image_pathname", argv[0]);

        return -1;
    }

    std::string image_pathname(argv[1]);

    Client * c = new Client();
    c->run_client(image_pathname);
    delete c;
}
