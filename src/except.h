#ifndef _EXCEPT_H
#define _EXCEPT_H

#include <string>

namespace except {

    class NotInImageSpaceException : public std::exception {
    public:
        explicit NotInImageSpaceException(const std::string& msg);
        ~NotInImageSpaceException() noexcept {}

        const char* what() const noexcept;
    private:
        std::string msg;
    };
}

#endif
