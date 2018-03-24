#include "ClientSocket.h"

using namespace SocketWrapper;

ClientSocket::ClientSocket(AddressFamily addressFamily, SocketType socketType, Protocol protocol, Address address, Port port):
    _addressFamily(addressFamily),
    _socketType(socketType),
    _protocol(protocol),
    _address(address),
    _port(port),
    _socket(NULL)
{
    // Fetch address information from Windows
    struct addrinfo hints, *addr;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = _addressFamily;
    hints.ai_socktype = _socketType;
    hints.ai_protocol = _protocol;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(_address.c_str(), _port.c_str(), &hints, &addr);
    if (status != 0) {
        spdlog::get("sockets_logger")->error("Failed to get socket address");
    }
    else {
        if (addr->ai_family != _addressFamily || addr->ai_socktype != _socketType || addr->ai_protocol != _protocol) {
            spdlog::get("sockets_logger")->error("Discovered socket parameters does not match with user's");
        }
        else {
            // If address information is successfully fetched - try to create socket
            _socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
            if (_socket == INVALID_SOCKET) {
                spdlog::get("sockets_logger")->error("Failed to create server socket");
                _socket = NULL;
            }
            else {
                status = connect(_socket, addr->ai_addr, (int)addr->ai_addrlen);
                if (status != 0) {
                    spdlog::get("sockets_logger")->error("Failed to connect client socket");
                    closesocket(_socket);
                    _socket = NULL;
                }
            }
        }
        freeaddrinfo(addr);
    }
}

ClientSocket::ClientSocket(SOCKET socket): _socket(socket) {
    if (_socket == INVALID_SOCKET) {
        closesocket(_socket);
        _socket = NULL;
    }
}


ClientSocket::~ClientSocket() {
    closesocket(_socket);
    _socket = NULL;
}

SOCKET_STATUS ClientSocket::closeSocket() {
    closesocket(_socket);
    return SOCKET_STATUS::SUCCESS;
}

int ClientSocket::sendData(const char* buffer, int bufferLength, int sendFlags) {
    return send(_socket, buffer, bufferLength, (int)sendFlags);
}

int ClientSocket::receiveData(char* buffer, int bufferLength, int receiveFlags) {
    return recv(_socket, buffer, bufferLength, (int)receiveFlags);
}