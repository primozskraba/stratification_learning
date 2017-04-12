#ifndef _NUM_H
#define _NUM_H

#include <array>
#include <utility>

#include "util.h"

namespace num {

    template <int modulo>
    class number {
        // internal alias
        using num = number<modulo>;

        int val{0};    // XXX consider changing to something else
    public:
        constexpr number(const int&);
        constexpr number(const num&);

        constexpr int value() { return val; }

        constexpr num inverse();

        // equality
        constexpr bool operator ==(const num&);

        constexpr num operator -();

        // addition
        constexpr num operator +(const num&);
        constexpr num operator -(const num&);
        constexpr num operator *(const num&);
    };

    template <int modulo>
    constexpr number<modulo> operator +(const int&, const number<modulo>&);
    template <int modulo>
    constexpr number<modulo> operator *(const int&, const number<modulo>&);

    // type aliases
    using binary = number<2>;
    using ternary = number<3>;

}

#include "num.hpp"

#endif
