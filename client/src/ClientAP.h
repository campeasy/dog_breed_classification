#pragma once

#include <iostream>
#include <string>

#include "SocketTCP.h"

class ClientAP{
    private:
        SocketTCP * socket;

        long get_file_size(const std::string image_pathname);

    public:
        ClientAP();
        ~ClientAP();

        int request(const std::string image_pathname, const std::string server_ip, const int server_port);
};
