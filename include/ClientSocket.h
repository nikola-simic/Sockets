#pragma once

#include "Sockets.h"

namespace SocketWrapper {

    class ClientSocket : public Socket {
    public:

        ClientSocket(AddressFamily addressFamily, SocketType socketType, Protocol protocol, Port port);
        ~ClientSocket();

        SOCKET_STATUS closeSocket();

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
        Port _port;
        SOCKET _socket;

    };

}