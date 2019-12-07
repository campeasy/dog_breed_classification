#include <iostream>
#include <string>

#include "ClientAP.h"

using namespace std;

void test(){
    ClientAP * my = new ClientAP();
    std::string result;

    result = my->perform("../test_images/dog5.jpg", "127.0.0.1", 9999);

    cout << endl << result << endl;
    return;
}

int main(){
    test();

    return 0;
}
