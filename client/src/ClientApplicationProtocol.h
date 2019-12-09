/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Application Protocol
*/

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "SocketTCP.h"

class ClientApplicationProtocol{
    private:
        SocketTCP * socket;

        std::vector<char> load_file(std::string filename);
        int request(const std::string image_pathname, const std::string server_ip, const int server_port);
        int response(std::string &result);

    public:
        ClientApplicationProtocol();
        ~ClientApplicationProtocol();

        // Performing a Request and returning a Response string:
        std::string perform(const std::string image_pathname, const std::string server_ip, const int server_port);
};
