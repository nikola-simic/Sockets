#include "ClientSocket.h"

using namespace SocketWrapper;

ClientSocket::ClientSocket(AddressFamily addressFamily, SocketType socketType, Protocol protocol, Port port) {

}
ClientSocket::ClientSocket(SOCKET socket): _socket(socket) {}


ClientSocket::~ClientSocket() {

}

SOCKET_STATUS ClientSocket::closeSocket() {
    closesocket(_socket);
    return SOCKET_STATUS::SUCCESS;
}