#pragma once

#include <iostream>
#include "../mode.h"

namespace srs {
#if SRS_DEBUG_MODE == 0
    void log(const std::string& msg) {
        return;
    }

#elif SRS_DEBUG_MODE == 1
    void log(const std::string& msg) {
        std::cout << "[LOG] " << msg << std::endl;
        return;
    }
#endif

}