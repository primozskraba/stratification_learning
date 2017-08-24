#ifndef _EXCEPT_H
#define _EXCEPT_H

#include <string>

namespace except {

    class NotInImageSpaceException : public std::exception {
    public:
        explicit NotInImageSpaceException(const std::string& msg);
        ~NotInImageSpaceException() _NOEXCEPT {}

        const char* what() const _NOEXCEPT;
    private:
        std::string msg;
    };
}

#endif
