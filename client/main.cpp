#include <iostream>

#include "SocketTCP.cpp"

using namespace std;

int main(){
    // Short Testing Routine:
    SocketTCP * my = new SocketTCP();

    my->socket_open("127.0.0.1", 3500);
    //my->socket_set_server();
    my->socket_get_server_ip();
    my->socket_get_server_port();
    my->socket_send_data("", 0);
    my->socket_close();

    return 0;
}