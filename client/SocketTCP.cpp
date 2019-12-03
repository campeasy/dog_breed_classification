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
#define BUFF_MAX_SIZE 1024

class SocketTCP{
    private:
        // PRIVATE ATTRIBUTES:
        char server_ip[IP_MAX_SIZE];
        int server_port;
        struct sockaddr_in server_address;

        int socket_descriptor;

        char buffer_to_send[BUFF_MAX_SIZE];

        // PRIVATE METHODS:
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

        int open_socket(){
            if(socket_descriptor != -1){
                fprintf(stdout, "[WARNING] Closing the previous socket - Descriptor: %d\n", socket_descriptor);
                if(close(socket_descriptor) != 0){
                    fprintf(stderr, "[FAIL] Can't close the previous socket - Descriptor: %d\n", socket_descriptor);
                    return -1;
                }

                socket_descriptor = -1;
                fprintf(stdout, "[OK] Previous socket correctly closed\n");
            }

            socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
            if(socket_descriptor == -1){
                fprintf(stderr, "[FAIL] Can't open the socket\n");
                return -1;
            }

            fprintf(stdout, "[OK] Socket correctly opened - Descriptor: %d\n", socket_descriptor);
            return 0;
        }

        int create_socket(char * temp_ip, int temp_port){
            if(create_server_address(temp_ip, temp_port) != 0) return -1;
            if(open_socket() != 0) return -1;

            return 0;
        }

        int close_socket(){
            if(close(socket_descriptor) != 0){
                fprintf(stderr, "[FAIL] Can't close socket - %d\n", socket_descriptor);
                return -1;
            }
            fprintf(stdout, "[OK] Socket correctly closed\n");

            strcpy(server_ip, "\0");
            server_port = -1;
            strcpy(buffer_to_send, "\0");
            socket_descriptor = -1;

            return 0;
        }

    public:
        SocketTCP(char * temp_ip, int temp_port){
            strcpy(server_ip, "\0");
            server_port = -1;
            strcpy(buffer_to_send, "\0");
            socket_descriptor = -1;

            // Socket Creation:
            if(create_socket(temp_ip, temp_port) != 0){
                fprintf(stderr, "[FAIL] Program failed at runtime - error on socket creation\n");
                exit(1);
            }
        }
        ~SocketTCP(){
            // Socket Deletion:
            if(close_socket() != 0){
                fprintf(stderr, "[WARNING] Socket closed but not correctly\n");
            }
        }

        string get_server_ip(){
            if(strcmp(server_ip, "\0") == 0) return NULL;
            string str(server_ip);
            return str;
        }

        int get_server_port(){
            return server_port;
        }
};