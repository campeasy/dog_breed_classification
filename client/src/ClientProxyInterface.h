/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Proxy Interface
*/

#include <string>

class ClientProxyInterface{
    public:
        virtual std::string classify_image() = 0;
};
