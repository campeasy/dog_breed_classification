/*
    December 2019
    DogBreed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Application Protocol
*/

#include "ClientAP.h"

long ClientAP::get_file_size(const std::string image_pathname){
    FILE * image_pointer = fopen(image_pathname.c_str(), "r");
    if(image_pointer == NULL){
        fprintf(stderr, "[FAIL - ClientAP] Can't get Image size - Bad pathname\n");
        return -1;
    }

    fseek(image_pointer, 0, SEEK_END);
    long image_size = ftell(image_pointer);
    if(image_size == -1){
        fprintf(stderr, "[FAIL - ClientAP] Can't get Image size - Bad format\n");
        return -1;
    }

    fprintf(stdout, "[OK - ClientAP] Image size got successfully\n");
    return image_size;
}

int ClientAP::request(const std::string image_pathname, const std::string server_ip, const int server_port){
    int err = 0;
    long image_size = 0;
    size_t sent_bytes = 0, read_bytes = 0;

    unsigned char request_buffer[MAXIMUM_SENDABLE_BYTES];

    // Opening the Image:
    FILE * image_pointer = fopen(image_pathname.c_str(), "r");
    if(image_pointer == NULL){
        fprintf(stderr, "[FAIL - ClientAP] Can't open Image - Bad pathname\n");
        return -1;
    }

    // Getting the Image Size:
    image_size = get_file_size(image_pathname);
    if(image_size == -1) return -1;

    // Setting the Server Address:
    err = socket->socket_set_server(server_ip, server_port);
    if(err == -1) return -1;

    // Connecting to the Server:
    err = socket->socket_open();
    if(err == -1) return -1;

    // Sending the Image Size:
    err = socket->socket_send_data(&image_size, sizeof(long));
    if(err == -1) return -1;

    // Sending the whole Image:
    while(sent_bytes < image_size){
        read_bytes = fread(request_buffer, 1, MAXIMUM_SENDABLE_BYTES, image_pointer);

        if(read_bytes == 0){
            fprintf(stderr, "[FAIL - ClientAP] Can't read Image\n");
            return -1;
        }

        err = socket->socket_send_data(request_buffer, read_bytes);
        if(err == -1) return -1;

        sent_bytes += read_bytes;
    }

    fprintf(stdout, "[OK - ClientAP] Request completed correctly\n");
    return 0;
}

int ClientAP::response(void * data_buffer, const int data_buffer_dim){
    int err = 0;

    if(data_buffer_dim > MAXIMUM_RECEIVABLE_BYTES){
        fprintf(stderr, "[FAIL - ClientAP] Exceeded Response Max Size");
        return -1;
    }

    err = socket->socket_recv_data(data_buffer, data_buffer_dim);
    if(err == -1) return -1;

    fprintf(stdout, "[Ok - ClientAp] Response completed correctly\n");
    return 0;
}

ClientAP::ClientAP(){
    socket = new SocketTCP();
    MAXIMUM_SENDABLE_BYTES = socket->get_max_receivable_data();
    MAXIMUM_RECEIVABLE_BYTES = socket->get_max_sendable_data();
}

ClientAP::~ClientAP(){
    delete(socket);
}

// Performing a Request and Getting a Response:
std::string ClientAP::perform(const std::string image_pathname, const std::string server_ip, const int server_port){
    char buffer[MAXIMUM_RECEIVABLE_BYTES];
    int err = 0;

    err = request(image_pathname, server_ip, server_port);
    if(err == -1){
        std::string str("Client Application Protocol Error - Bad Request");
        return str;
    }

    err = response(buffer, MAXIMUM_RECEIVABLE_BYTES);
    if(err == -1){
        std::string str("Client Application Protocol Error - Bad Response");
        return str;
    }

    std::string str(buffer);
    return str;
}
