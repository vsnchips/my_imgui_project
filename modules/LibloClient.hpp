#ifndef LIBLO_CLIENT_H
#define LIBLO_CLIENT_H

#include <lo/lo.h>
#include <string>
#include <vector>

class LibloClient {
public:
    LibloClient(const std::string &server_address, int port);
    ~LibloClient();

    void update();

    // Data structures to store different types of messages
    std::string lastStringMessage;
    int lastIntMessage;
    float lastFloatMessage;
    // Add more as needed...

private:
    lo_server_thread st;

    // Static callback function
    static int genericHandler(const char *path, const char *types, lo_arg **argv,
                              int argc, lo_message msg, void *user_data);

    // Non-static function to handle messages
    void handleMessages(const char *path, const char *types, lo_arg **argv,
                        int argc, lo_message msg);
};

#endif // LIBLO_CLIENT_H
