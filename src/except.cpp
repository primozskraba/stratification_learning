#include "except.h"

namespace except {

    NotInImageSpaceException::NotInImageSpaceException(const std::string& _msg): msg(_msg) {}

    const char* NotInImageSpaceException::what() const _NOEXCEPT {
        return msg.c_str();
    }
}
