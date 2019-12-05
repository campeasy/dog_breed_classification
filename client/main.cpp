#include <iostream>

#include "SocketTCP.cpp"

using namespace std;

int main(){
    // Short Testing Routine:
    SocketTCP * my = new SocketTCP();

    my->socket_set_server("127.0.0.1", 3456);
    my->socket_open();
    my->socket_send_data("", 0);
    my->socket_close();

    cout << endl
         << "Setted IP: " << my->socket_get_server_ip()
         << endl
         << "Setted Port: " << my->socket_get_server_port()
         << endl;
    return 0;
}