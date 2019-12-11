#include <iostream>
#include <string>


#include "ClientProxy.h"
#include "ClientApplicationProtocol.h"

using namespace std;

void test(){
    // ClientApplicationProtocol * my = new ClientApplicationProtocol();
    // std::string result;

    // result = my->perform("../test_images/dog5.jpg", "127.0.0.1", 10000);

    // cout << endl << result << endl;
    // return;

    ClientProxy * proxy = new ClientProxy();

    std::string str("127.0.0.1");
    proxy->add_server_address(str, 9999);
    proxy->add_server_address(str, 6555);

    proxy->classify_image("../test_images/dog5.jpg");

    return;
}

int main(){
    test();

    return 0;
}
