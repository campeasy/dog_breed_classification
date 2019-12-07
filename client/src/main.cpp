#include <iostream>
#include <string>

#include "ClientAP.h"
#include "SocketTCP.h"

using namespace std;


#define BUFFER_SIZE 1024

void test(){
    ClientAP * my = new ClientAP();
    std::string result;

    result = my->perform("../server/test_images/dog5.jpg", "127.0.0.1", 9999);

    cout << endl << result << endl;
    return;
}

int main(){
    test();

    return 0;
}
