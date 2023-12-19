// server.cpp

#include "server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

const std::string RED_TEXT = "\033[1;31m";
const std::string GREEN_TEXT = "\033[1;32m";
const std::string RESET_TEXT = "\033[0m";

namespace Net
{
    Server::Server() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << RED_TEXT << "Error creating socket.\n" << RESET_TEXT;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << RED_TEXT << "Error binding the socket.\n" << RESET_TEXT;
            close(serverSocket);
        }

        if (listen(serverSocket, SOMAXCONN) == -1) {
            std::cerr << RED_TEXT << "Error listening for connections.\n" << RESET_TEXT;
            close(serverSocket);
        }
    }

    Server::~Server() {
        close(serverSocket);
    }

    void Server::handleClients(int clientSocket) {
        char buffer[1024];
        std::string clientName;

        ssize_t nameLength = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (nameLength <= 0) {
            std::cerr << RED_TEXT << "Connection closed by client.\n" << RESET_TEXT;
            close(clientSocket);
            return;
        }

        buffer[nameLength] = '\0';
        clientName = buffer;
        clientNames[clientSocket] = clientName;
        clientSockets[clientName] = clientSocket;

        std::cout << GREEN_TEXT << "Client" << clientSocket << " connected as " << clientName << RESET_TEXT << std::endl;

        while (true)
        {
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                std::cerr << RED_TEXT << "Connection closed by client.\n" << RESET_TEXT;
                break;
            }

            buffer[bytesRead] = '\0';
            std::cout << "Client " << clientName << ": " << buffer << std::endl;

            if (buffer[0] == '@') {
                char recipient[256];
                sscanf(buffer, "@%s", recipient);

                auto recipientSocket = clientSockets.find(recipient);
                if (recipientSocket != clientSockets.end()) {
                    std::string temp = GREEN_TEXT + ">>> from " + RESET_TEXT + clientName + GREEN_TEXT + " to " + RESET_TEXT + buffer;
                    send(recipientSocket->second, temp.c_str(), temp.length(), 0);
                } else {
                    std::string notFoundMessage = "\033[1;31mUser not found.\033[0m";
                    send(clientSocket, notFoundMessage.c_str(), notFoundMessage.length(), 0);
                }
            } else {
                for (const auto& pair : clientSockets) {
                    if (pair.second != clientSocket) {
                        std::string temp = GREEN_TEXT + ">>> from " + RESET_TEXT + clientName + "to all: " + buffer;
                        send(pair.second, temp.c_str(), temp.length(), 0);
                    }
                }
            }
        }
        
        clientNames.erase(clientSocket);
        clientSockets.erase(clientName);

        close(clientSocket);
    }

    void Server::start() {
        while (true)
        {
            sockaddr_in clientAddr{};
            socklen_t clientAddrLen = sizeof(clientAddr);
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

            if (clientSocket == -1) {
                std::cerr << RED_TEXT << "Error accepting connection.\n" << RESET_TEXT;
                continue;
            }

            std::thread(&Server::handleClients, this, clientSocket).detach();
        }   
    }

} // namespace Net
