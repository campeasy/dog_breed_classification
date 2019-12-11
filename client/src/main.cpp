#include <iostream>
#include <string>


#include "ClientProxyFactory.h"

using namespace std;

void test(){
    std::string result;

    ClientProxyInterface * proxy = ClientProxyFactory::create_ClientProxyInterface();
    result = proxy->classify_image("../test_images/dog5.jpg");

    fprintf(stdout, "\n%s", result.c_str());
    return;
}

int main(){
    test();

    return 0;
}
