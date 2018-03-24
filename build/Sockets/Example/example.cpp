#include "ServerSocket.h"
#include "ClientSocket.h"
#include <iostream>

using namespace SocketWrapper;

void serverOperation(std::shared_ptr<ClientSocket> client) {
    std::cout << "Server operation" << std::endl;
    char buf[100];

    // Receive data from client
    int receivedSize = client->receiveData(buf, 100, 0);

    std::cout << "Server received from client: " << buf << std::endl;

    // Echo back data to client
    client->sendData(buf, receivedSize, 0);

    // Close client socket
    client->closeSocket();
}

int main() {
   
    // Socket configuration
    const Address serverAddress = "192.168.0.103";
    const Port serverPort = "25000";
    const QueueSize serverQueueSize = 100;

    // Create server socket
    std::shared_ptr<ServerSocket> server(new ServerSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, serverAddress, serverPort, serverQueueSize));
    ServerOperation f(new std::function<void(std::shared_ptr<ClientSocket>)>(serverOperation));

    // Start server socket
    server->start(f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    // Create client socket
    std::shared_ptr<ClientSocket> client(new ClientSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, serverAddress, serverPort));
    
    // Send data to server
    std::string sendBuf("Hello, server!");
    client->sendData(sendBuf.c_str(), sendBuf.size(), 0);

    // Receive data from server
    char receiveBuf[100];
    client->receiveData(receiveBuf, 100, 0);
    std::cout << "Client received from server: " << receiveBuf << std::endl;

    // Stop server
    server->stop();


    int n;
    std::cin >> n;
    return 0;
}