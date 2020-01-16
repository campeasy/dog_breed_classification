/*
    December 2019
    Dog Breed Classification - Salvatore Campisi, Daniele Calanna
    Advanced Programming Languages

    Client
*/

#include "Client.h"

Client::Client(){
    // Building the "local" Classificator:
    proxy = ClientProxyFactory::create_ClientProxyInterface();
}

Client::~Client(){
    // Destroying the "local" Classificator:
    delete proxy;
}

bool Client::get_feedback_from_user(){
    fprintf(stdout, "Are you satisfied? (Yes/No): ");
    char feedback;
    while(!(feedback == 'Y' || feedback == 'N' || feedback == 'y' || feedback == 'n')){
        fscanf(stdin, "%c", &feedback);
    }
    return (feedback == 'Y' || feedback == 'y');
}

void Client::run_client(const std::string image_pathname){
    std::string result;

    result = proxy->classify_image(image_pathname.c_str());

    fprintf(stdout, "\n---------------[CLIENT - COMPUTATION RESULT]---------------\n");
    fprintf(stdout, "%s", result.c_str());
    fprintf(stdout, "-----------------------------------------------------------\n");

    proxy->feedback(get_feedback_from_user());
    return;
}
