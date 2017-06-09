#ifndef _NUM_H
#define _NUM_H

#include <iostream>

namespace num {

    template <int modulo>
    class number {
        // internal alias
        using num = number<modulo>;

        int val;    // XXX consider changing to something else
    public:
        constexpr number(const int& val=0);
        constexpr number(const num& val=0);

        constexpr int value() const { return val; }

        constexpr num inverse() const;

        // equality
        constexpr bool operator ==(const num&) const;
        constexpr bool operator !=(const num&) const;

        constexpr num operator -() const;

        // basic operations
        constexpr num operator +(const num&) const;
        constexpr num operator -(const num&) const;
        constexpr num operator *(const num&) const;

        num& operator +=(const num&);
    };

    template <int modulo>
    constexpr bool operator ==(const int&, const number<modulo>&);

    template <int modulo>
    constexpr number<modulo> operator +(const int&, const number<modulo>&);
    template <int modulo>
    constexpr number<modulo> operator *(const int&, const number<modulo>&);

    template <int modulo>
    constexpr number<modulo> operator /(const int&, const number<modulo>&);

    template <int modulo>
    std::ostream& operator <<(std::ostream&, const number<modulo>&);

    // type aliases
    using binary = number<2>;
    using ternary = number<3>;

}

#include "num.hpp"

#endif
