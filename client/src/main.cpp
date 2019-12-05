#include <iostream>
#include <string>

#include "SocketTCP.h"

using namespace std;

int main(){
    // Short Testing Routine:
    SocketTCP * my = new SocketTCP();
    string ip = "127.0.0.1";
    my->socket_set_server(ip.c_str(), 3456);
    my->socket_open();
    string data = "";
    my->socket_send_data(data.c_str(), 0);
    my->socket_close();

    cout << endl
         << "Setted IP: " << my->socket_get_server_ip()
         << endl
         << "Setted Port: " << my->socket_get_server_port()
         << endl;
    return 0;
}
