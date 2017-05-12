#ifndef _TSTEP_H
#define _TSTEP_H

#include <iostream>
#include <cassert>
#include <limits>


namespace ts {

    class tstep {
    private:
        uint32_t ts;

    public:
        /// default constructor (sets to undefined if called without an argument)
        constexpr tstep(const uint32_t& ts=std::numeric_limits<decltype(ts)>::max());

        constexpr uint32_t step() const { return ts; }

        constexpr bool isUndefined() const;


        /// comparison operator
        constexpr bool operator ==(const tstep&) const;
        constexpr bool operator ==(const uint32_t&) const;
        constexpr bool operator !=(const tstep&) const;
        constexpr bool operator <(const tstep&) const;
        constexpr bool operator <=(const tstep&) const;

        /// arithmetic operators
        constexpr tstep operator +(const tstep&) const;
        constexpr tstep operator -(const tstep&) const;

        // XXX think about renaming these
        constexpr bool canMultiplyBy(const tstep&) const;
        constexpr bool canAddBy(const tstep&) const;

        constexpr bool canMultiplyTo(const tstep&) const;
        constexpr bool canAddTo(const tstep&) const;
    };

    std::ostream& operator <<(std::ostream& os, const tstep& ts);
}

#include "tstep.hpp"

#endif
