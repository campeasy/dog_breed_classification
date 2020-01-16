/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Proxy
*/

#include "ClientProxy.h"

void ClientProxy::print_servers(){
    fprintf(stdout, "[ClientProxy] Servers List:\n");
    for(int i = 0; i < ip_addrexes.size(); ++i){
        fprintf(stdout, "Server n. %d - (%s : %d)\n", i+1, ip_addrexes[i].c_str(), ports[i]);
    }
    fprintf(stdout, "\n");
    return;
}

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
    client_application_protocol = new ClientApplicationProtocol();
}

ClientProxy::~ClientProxy(){
    delete client_application_protocol;
}

std::string ClientProxy::classify_image(const std::string image_pathname){
    int i = 0, number_of_attempts = 0, success = 0;
    std::string result, bad_request(CAP_BAD_REQUEST), bad_response(CAP_BAD_RESPONSE);

    while(success == 0){
        ++number_of_attempts;
        fprintf(stdout, "\n[ClientProxy] Attempt %d - Server (%s : %d) \n", number_of_attempts, ip_addrexes[i].c_str(), ports[i]);

        result = client_application_protocol->perform(image_pathname, ip_addrexes[i], ports[i]);

        if(result.compare(bad_request) == 0 || result.compare(bad_response) == 0){
            success = 0;
            fprintf(stderr, "[FAIL - ClientProxy] Can't contact Server (%s : %d)\n", ip_addrexes[i].c_str(), ports[i]);
        }
        else{
            success = 1;
            fprintf(stdout, "[OK - ClientProxy] Server (%s : %d) correctly responded\n", ip_addrexes[i].c_str(), ports[i]);
        }

        // If this is the last Server but we had no success, than reset the pointer to the first Server:
        if(i == ip_addrexes.size() - 1 && success == 0) i = 0;
        // Else switch to the next Server:
        else i++;
    }

    return result;
}

void ClientProxy::feedback(const bool satisfaction){
    client_application_protocol->send_feedback(satisfaction);
}
