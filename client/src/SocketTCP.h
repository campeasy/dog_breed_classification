/*
    C++ Wrapper for Socket
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
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

        // Data Buffers:
        char buffer_to_send[BUFF_SEND_MAX_SIZE];
        char buffer_to_recv[BUFF_RECV_MAX_SIZE];

        // Private Methods for handling the Server Address:
        int set_server_ip(const std::string &temp_ip);

        int set_server_port(int temp_port);

        int create_server_address(const std::string &temp_ip, int temp_port);

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

        int socket_set_server(const std::string &_server_ip, int _server_port);

        // Exposed Public Methods for Operations: 
        int socket_open();

        int socket_open(const std::string &_server_ip, int _server_port);

        int socket_close();

        int socket_send_data(const void * data, size_t data_dim);

        // TODO: socket_recv_data()
};
