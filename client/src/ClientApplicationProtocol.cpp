/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client Application Protocol
*/

#include "ClientApplicationProtocol.h"

std::vector<char> ClientApplicationProtocol::load_file(std::string filename)
{
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(result.data(), pos);

    return result;
}

int ClientApplicationProtocol::request(const std::string image_pathname, const std::string server_ip, const int server_port){
    int err = 0;

    // Setting the Server Address:
    err = socket->socket_set_server(server_ip, server_port);
    if(err == -1) return -1;

    // Connecting to the Server:
    err = socket->socket_open();
    if(err == -1) return -1;

    // Loading the Image to a vector:
    std::vector<char> image = load_file(image_pathname);

    // Getting the Image size:
    long image_size = image.size();

    // Sending the Image Size:
    err = socket->socket_send_data(&image_size, sizeof(long));
    if(err == -1) return -1;

    void * image_addr = (void *)(image.data());

    // Sending the whole Image:
    void * current_addr = image_addr;
    size_t remaining_bytes = image_size;
    while(remaining_bytes > 0){
        ssize_t current_bytes_sent;
        err = socket->socket_send_data(current_addr, remaining_bytes, current_bytes_sent);

        if(err == -1) return -1;
        current_addr = (void*)((char*)current_addr + current_bytes_sent);
        remaining_bytes -= current_bytes_sent;
    }

    fprintf(stdout, "[OK - ClientApplicationProtocol] Request completed correctly\n");
    return 0;
}

int ClientApplicationProtocol::response(std::string &result){
    int err = 0;

    size_t response_size;
    err = socket->socket_recv_data(&response_size, sizeof(size_t));
    if(err == -1) return -1;
    fprintf(stdout, "[OK - ClientApplicationProtocol] Response size to receive: %lu\n", response_size);

    char * result_buffer = new char[response_size];
    err = socket->socket_recv_data(result_buffer, response_size);
    if(err == -1) return -1;
    result = std::string(result_buffer);
    delete[] result_buffer;

    fprintf(stdout, "[Ok - ClientApplicationProtocol] Response completed correctly\n");
    return 0;
}

ClientApplicationProtocol::ClientApplicationProtocol(){
    socket = new SocketTCP();
}

ClientApplicationProtocol::~ClientApplicationProtocol(){
    delete(socket);
}

// Performing a Request and Getting a Response:
std::string ClientApplicationProtocol::perform(const std::string image_pathname, const std::string server_ip, const int server_port){
    int err = 0;

    err = request(image_pathname, server_ip, server_port);
    if(err == -1){
        // Bad Request:
        std::string str(CAP_BAD_REQUEST);
        return str;
    }
    std::string result;
    err = response(result);
    if(err == -1){
        // Bad Response:
        std::string str(CAP_BAD_RESPONSE);
        return str;
    }

    return result;
}

void ClientApplicationProtocol::send_feedback(const bool satisfaction){
    int err = socket->socket_send_data(&satisfaction, sizeof(bool));
    //if(err == -1) return -1;
    //return 0;
}
