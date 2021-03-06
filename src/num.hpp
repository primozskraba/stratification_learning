#include "util.h"

namespace num {

    // constructors
    template <int modulo>
    constexpr number<modulo>::number(const int& _val) : val(util::mod<modulo>(_val)) {}
    template <int modulo>
    constexpr number<modulo>::number(const num& other) : num(other.val) {}

    template <int modulo>
    inline constexpr number<modulo> number<modulo>::inverse() const {
        return util::inverse<modulo>(val);
    }

    // operators
    template <int modulo>
    constexpr bool number<modulo>::operator ==(const num& other) const {
        return val == other.val;
    }

    template <int modulo>
    constexpr bool number<modulo>::operator !=(const num& other) const {
        return !(val == other.val);
    }

    template <int modulo>
    constexpr number<modulo> number<modulo>::operator -() const {
        return modulo - val;
    }

    template <int modulo>
    constexpr number<modulo> number<modulo>::operator +(const num& b) const {
        return val + b.val;
    }

    template <int modulo>
    constexpr number<modulo> number<modulo>::operator -(const num& b) const {
        return *this + (-b);
    }

    template <int modulo>
    constexpr number<modulo> number<modulo>::operator *(const num& b) const {
        return val * b.val;
    }

    template <int modulo>
    number<modulo>& number<modulo>::operator +=(const num& b) {
        *this = *this + b;
        return *this;
    }

    template <int modulo>
    constexpr bool operator ==(const int& a, const number<modulo>& b) {
        return b == a;
    }

    template <int modulo>
    constexpr number<modulo> operator +(const int& a, const number<modulo>& b) {
        return b + a;
    }

    template <int modulo>
    constexpr number<modulo> operator *(const int& a, const number<modulo>& b) {
        return b * a;
    }

    template <int modulo>
    constexpr number<modulo> operator /(const int& a, const number<modulo>& b) {
        return a * b.inverse();
    }

    template <int modulo>
    std::ostream& operator <<(std::ostream& os, const number<modulo>& num) {
        os << num.value();
        return os;
    }

}
