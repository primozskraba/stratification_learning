#include "except.h"

namespace except {

    NotInImageSpaceException::NotInImageSpaceException(const std::string& _msg): msg(_msg) {}

    const char* NotInImageSpaceException::what() const noexcept {
        return msg.c_str();
    }
}
