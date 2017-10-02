#include "except.h"

namespace except {

    NotInImageSpaceException::NotInImageSpaceException(const std::string& _msg): msg(_msg) {}

    const char* NotInImageSpaceException::what() const noexcept {
        return msg.c_str();
    }

    AssertException::AssertException(const std::string& _file, const int& _line,
            const std::string& _cond):
        file(_file),
        line(_line),
        cond(_cond) {}

    const char* AssertException::what() const noexcept {
        return (file + " line " + std::to_string(line) + ": " + cond).c_str();
    }
}
