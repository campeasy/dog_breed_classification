/*
    December 2019
    DogBreed Classification
    Advanced Programming Languages

    C++ Wrapper for Socket in C

    Written by Salvatore Campisi e Daniele Calanna
*/

#include "SocketTCP.h"

// Private Methods for handling the Server Address:
int SocketTCP::set_server_ip(const std::string &temp_ip){
    if(strcpy(server_ip, temp_ip.c_str()) == NULL){
        fprintf(stderr, "[FAIL - SocketTCP] Can't set the Server IP\n");
        return -1;
    }

    fprintf(stdout, "[OK - SocketTCP] Server IP correctly setted - IP: %s\n", server_ip);
    return 0;
}

int SocketTCP::set_server_port(const int temp_port){
    if(temp_port < 0 || temp_port > 65353){
        fprintf(stderr, "[FAIL - SocketTCP] Can't use Port %d - Port not valid\n", temp_port);
        return -1;
    }
    if(temp_port >= 0 && temp_port <= 1023){
        fprintf(stderr, "[WARNING - SocketTCP] Should use a different Port than %d - Port usually reserved\n", temp_port);
    }

    server_port = temp_port;
    fprintf(stdout, "[OK - SocketTCP] Server Port correctly setted - Port: %d\n", server_port);
    return 0;
}

int SocketTCP::create_server_address(const std::string &temp_ip, const int temp_port){
    if(set_server_ip(temp_ip) != 0) return -1;
    if(set_server_port(temp_port) != 0) return -1;

    // Server Address Construction:
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = inet_addr(server_ip);

    fprintf(stdout, "[OK - SocketTCP] Server Address correctly constructed - (%s , %d)\n", server_ip, server_port);
    return 0;
}

int SocketTCP::check_server_address(){
    if(strcmp(server_ip,"\0") == 0 || server_port == -1){
        return -1;
    }

    return 0;
}

// Private Methods for handling Socket Operations:
int SocketTCP::create_socket(){
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_descriptor == -1){
        fprintf(stderr, "[FAIL - SocketTCP] Can't create the socket - Error creating\n");
        return -1;
    }

    fprintf(stdout, "[OK - SocketTCP] Socket correctly created - Descriptor: %d\n", socket_descriptor);

    return 0;
}

int SocketTCP::connect_socket(){
    int result = 0;
    result = connect(socket_descriptor, (struct sockaddr *) &server_address, sizeof(server_address));
    if(result != 0){
        fprintf(stderr, "[FAIL - SocketTCP] Can't establish connection - Error: %d \n", errno);
        return -1;
    }

    fprintf(stdout, "[OK - SocketTCP] Connection correctly established\n");
    return 0;
}

int SocketTCP::close_socket(){
    if(close(socket_descriptor) != 0){
        fprintf(stderr, "[FAIL - SocketTCP] Can't close socket - Descriptor: %d\n", socket_descriptor);
        return -1;
    }

    fprintf(stdout, "[OK - SocketTCP] Socket correctly closed\n");

    return 0;
}

SocketTCP::SocketTCP(){
    socket_descriptor = -1;
    socket_is_alive = 0;

    strcpy(server_ip, "\0");
    server_port = -1;
}

SocketTCP::~SocketTCP(){
    if(socket_is_alive == 1){
        close_socket();
        fprintf(stderr, "[WARNING - SocketTCP] Destroyed a socket that was connected\n");
    }
}

// Public Methods for Getting/Setting Server Address Informations:
std::string SocketTCP::socket_get_server_ip(){
    std::string str(server_ip);
    return str;
}

int SocketTCP::socket_get_server_port(){
    return server_port;
}

int SocketTCP::socket_set_server(const std::string &_server_ip, const int _server_port){
    if(socket_is_alive == 1){
        char temp_error[256];
        char temp_warning[256];

        strcpy(temp_error, "[FAIL - SocketTCP] Can't set the server infos, socket already opened");
        strcpy(temp_warning, "If want set another server, first close the socket");

        fprintf(stderr, "%s - IP: %s Port: %d\n%s\n", temp_error, server_ip, server_port, temp_warning);
        return -1;
    }

    // Setting the server infos:
    if(create_server_address(_server_ip, _server_port) != 0) return -1;
    return 0;
}

// Exposed Public Methods for Operations: 
int SocketTCP::socket_open(){
    if(socket_is_alive == 1){
        fprintf(stderr, "[FAIL - SocketTCP] Can't create the socket - Socket already alive\n");
        return -1;
    }

    if(check_server_address() != 0){
        fprintf(stderr, "[FAIL - SocketTCP] Can't create the socket - Setted Server Address Not Valid\n");
        return -1;
    }

    if(create_socket() != 0 || connect_socket() != 0){ 
        fprintf(stderr, "[FAIL - SocketTCP] Socket is NOT alive\n");
        return -1;
    }

    socket_is_alive = 1;
    fprintf(stdout, "[OK - SocketTCP] Socket is alive\n");
    return 0;
}

int SocketTCP::socket_open(const std::string &_server_ip, const int _server_port){
    if(socket_is_alive == 1){
        fprintf(stderr, "[FAIL - SocketTCP] Can't create the socket - Socket already alive\n");
        return -1;
    }

    if(create_server_address(_server_ip, _server_port) != 0){
        return -1;
    }

    if(check_server_address() != 0){
        fprintf(stderr, "[FAIL - SocketTCP] Server Address Not Valid");
        return -1;
    }

    if(create_socket() != 0 || connect_socket() != 0){ 
        fprintf(stderr, "[FAIL - SocketTCP] Socket is NOT alive\n");
        return -1;
    }

    socket_is_alive = 1;
    fprintf(stdout, "[OK - SocketTCP] Socket is alive\n");
    return 0;
}

int SocketTCP::socket_close(){
    if(socket_is_alive == 0){
        fprintf(stderr, "[FAIL - SocketTCP] Can't close socket that is not alive\n");
        return -1;
    }

    if(close_socket() != 0) return -1;

    socket_is_alive = 0;
    return 0;
}

int SocketTCP::socket_send_data(const void * data, const size_t data_dim){
    if(socket_is_alive == 0){
        fprintf(stderr, "[FAIL - SocketTCP] Can't send data - Socket is not alive\n");
        return -1;
    }

    if(data_dim > BUFF_SEND_MAX_SIZE){
        fprintf(stderr, "[FAIL - SocketTCP] Can't send data - Too much data to send\n");
        return -1;
    }

    if(send(socket_descriptor, data, data_dim, 0) == -1){
        fprintf(stderr, "[FAIL - SocketTCP] Can't send data - Error sending data\n");
        return -1;
    }

    fprintf(stdout, "[OK - SocketTCP] Data sent correctly\n");
    return 0;
}

int SocketTCP::socket_recv_data(void * buffer, const size_t buffer_dim ){
    if(socket_is_alive == 0){
        fprintf(stderr, "[FAIL - SocketTCP] Can't receive data - Socket is not alive\n");
        return -1;
    }

    if(buffer_dim > BUFF_RECV_MAX_SIZE){
        fprintf(stderr, "[FAIL - SocketTCP] Can't receive data - Too much data to receive\n");
        return -1;
    }

    if(recv(socket_descriptor, buffer, buffer_dim, 0) == -1){
        fprintf(stderr, "[FAIL - SocketTCP] Can't receive data - Error receiving data\n");
        return -1;
    }

    fprintf(stdout, "[OK - SocketTCP] Data received correctly\n");
    return 0;
}
