#include <iostream>

#include "DesktopClient.cpp"

using namespace std;

int main(){
    SocketTCP * my = new SocketTCP();
    
    my->test("127.0.0.1", 3666);
    cout << "IP: " << my->get_server_ip() << " Port: " << my->get_server_port() << endl;

    return 0;
}