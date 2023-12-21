#ifndef CLIENT_HPP
#define CLIENT_HPP

namespace Net
{
    class Client
    {
    private:
        int clientSocket;
        char buffer[1024];
        
        void receiveMessages(int);
    public:
        Client();
        ~Client();

        void start();
    };

} // namespace Net


#endif