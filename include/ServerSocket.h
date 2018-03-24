#pragma once

#include "Sockets.h"
#include "ClientSocket.h"

namespace SocketWrapper {

    using ServerOperation = std::shared_ptr<std::function<void(std::shared_ptr<ClientSocket>)>>;

    class ServerSocket : public Socket {
    public:

        ServerSocket(AddressFamily addressFamily, SocketType socketType, Protocol protocol, Port port, QueueSize queueSize);
        ~ServerSocket();

        SOCKET_STATUS start(ServerOperation serverOperation);
        SOCKET_STATUS stop();
        SOCKET_STATUS closeSocket();

        // Unavailable constructors
        ServerSocket(const ServerSocket&) = delete;
        ServerSocket(ServerSocket&&) = delete;

        // Unavailable operators
        ServerSocket& operator=(const ServerSocket&) = delete;
        ServerSocket& operator=(ServerSocket&&) = delete;

    private:

        void acceptConnections(ServerOperation serverOperation);

        AddressFamily _addressFamily;
        SocketType _socketType;
        Protocol _protocol;
        Port _port;
        QueueSize _queueSize;
        SOCKET _socket; 
        std::atomic<bool> _isExiting;
        std::shared_ptr<std::thread> workerThread;
        struct _ClientContext {
            std::thread _thread;
            SOCKET _socket;
        };
        std::vector<std::shared_ptr<_ClientContext>> _clients;
    };

}
