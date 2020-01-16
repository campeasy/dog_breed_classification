/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Proxy
*/

#pragma once

#include <string>
#include <vector>

#include "ClientProxyInterface.h"
#include "ClientApplicationProtocol.h"

class ClientProxy : public ClientProxyInterface{
    private:
        // Communication Protocol:
        ClientApplicationProtocol * client_application_protocol;

        // Pool of Servers:
        std::vector<std::string> ip_addrexes;
        std::vector<int> ports;

    public:
        ClientProxy();
        ~ClientProxy();

        // Methods for managing the Pool of Servers:
        void print_servers();
        void add_server_address(const std::string ip, const int port);
        void remove_server_address(const std::string ip, const int port);

        // Implementation of the virtual method:
        std::string classify_image(const std::string image_pathname) override;
        void feedback(const bool satisfaction) override;

};
