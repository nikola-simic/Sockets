#include "PCH.h"
#include "Sockets.h"

const uint32_t MAX_LOG_FILE_SIZE = 1024 * 1024; // in bytes
const uint8_t  MAX_LOG_FILE_NUM = 10;

int Sockets::Initialize() {
    try {
        auto logger = spdlog::rotating_logger_mt("sockets_logger", "../../../out/log.txt", MAX_LOG_FILE_SIZE, MAX_LOG_FILE_NUM);
        logger->info("Initialization succeeded");
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Logging library initialization failed: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

int Sockets::Uninitialize() {
    auto logger = spdlog::get("sockets_logger");
    logger->info("Uninitialization succeeded");
    return 0;
}