/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Proxy
*/

#include "ClientProxy.h"

void ClientProxy::add_server_address(const std::string ip, const int port){
    ip_addrexes.insert(ip_addrexes.begin(), ip);
    ports.insert(ports.begin(), port);

    return;
}

void ClientProxy::remove_server_address(const std::string ip, const int port){
    for(int i = 0; i < ip_addrexes.size(); ++i){

        if(ip_addrexes[i].compare(ip) == 0 && i < ports.size() && ports[i] == port){
            ip_addrexes.erase(ip_addrexes.begin() + i);
            ports.erase(ports.begin() + i);
            break;
        } 

    }

}

ClientProxy::ClientProxy(){
}

ClientProxy::~ClientProxy(){
}

std::string ClientProxy::classify_image(const std::string image_pathname){
    std::string str("TODO");
    return str;
}
