#ifndef _TSTEP_H
#define _TSTEP_H

#include <iostream>
#include <limits>

#include "except.h"


namespace ts {

    class tstep {
    private:
        int ts;

    public:
        using val_type = int;

        static constexpr int INF= std::numeric_limits<decltype(ts)>::max();
        static constexpr int UNDEFINED = std::numeric_limits<decltype(ts)>::max()-1;

        /// default constructor (sets to undefined if called without an argument)
        constexpr tstep(const int& ts=UNDEFINED);

        constexpr int step() const { return ts; }

        constexpr bool isUndefined() const;
        constexpr bool isInfinity() const;


        /// comparison operator
        constexpr bool operator ==(const tstep&) const;
        constexpr bool operator ==(const int&) const;
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

    constexpr bool operator ==(const int&, const tstep&);

    std::ostream& operator <<(std::ostream& os, const tstep& ts);

}

#include "tstep.hpp"

#endif
