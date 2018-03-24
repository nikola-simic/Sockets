#include "ServerSocket.h"
#include "ClientSocket.h"
#include <iostream>

using namespace SocketWrapper;

void serverOperation(std::shared_ptr<ClientSocket> client) {
    std::cout << "Server operation" << std::endl;
    client->closeSocket();
}

int main() {
   
    const Port serverPort = 25000;
    const QueueSize serverQueueSize = 100;
    std::shared_ptr<ServerSocket> server(new ServerSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, serverPort, serverQueueSize));
    ServerOperation f(new std::function<void(std::shared_ptr<ClientSocket>)>(serverOperation));
    server->start(f);
    server->stop();
    return 0;
}