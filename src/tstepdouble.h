#ifndef _TSTEPDOUBLE_H
#define _TSTEPDOUBLE_H

#include <iostream>
#include <cassert>
#include <limits>


namespace ts {
    class tstepdouble {
    private:
        double ts;

    public:
        static constexpr double INF= std::numeric_limits<decltype(ts)>::infinity();
        static constexpr double UNDEFINED = std::numeric_limits<decltype(ts)>::max();

        /// default constructor (sets to undefined if called without an argument)
        constexpr tstepdouble(const double& ts=UNDEFINED);

        constexpr double step() const { return ts; }

        constexpr bool isUndefined() const;
        constexpr bool isInfinity() const;


        /// comparison operator
        constexpr bool operator ==(const tstepdouble&) const;
        constexpr bool operator ==(const double&) const;
        constexpr bool operator !=(const tstepdouble&) const;
        constexpr bool operator <(const tstepdouble&) const;
        constexpr bool operator <=(const tstepdouble&) const;

        /// arithmetic operators
        constexpr tstepdouble operator +(const tstepdouble&) const;
        constexpr tstepdouble operator -(const tstepdouble&) const;

        // XXX think about renaming these
        constexpr bool canMultiplyBy(const tstepdouble&) const;
        constexpr bool canAddBy(const tstepdouble&) const;

        constexpr bool canMultiplyTo(const tstepdouble&) const;
        constexpr bool canAddTo(const tstepdouble&) const;
    };

    constexpr bool operator ==(const double&, const tstepdouble&);

    std::ostream& operator <<(std::ostream& os, const tstepdouble& ts);
    
}

#include "tstepdouble.hpp"

#endif
