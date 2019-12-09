#include <iostream>
#include <string>

#include "ClientApplicationProtocol.h"

using namespace std;

void test(){
    ClientApplicationProtocol * my = new ClientApplicationProtocol();
    std::string result;

    result = my->perform("../test_images/dog5.jpg", "127.0.0.1", 10000);

    cout << endl << result << endl;
    return;
}

int main(){
    test();

    return 0;
}
