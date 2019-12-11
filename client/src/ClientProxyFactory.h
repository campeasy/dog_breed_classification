/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Proxy Factory
*/

#pragma once

#include <string>

#include "ClientProxy.h"

class ClientProxyFactory{
    public:
        static ClientProxyInterface * create_ClientProxyInterface();
};
