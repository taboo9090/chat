#include "client.hpp"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>

const int IPv4 = AF_INET;
const int TCP = SOCK_STREAM;

const std::string RED_TEXT = "\033[1;31m";
const std::string GREEN_TEXT = "\033[1;32m";
const std::string RESET_TEXT = "\033[0m";

namespace Net
{
    Client::Client() {
        // Create the socket
        if ((clientSocket = socket(IPv4, TCP, 0)) == -1)
        {
            printf("- Error creating socket\n.");
            return;
        }

        // Initialize address configurations
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");        

        // Connect to the server
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            printf("- Error connecting to server.\n");
            close(clientSocket);
        }

        // Menu
        printf("\n        Welcome to the OS CHAT\n----------------------------------------\nAvailable commands:\n+ @<recipient> <message> - sends a message to the specific user\n+ /list - returns available users\n+ /menu - returns the list of available commands\n+ /exit - exit even in Africa still exit----------------------------------------\n");

        // SignIn
        printf("\n%sEnter your name:%s ", GREEN_TEXT.c_str(), RESET_TEXT.c_str());
        std::cin.getline(buffer, sizeof(buffer));
        send(clientSocket, buffer, sizeof(buffer), 0);
    }

    Client::~Client() {
        close(clientSocket);
    }

    void Client::receiveMessages(int clientSocket) {
        while (true) {
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                printf("%s- Cpnnection closed by server.%s\n", RED_TEXT.c_str(), RESET_TEXT.c_str());
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
