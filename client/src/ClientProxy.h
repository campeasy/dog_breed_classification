/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Proxy
*/

#pragma once

#include <vector>

#include "ClientProxyInterface.h"
#include "ClientApplicationProtocol.h"

class ClientProxy : public ClientProxyInterface{
    private:
        // Communication Protocol:
        ClientApplicationProtocol * client_application_protocol;

        // Pool of Servers:
        std::vector<std::string> ip_adresses;
        std::vector<int> ports;

        // Methods for managing the Pool of Servers:
        int add_server_address(const std::string ip, const int port);
        int remove_server_address(const std::string ip, const int port);

    public:
        ClientProxy();
        ~ClientProxy();

        // Implementation of the virtual method:
        std::string classify_image(const std::string image_pathname);

};