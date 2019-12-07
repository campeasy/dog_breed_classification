/*
    December 2019
    DogBreed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Application Protocol
*/

#pragma once

#include <string.h>

#include <iostream>
#include <string>

#include "SocketTCP.h"

class ClientAP{
    private:
        SocketTCP * socket;
        int MAXIMUM_SENDABLE_BYTES;
        int MAXIMUM_RECEIVABLE_BYTES;

        long get_file_size(const std::string image_pathname);
        int request(const std::string image_pathname, const std::string server_ip, const int server_port);
        int response(void * data_buffer, const int data_buffer_dim);

    public:
        ClientAP();
        ~ClientAP();

        // Performing a Request and returning a Response string:
        std::string perform(const std::string image_pathname, const std::string server_ip, const int server_port);
};
