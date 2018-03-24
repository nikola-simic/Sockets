#pragma once

#include "CommonHeaders.h"

namespace SocketWrapper {

    using AddressFamily = uint8_t;
    using SocketType = uint8_t;
    using Protocol = uint8_t;
    using Address = std::string;
    using Port = std::string;
    using QueueSize = uint16_t;

    enum class SOCKET_STATUS : uint8_t {
        SUCCESS = 0,
        FAILURE = 1
    };

    class Socket {
    public:

        Socket(const Socket&) = delete;
        Socket(Socket&&) = delete;
        Socket& operator=(const Socket&) = delete;
        Socket& operator=(Socket&&) = delete;

    protected:

        static void Initialize();
        static void Uninitialize();

        Socket();
        virtual ~Socket();

    private:

        static const uint32_t MAX_LOG_FILE_SIZE = 1024 * 1024; // in bytes
        static const uint8_t  MAX_LOG_FILE_NUM = 10;

    };
}
