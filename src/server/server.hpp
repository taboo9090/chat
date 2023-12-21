#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>

namespace Net
{
    class Server
    {
    private:
        int serverSocket;
        char buffer[1024];
        ssize_t bytesRead;
        std::map<int, std::string> clientNames;
        std::map<std::string, int> clientSockets;

        void handleClients(int);
    public:
        Server();
        ~Server();

        void start();
    };
    
} // namespace Net


#endif