// client.cpp

#include "client.hpp"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>

#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define RESET_TEXT "\033[0m"

namespace Net
{
    Client::Client() {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Error creating socket.\n";
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345);
        inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << RED_TEXT << "Error connecting to server.\n" << RESET_TEXT;
            close(clientSocket);
        }

        std::cout << "Connected to server.\n";

        char buffer[256];
        std::cout << "Enter your name: ";
        std::cin.getline(buffer, sizeof(buffer));
        send(clientSocket, buffer, strlen(buffer), 0);
    
    }

    Client::~Client() {
        close(clientSocket);
    }

    void Client::receiveMessages(int clientSocket) {
        char buffer[1024];
        while (true) {
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                std::cerr << RED_TEXT << "Connection closed by server.\n" << RESET_TEXT;
                break;
            }

            buffer[bytesRead] = '\0';
            std::cout << buffer << std::endl;
        }
    }

    void Client::start() {
        std::thread(&Client::receiveMessages, this, clientSocket).detach();

        char buffer[1024];
        while (true)
        {
            std::cin.getline(buffer, sizeof(buffer));
            send(clientSocket, buffer, strlen(buffer), 0);
        }

        close(clientSocket);
    }

} // namespace Net
