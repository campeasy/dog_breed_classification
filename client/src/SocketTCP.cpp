/*
    C++ Wrapper for Socket
*/

#include "SocketTCP.h"

using namespace std;

// Private Methods for handling the Server Address:
int SocketTCP::set_server_ip(const char * temp_ip){
    if(temp_ip == NULL){
        fprintf(stderr, "[FAIL] Can't set the Server IP\n");
        return -1;
    }
    if(strcpy(server_ip, temp_ip) == NULL){
        fprintf(stderr, "[FAIL] Can't set the Server IP\n");
        return -1;
    }

    fprintf(stdout, "[OK] Server IP correctly setted - IP: %s\n", server_ip);
    return 0;
}

int SocketTCP::set_server_port(int temp_port){
    if(temp_port < 0 || temp_port > 65353){
        fprintf(stderr, "[FAIL] Can't use Port %d - Port not valid\n", temp_port);
        return -1;
    }
    if(temp_port >= 0 && temp_port <= 1023){
        fprintf(stderr, "[WARNING] Should use a different Port than %d - Port usually reserved\n", temp_port);
    }

    server_port = temp_port;
    fprintf(stdout, "[OK] Server Port correctly setted - Port: %d\n", server_port);
    return 0;
}

int SocketTCP::create_server_address(const char * temp_ip, int temp_port){
    if(set_server_ip(temp_ip) != 0) return -1;
    if(set_server_port(temp_port) != 0) return -1;

    // Server Address Construction:
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    server_address.sin_addr.s_addr = inet_addr(server_ip);

    fprintf(stdout, "[OK] Server Address correctly constructed - (%s , %d)\n", server_ip, server_port);
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
        fprintf(stderr, "[FAIL] Can't create the socket - Error creating\n");
        return -1;
    }

    fprintf(stdout, "[OK] Socket correctly created - Descriptor: %d\n", socket_descriptor);

    return 0;
}

int SocketTCP::connect_socket(){
    int result = 0;
    result = connect(socket_descriptor, (struct sockaddr *) &server_address, sizeof(server_address));
    if(result != 0){
        fprintf(stderr, "[FAIL] Can't establish connection - Error: %d \n", errno);
        return -1;
    }

    fprintf(stdout, "[OK] Connection correctly established\n");
    return 0;
}

int SocketTCP::close_socket(){
    if(close(socket_descriptor) != 0){
        fprintf(stderr, "[FAIL] Can't close socket - Descriptor: %d\n", socket_descriptor);
        return -1;
    }

    fprintf(stdout, "[OK] Socket correctly closed\n");

    return 0;
}

SocketTCP::SocketTCP(){
    socket_descriptor = -1;
    socket_is_alive = 0;

    strcpy(server_ip, "\0");
    server_port = -1;

    strcpy(buffer_to_send, "\0");
    strcpy(buffer_to_recv, "\0");
}

SocketTCP::~SocketTCP(){
    if(socket_is_alive == 1){
        close_socket();
        fprintf(stderr, "[WARNING] Destroyed a socket that was connected\n");
    }
}

// Public Methods for Getting/Setting Server Address Informations:
string SocketTCP::socket_get_server_ip(){
    string str(server_ip);
    return str;
}

int SocketTCP::socket_get_server_port(){
    return server_port;
}

int SocketTCP::socket_set_server(const char * _server_ip, int _server_port){
    if(socket_is_alive == 1){
        char temp_error[256];
        char temp_warning[256];

        strcpy(temp_error, "[FAIL] Can't set the server infos, socket already opened");
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
        fprintf(stderr, "[FAIL] Can't create the socket - Socket already alive\n");
        return -1;
    }

    if(check_server_address() != 0){
        fprintf(stderr, "[FAIL] Can't create the socket - Setted Server Address Not Valid\n");
        return -1;
    }

    if(create_socket() != 0 || connect_socket() != 0){ 
        fprintf(stderr, "[FAIL] Socket is NOT alive\n");
        return -1;
    }

    socket_is_alive = 1;
    fprintf(stdout, "[OK] Socket is alive\n");
    return 0;
}

int SocketTCP::socket_open(const char * _server_ip, int _server_port){
    if(socket_is_alive == 1){
        fprintf(stderr, "[FAIL] Can't create the socket - Socket already alive\n");
        return -1;
    }

    if(create_server_address(_server_ip, _server_port) != 0){
        return -1;
    }

    if(check_server_address() != 0){
        fprintf(stderr, "[FAIL] Server Address Not Valid");
        return -1;
    }

    if(create_socket() != 0 || connect_socket() != 0){ 
        fprintf(stderr, "[FAIL] Socket is NOT alive\n");
        return -1;
    }

    socket_is_alive = 1;
    fprintf(stdout, "[OK] Socket is alive\n");
    return 0;
}

int SocketTCP::socket_close(){
    if(socket_is_alive == 0){
        fprintf(stderr, "[FAIL] Can't close socket that is not alive\n");
        return -1;
    }

    if(close_socket() != 0) return -1;

    socket_is_alive = 0;
    return 0;
}

int SocketTCP::socket_send_data(const char * data, int data_dim){
    if(socket_is_alive == 0){
        fprintf(stderr, "[FAIL] Can't send data - Socket is not alive\n");
        return -1;
    }

    if(data_dim > BUFF_SEND_MAX_SIZE){
        fprintf(stderr, "[FAIL] Can't send data - Too much data\n");
        return -1;
    }

    strncpy(buffer_to_send, "", BUFF_SEND_MAX_SIZE);
    strncpy(buffer_to_send, data, BUFF_SEND_MAX_SIZE);

    if(send(socket_descriptor, buffer_to_send, BUFF_SEND_MAX_SIZE, 0) == -1){
        fprintf(stderr, "[FAIL] Can't send data - Error sending data\n");
        return -1;
    }

    fprintf(stdout, "[OK] Data sent correctly\n");
    return 0;
}