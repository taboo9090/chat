#include "server.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

const int IPv4 = AF_INET;
const int TCP = SOCK_STREAM;

const std::string RED_TEXT = "\033[1;31m";
const std::string GREEN_TEXT = "\033[1;32m";
const std::string RESET_TEXT = "\033[0m";

namespace Net
{
    Server::Server() {
        printf("----------------------------\n");
        // Create the socket
        if ((serverSocket = socket(IPv4, TCP, 0)) == -1)
        {
            printf("- Error creating socket\n.");
        }
        printf("%s+ Socket is created.\n", GREEN_TEXT.c_str());

        // Initialize address configurations
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345);
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        
        printf("+ Confugirations initialized.\n");

        // Bind the socket
        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            printf("- Binding the socket.\n");
            close(serverSocket);
        }
        printf("+ Socket binded.\n");

        // Listen for connections
        if (listen(serverSocket, SOMAXCONN) == -1) {
            printf("- Error listening for connections.\n");
            close(serverSocket);
        }

        printf("+ Listening for connections.%s\n", RESET_TEXT.c_str());
        printf("----------------------------\n");
    }

    Server::~Server() {
        close(serverSocket);
    }

    void Server::handleClients(int clientSocket) {
        std::string clientName;
        // NOTE: First received message is name of client
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
        {
            printf("%s- Connection closed by client.%s\n", RED_TEXT.c_str(), RESET_TEXT.c_str());
            return;
        }
        buffer[bytesRead] = '\0';

        clientName = buffer;
        clientNames[clientSocket] = clientName;
        clientSockets[clientName] = clientSocket;

        printf("+ Client %i connected as %s\n", clientSocket, clientName.c_str());

        while (true)
        {
            bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                printf("%s- Connection closed by client.%s\n", RED_TEXT.c_str(), RESET_TEXT.c_str());
                break;
            }
            buffer[bytesRead] = '\0';
            
            std::cout << ">>> Client " << clientName << ": " << buffer << std::endl;
            // printf(">>> Client %s: %s\n", clientName.c_str(), buffer); -Not working.

            if (buffer[0] == '@') {
                char recipient[256];
                sscanf(buffer, "@%s", recipient);

                auto recipientSocket = clientSockets.find(recipient);
                if (recipientSocket != clientSockets.end()) 
                {   
                    std::string message = GREEN_TEXT + ">>> from " + RESET_TEXT + clientName + " : " + buffer;
                    send(recipientSocket->second, message.c_str(), message.length(), 0);
                } else 
                {
                    std::string notFoundMessage = "\033[1;31mUser not found.\033[0m";
                    send(clientSocket, notFoundMessage.c_str(), notFoundMessage.length(), 0);
                }
            }
            else if (buffer[0] == '/' and buffer[1] == 'l')
            {
                printf("%s+ Available users:%s\n", GREEN_TEXT.c_str(), RESET_TEXT.c_str());
                std::map<std::string, int>::iterator it = clientSockets.begin();
                while (it != clientSockets.end())
                {   
                    if (it->first != clientName)
                    {
                        send(clientSocket, it->first.c_str(), sizeof(buffer), 0);
                        ++it;
                    }
                    
                }
            }
            else
            {
                for (const auto& pair : clientSockets) 
                {
                    if (pair.second != clientSocket) 
                    {
                        std::string message = GREEN_TEXT + ">>> from " + RESET_TEXT + clientName + GREEN_TEXT + " to all: " + RESET_TEXT + buffer;
                        send(pair.second, message.c_str(), message.length(), 0);
                    }
                }
            }
        }
        clientNames.erase(clientSocket);
        clientSockets.erase(clientName);

        close(clientSocket);
    }

    void Server::start() 
    {
        while (true)
        {
            sockaddr_in clientAddr{};
            socklen_t clientAddrLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

            if (clientSocket == -1) 
            {
                std::cerr << RED_TEXT << "Error accepting connection.\n" << RESET_TEXT;
                continue;
            }

            std::thread(&Server::handleClients, this, clientSocket).detach();
        }   
    }

} // namespace Net
