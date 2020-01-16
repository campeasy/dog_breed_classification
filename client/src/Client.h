/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client
*/

#pragma once

#include "ClientProxyFactory.h"

class Client{
    private:
        // Classificator Object:
        ClientProxyInterface * proxy;
        bool get_feedback_from_user();

    public:
        Client();
        ~Client();

        void run_client(const std::string image_pathname);
};
