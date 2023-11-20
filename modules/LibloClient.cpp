#include "LibloClient.hpp"
#include <iostream>

void error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

LibloClient::LibloClient(const std::string &server_address, int port) {
    std::cout << "Creating new lo server" << std::endl;
    /* start a new server on port 7770 */
    lo_server_thread st = lo_server_thread_new("7770", error);

    /* add method that will match any path and args */
    lo_server_thread_add_method(st, NULL, NULL, genericHandler, this);
    lo_server_thread_start(st);

}

LibloClient::~LibloClient() {
    std::cout<< "Stopping osc client" << std::endl;
    lo_server_thread_stop(st);
    std::cout<< "Freeing osc client" << std::endl;
    lo_server_thread_free(st);
}

void LibloClient::update() {
    //lo_server_recv_noblock(server, 0);
    // Optionally print the last messages received
    // Add more as needed...
}

int LibloClient::genericHandler(const char *path, const char *types, lo_arg **argv,
                                int argc, lo_message msg, void *user_data) {
    std::cout << "Generic Handling " << argc << " Messages." << std::endl;
    LibloClient* client = static_cast<LibloClient*>(user_data);
    if (client) {
        client->handleMessages(path, types, argv, argc, msg);
    }
    return 1;
}

void LibloClient::handleMessages(const char *path, const char *types, lo_arg **argv,
                                 int argc, lo_message msg) {

    std::cout << "Handling " << argc << " Messages." << std::endl;
    for (int i = 0; i < argc; ++i) {
        if (types[i] == 'i') {
            lastIntMessage = argv[i]->i;
    std::cout << "Last int message: " << lastIntMessage << std::endl;
        } else if (types[i] == 'f') {
            lastFloatMessage = argv[i]->f;
    std::cout << "Last float message: " << lastFloatMessage << std::endl;
        } else if (types[i] == 's') {
            lastStringMessage = argv[i]->s;
    std::cout << "Last string message: " << lastStringMessage << std::endl;
        }
        // Add more cases for other types as needed
    }
}