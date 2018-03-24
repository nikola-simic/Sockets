#include "ServerSocket.h"

using namespace SocketWrapper;

ServerSocket::ServerSocket(AddressFamily addressFamily, SocketType socketType, Protocol protocol, Port port, QueueSize queueSize):
    _addressFamily(addressFamily),
    _socketType(socketType),
    _protocol(protocol),
    _port(port),
    _queueSize(queueSize),
    _socket(NULL),
    _isExiting(false)
{
    // Fetch address information from Windows
    struct addrinfo hints, *addr;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = _addressFamily;
    hints.ai_socktype = _socketType;
    hints.ai_protocol = _protocol;
    hints.ai_flags = AI_PASSIVE;
    std::stringstream ss;
    ss << _port;
    int status = getaddrinfo(NULL, ss.str().c_str(), &hints, &addr);
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
                status = bind(_socket, addr->ai_addr, (int)addr->ai_addrlen);
                if (status == SOCKET_ERROR) {
                    spdlog::get("sockets_logger")->error("Failed to bind server socket to local address");
                    closesocket(_socket);
                    _socket = NULL;
                }
                else {
                    status = listen(_socket, _queueSize);
                    if (status == SOCKET_ERROR) {
                        spdlog::get("sockets_logger")->error("Failed to listen on local address");
                        closesocket(_socket);
                        _socket = NULL;
                    }
                    else {
                        spdlog::get("sockets_logger")->info("Server socket is listening on local address");
                        spdlog::get("sockets_logger")->flush_on(spdlog::level::debug);
                    }
                }
            }
        }
        freeaddrinfo(addr);
    }
        
}

ServerSocket::~ServerSocket() {
    if(_socket != INVALID_SOCKET) closesocket(_socket);
}

SOCKET_STATUS ServerSocket::start(ServerOperation serverOperation) {
    if (_socket == NULL) {
        spdlog::get("sockets_logger")->error("Starting server socket failed.");
        return SOCKET_STATUS::FAILURE;
    }
    if (_isExiting) {
        spdlog::get("sockets_logger")->error("Starting server socket failed - socket is stopping");
        return SOCKET_STATUS::FAILURE;
    }
    if (!serverOperation && !(*serverOperation)) {
        spdlog::get("sockets_logger")->error("Starting server socket failed - server operation is empty");
        return SOCKET_STATUS::FAILURE;
    }
    workerThread = std::make_shared<std::thread>(std::thread(&ServerSocket::acceptConnections, this, serverOperation));
    if (!workerThread) {
        spdlog::get("sockets_logger")->error("Starting server thread failed.");
        return SOCKET_STATUS::FAILURE;
    }
    return SOCKET_STATUS::SUCCESS;
}

SOCKET_STATUS ServerSocket::stop() {
    if (!workerThread) {
        spdlog::get("sockets_logger")->error("Server thread does not exist");
        return SOCKET_STATUS::FAILURE;
    }
    _isExiting = true;
    closesocket(_socket);
    if (workerThread->joinable()) {
        workerThread->join();
    }
    _isExiting = false;
    spdlog::get("sockets_logger")->info("Server successfully completed its operation");
    spdlog::get("sockets_logger")->flush_on(spdlog::level::debug);
    return SOCKET_STATUS::SUCCESS;
}



void ServerSocket::acceptConnections(ServerOperation serverOperation) {
    if (_socket != NULL) {
        while (!_isExiting) {
            SOCKET client = accept(_socket, NULL, NULL);
            if (client == INVALID_SOCKET) {
                spdlog::get("sockets_logger")->error("Server got invalid client connection. \nClosing listening socket");
                closesocket(_socket);
                _socket = NULL;
                break;
            }
            // Creating new client thread
            std::shared_ptr<ClientSocket> clientSocket(new ClientSocket(client));
            std::shared_ptr<_ClientContext> clientContext = std::make_shared<_ClientContext>(_ClientContext());
            clientContext->_socket = client;
            clientContext->_thread = std::thread(*serverOperation, clientSocket);
            _clients.push_back(clientContext);
        }
        std::for_each(_clients.begin(), _clients.end(), [](std::shared_ptr<_ClientContext>& c) {
            closesocket(c->_socket);
            if (c->_thread.joinable()) c->_thread.detach();
        });
        spdlog::get("sockets_logger")->info("Server thread stopped");
    }
}

