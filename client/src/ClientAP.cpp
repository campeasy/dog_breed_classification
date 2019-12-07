#include "ClientAP.h"

#define BUFFER_REQUEST_MAX_SIZE 1024
#define BUFFER_RESPONSE_MAX_SIZE 1024

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

ClientAP::ClientAP(){
    socket = new SocketTCP();
}

ClientAP::~ClientAP(){
    delete(socket);
}

int ClientAP::request(const std::string image_pathname, const std::string server_ip, const int server_port){
    int err = 0;
    long image_size = 0;
    size_t sent_bytes, read_bytes = 0;

    unsigned char request_buffer[BUFFER_REQUEST_MAX_SIZE];

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
        read_bytes = fread(request_buffer, 1, BUFFER_REQUEST_MAX_SIZE, image_pointer);

        if(read_bytes == 0){
            fprintf(stderr, "[FAIL - ClientAP] Can't read Image\n");
            return -1;
        }

        err = socket->socket_send_data(request_buffer, read_bytes);
        if(err == -1) return -1;

        sent_bytes += read_bytes;
    }

    fprintf(stdout, "[OK - ClientAP] Image Sent Correctly\n");
    return 0;
}
