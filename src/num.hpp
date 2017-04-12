#include "util.h"

namespace num {
    // constructors
    template <int modulo>
    constexpr number<modulo>::number(const int& _val) : val(util::mod(_val, modulo)) {}
    template <int modulo>
    constexpr number<modulo>::number(const num& other) : num(other.val) {}

    template <int modulo>
    inline constexpr number<modulo> number<modulo>::inverse() {
        return util::inverse<modulo>(val);
    }

    // operators
    template <int modulo>
    constexpr bool number<modulo>::operator ==(const num& other) {
        return val == other.val;
    }

    template <int modulo>
    constexpr bool number<modulo>::operator !=(const num& other) {
        return !(val == other.val);
    }

    template <int modulo>
    constexpr number<modulo> number<modulo>::operator -() {
        return modulo - val;
    }

    template <int modulo>
    constexpr number<modulo> number<modulo>::operator +(const num& b) {
        return val + b.val;
    }

    template <int modulo>
    constexpr number<modulo> number<modulo>::operator -(const num& b) {
        return *this + (-b);
    }

    template <int modulo>
    constexpr number<modulo> number<modulo>::operator *(const num& b) {
        return val * b.val;
    }

    template <int modulo>
    constexpr number<modulo> operator +(const int& a, const number<modulo>& b) {
        return b + a;
    }

    template <int modulo>
    constexpr number<modulo> operator *(const int& a, const number<modulo>& b) {
        return b * a;
    }
}
