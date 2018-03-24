#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows headers
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Standard headers
#include <iostream>
#include <sstream>
#include <memory>
#include <thread>
#include <vector>
#include <algorithm>

// Dependencies headers
#include <spdlog\spdlog.h>