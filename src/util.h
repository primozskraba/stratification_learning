#ifndef _UTIL_H
#define _UTIL_H

#include <array>
#include <utility>

namespace util {

    constexpr int mod(const int& val, const int& base);

    namespace helpers {
        template <int modulo>
        constexpr int get_inverse_single(const int& val, const int& inv_candidate) {
            return inv_candidate == 0 ? 0 : (mod(val*inv_candidate, modulo) == 1 ? inv_candidate : get_inverse_single<modulo>(val, inv_candidate-1));
        }
        
        template <int modulo, int... Is>
        constexpr std::array<int, modulo> calc_inverses(std::integer_sequence<int,Is...>) {
            return std::array<int,modulo>{{ get_inverse_single<modulo>(Is, modulo-1)... }};
        }

        template <int modulo>
        struct InverseHelper {
            static constexpr std::array<int,modulo> inverses = calc_inverses<modulo>(std::make_integer_sequence<int,modulo>());
        };

        template <int modulo>
        constexpr std::array<int,modulo> InverseHelper<modulo>::inverses;
    }

    constexpr int mod(const int& val, const int& base) {
        return val >= 0 ? val % base : mod(val + (-val / base + 1)*base, base);
    }

    template <int modulo>
    constexpr int inverse(const int& val) {
        return 0 <= val && val < modulo ?
            helpers::InverseHelper<modulo>::inverses[val] :
            throw std::range_error("Invalid range for inverse!");
    }
}

#endif
