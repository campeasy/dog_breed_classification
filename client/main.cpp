#include <iostream>

#include "SocketTCP.cpp"

using namespace std;

int main(){
    // Short Testing Routine:
    SocketTCP * my = new SocketTCP("127.0.0.1", 1300);

    return 0;
}