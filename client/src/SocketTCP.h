/*
    December 2019
    DogBreed Classification
    Advanced Programming Languages

    C++ Wrapper for Socket in C

    Written by Salvatore Campisi e Daniele Calanna
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>

#define IP_MAX_SIZE 1024
#define BUFF_SEND_MAX_SIZE 1024
#define BUFF_RECV_MAX_SIZE 1024

class SocketTCP{
    private:
        // Socket Informations:
        int socket_descriptor;
        int socket_is_alive;

        // Server Informations:
        char server_ip[IP_MAX_SIZE];
        int server_port;
        struct sockaddr_in server_address;

        // Private Methods for handling the Server Address:
        int set_server_ip(const std::string &temp_ip);
        int set_server_port(const int temp_port);
        int create_server_address(const std::string &temp_ip, const int temp_port);
        int check_server_address();

        // Private Methods for handling Socket Operations:
        int create_socket();
        int connect_socket();
        int close_socket();

    public:
        SocketTCP();
        ~SocketTCP();

        // Public Methods for Getting/Setting Server Address Informations:
        std::string socket_get_server_ip();
        int socket_get_server_port();
        int socket_set_server(const std::string &_server_ip, const int _server_port);

        // Exposed Public Methods for Operations: 
        int socket_open();
        int socket_open(const std::string &_server_ip, const int _server_port);
        int socket_close();
        int socket_send_data(const void * data, const size_t data_dim);
        int socket_recv_data(void * buffer, const size_t buffer_dim);
};
