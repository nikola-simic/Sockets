#pragma once

#include "Sockets.h"

namespace SocketWrapper {

    class ClientSocket : public Socket {
    public:

        ClientSocket(AddressFamily addressFamily, SocketType socketType, Protocol protocol, Address address, Port port);
        ~ClientSocket();

        SOCKET_STATUS closeSocket();

        int sendData(const char* buffer, int bufferLength, int sendFlags);
        int receiveData(char* buffer, int bufferLength, int receiveFlags);

        // Unavailable constructors
        ClientSocket(const ClientSocket&) = delete;
        ClientSocket(ClientSocket&&) = delete;
        
        // Unavailable operators
        ClientSocket& operator=(const ClientSocket&) = delete;
        ClientSocket& operator=(ClientSocket&&) = delete;
        

    private:

        friend class ServerSocket;
        ClientSocket(SOCKET socket);


        AddressFamily _addressFamily;
        SocketType _socketType;
        Protocol _protocol;
        Address _address;
        Port _port;
        SOCKET _socket;
    };

}