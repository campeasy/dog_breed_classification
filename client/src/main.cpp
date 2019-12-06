#include <iostream>
#include <string>

#include "SocketTCP.h"

using namespace std;

typedef unsigned char byte;

#define BUFFER_SIZE 1024

int main(){
    // Short Testing Routine:
    SocketTCP * my = new SocketTCP();
    const string ip = "127.0.0.1";
    my->socket_set_server(ip, 9999);
    my->socket_open();
    
    FILE* file_ptr = fopen("../server/test_images/dog5.jpg", "r");
    fseek(file_ptr, 0, SEEK_END);
    long file_size = ftell(file_ptr);
    rewind(file_ptr);

    std::cout<<file_size<<std::endl;

    my->socket_send_data(&file_size, sizeof(long));

    size_t bytes_sent = 0;
    byte buffer[BUFFER_SIZE];
    while(bytes_sent < file_size)
    {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file_ptr);
        my->socket_send_data(buffer, bytes_read);
        bytes_sent += bytes_read;
    }
    std::cout<<"File sent"<<std::endl;

    char result[BUFFER_SIZE];
    size_t bytes_to_receive = BUFFER_SIZE;
    my->socket_recv_data(result, BUFFER_SIZE);
    cout << string(result) << endl;

    fclose(file_ptr);
    my->socket_close();

    cout << endl
         << "Setted IP: " << my->socket_get_server_ip()
         << endl
         << "Setted Port: " << my->socket_get_server_port()
         << endl;
    return 0;
}
