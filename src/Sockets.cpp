#include "Sockets.h"

using namespace SocketWrapper;

Socket::Socket() {
    Initialize();
}

Socket::~Socket() {
    Uninitialize();
}

void Socket::Initialize() {
    try {
        // Initialize logger
        auto logger = spdlog::get("sockets_logger");
        if (!logger) {
            logger = spdlog::rotating_logger_mt("sockets_logger", "../../../out/log.txt", MAX_LOG_FILE_SIZE, MAX_LOG_FILE_NUM);
        }
       
        // Initialize WinSock subsystem
        uint16_t winSockVer = MAKEWORD(2, 2);
        WSAData wsaData;
        int winSockStat = WSAStartup(winSockVer, &wsaData);
        if (!winSockStat) {
            
            if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
                WSACleanup();
            }
            else {
                logger->info("Windows socket subsystem started - version: 2.2");
            }
        }
        else {
            logger->info("Windows socket subsystem failed to start");

        }
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Logging library initialization failed: " << ex.what() << std::endl;
    }
}

void Socket::Uninitialize() {
    WSACleanup();
}