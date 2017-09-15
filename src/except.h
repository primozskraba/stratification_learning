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
        explicit AssertException(const std::string& line, const std::string& cond);
        ~AssertException() noexcept {}

        const char* what() const noexcept;
    private:
        std::string line;
        std::string cond;
    };
}

#ifdef NDEBUG
#define DEBUG_ASSERT(cond)
#else
#define DEBUG_ASSERT(COND) \
    ((COND) ? \
         static_cast<void>(0) : \
         throw except::AssertException( \
             static_cast<std::ostringstream&>(std::ostringstream() << __FILE__ << " line " << __LINE__).str(), \
             #COND \
         ) \
     )
#endif
#define ASSERT(COND) \
     ((COND) ? \
          static_cast<void>(0) : \
          throw except::AssertException( \
              static_cast<std::ostringstream&>(std::ostringstream() << __FILE__ << " line " << __LINE__).str(), \
              #COND \
          ) \
      )

#endif
