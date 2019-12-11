/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Proxy Factory
*/

#include "ClientProxyFactory.h"

ClientProxyInterface * ClientProxyFactory::create_ClientProxyInterface(){
    ClientProxy * proxy = new ClientProxy();

    // Setting the Servers Informations:
    std::string main_server_ip("127.0.0.1");
    int main_server_port = 9999;

    std::string backup_server_ip("127.0.0.1");
    int backup_server_port = 8888;

    // Adding the Servers addresses:
    proxy->add_server_address(backup_server_ip, backup_server_port);
    proxy->add_server_address(main_server_ip, main_server_port);

    fprintf(stdout, "[OK - ClientProxyFactory] Client Proxy Object correctly created\n");
    return (ClientProxyInterface *) proxy;
}
