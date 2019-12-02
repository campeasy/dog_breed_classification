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
        char server_ip[IP_MAX_SIZE];
        int server_port;
        struct sockaddr_in server_address;

        int socket_descriptor;

        char buffer_to_send[BUFF_MAX_SIZE];

        int set_server_ip(char * temp_ip){
            if(temp_ip == NULL){
                fprintf(stderr, "[FAIL] Can't set the Server IP\n");
                return -1;
            }
            if(strcpy(server_ip, temp_ip) == NULL){
                fprintf(stderr, "[FAIL] Can't set the Server IP\n");
                return -1;
            }

            fprintf(stdout, "[OK] Server IP correctly setted\n");
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
            fprintf(stdout, "[OK] Server Port correctly setted\n");
            return 0;
        }

        int create_server_address(char * temp_ip, int temp_port){
            if(set_server_ip(temp_ip) != 0) return -1;
            if(set_server_port(temp_port) != 0) return -1;

            // Server Address Construction:
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons(server_port);
            server_address.sin_addr.s_addr = inet_addr(server_ip);

            fprintf(stdout, "[OK] Server Address correctly constructed\n");
            return 0;
        }

        int create(){
            socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
            if(socket_descriptor == -1){
                fprintf(stderr, "[FAIL] Can't create socket\n");
                return -1;
            }
            else return 0;
        }

        // int connect(){

        // }

    public:
        SocketTCP(){
            fprintf(stdout, "prova: %s\n", server_ip);
            server_port = 0;
            strcpy(buffer_to_send, "\0");
        }

        string get_server_ip(){
            if(strcmp(server_ip, "\0") == 0) return NULL;
            string str(server_ip);
            return str;
        }

        int get_server_port(){
            return server_port;
        }

        void test(char * temp_ip, int temp_port){
            create_server_address(temp_ip, temp_port);
        }


};