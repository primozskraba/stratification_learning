#ifndef _EXCEPT_H
#define _EXCEPT_H

#include <string>
#include <sstream>

namespace except {

    class NotInImageSpaceException : public std::exception {
    public:
        explicit NotInImageSpaceException(const std::string& msg);
        ~NotInImageSpaceException() noexcept {}

        const char* what() const noexcept;
    private:
        std::string msg;
    };

    class AssertException : public std::exception {
    public:
        explicit AssertException(const std::string& file, const int& line,
                const std::string& cond);
        ~AssertException() noexcept {}

        const char* what() const noexcept;
    private:
        std::string file;
        int line;
        std::string cond;
    };
}

#ifdef NDEBUG
#define DEBUG_ASSERT(cond)
#else
#define DEBUG_ASSERT(COND) \
    if (!(COND)) { \
        throw except::AssertException( \
                std::string(__FILE__), \
                __LINE__, \
                std::string(#COND) \
                ); \
    }
#endif
#define ASSERT(COND) \
    if (!(COND)) { \
        throw except::AssertException( \
                std::string(__FILE__), \
                __LINE__, \
                std::string(#COND) \
                ); \
    }
#endif
