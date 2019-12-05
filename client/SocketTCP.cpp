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

using namespace std;

#define IP_MAX_SIZE 1024
#define BUFF_SEND_MAX_SIZE 1024
#define BUFF_RECV_MAX_SIZE 1024

class SocketTCP{
    private:
        // ----- Private Socket Attributes -----
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
        // -------------------------------------

        // ----- Private Methods for constructing the Server Address -----
        int set_server_ip(char * temp_ip){
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

        int set_server_port(int temp_port){
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

        int create_server_address(char * temp_ip, int temp_port){
            if(set_server_ip(temp_ip) != 0) return -1;
            if(set_server_port(temp_port) != 0) return -1;

            // Server Address Construction:
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons(server_port);
            server_address.sin_addr.s_addr = inet_addr(server_ip);

            fprintf(stdout, "[OK] Server Address correctly constructed - (%s , %d)\n", server_ip, server_port);
            return 0;
        }
        // ---------------------------------------------------------------

        int create_socket(){
            if(socket_is_alive == 1){
                fprintf(stderr, "[FAIL] Can't create socket - Already alive\n");
                return -1;
            }

            socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
            if(socket_descriptor == -1){
                fprintf(stderr, "[FAIL] Can't create the socket - Error creating\n");
                return -1;
            }

            socket_is_alive = 1;
            fprintf(stdout, "[OK] Socket correctly created - Descriptor: %d\n", socket_descriptor);

            return 0;
        }

        // TODO: connect_socket()

        int close_socket(){
            if(socket_is_alive == 0){
                fprintf(stderr, "[FAIL] Can't close socket that is not alive\n");
                return -1;
            }

            if(close(socket_descriptor) != 0){
                fprintf(stderr, "[FAIL] Can't close socket - Descriptor: %d\n", socket_descriptor);
                return -1;
            }
            
            socket_is_alive = 0;
            fprintf(stdout, "[OK] Socket correctly closed\n");

            return 0;
        }

    public:
        SocketTCP(){
            socket_descriptor = -1;
            socket_is_alive = 0;

            strcpy(server_ip, "\0");
            server_port = -1;

            strcpy(buffer_to_send, "\0");
            strcpy(buffer_to_recv, "\0");
        }

        ~SocketTCP(){
            if(socket_is_alive == 1){
                close_socket();
                fprintf(stderr, "[WARNING] Destroyed a socket that was connected\n");
            }
        }

        // ----- Public Methods for Getting/Setting Server Address Informations -----
        string socket_get_server_ip(){
            string str(server_ip);
            return str;
        }

        int socket_get_server_port(){
            return server_port;
        }

        int socket_set_server(char * _server_ip, int _server_port){
            if(socket_is_alive == 1){
                char * temp_error = "[FAIL] Can't set the server infos, socket already opened";
                char * temp_warning = "If want set another server, first close the socket";
                fprintf(stderr, "%s - IP: %s Port: %d\n%s\n", temp_error, server_ip, server_port, temp_warning);
                return -1;
            }

            // Setting the server infos:
            if(create_server_address(_server_ip, _server_port) != 0) return -1;
            return 0;
        }
        // --------------------------------------------------------------------------


        // TODO: socket_open(), socket_send_data(data_dim), socket_recv_data(data_dim), socket_close()
};