namespace ts {
    	
    constexpr tstepdouble::tstepdouble(const double& _ts) : ts(_ts) {}

    constexpr bool tstepdouble::isUndefined() const {
        return ts == UNDEFINED;
    }

    constexpr bool tstepdouble::isInfinity() const {
        return ts == INF;
    }


    constexpr bool tstepdouble::operator ==(const tstepdouble& other) const {
        return ts == other.ts;
    }

    constexpr bool tstepdouble::operator ==(const double& other) const {
        return ts == other;
    }

    constexpr bool tstepdouble::operator !=(const tstepdouble& other) const {
        return ts != other.ts;
    }

    constexpr bool tstepdouble::operator <(const tstepdouble& other) const {
        DEBUG_ASSERT(!isUndefined() && !other.isUndefined());
        return ts < other.ts;
    }

    constexpr bool tstepdouble::operator <=(const tstepdouble& other) const {
        DEBUG_ASSERT(!isUndefined() && !other.isUndefined());
        return ts <= other.ts;
    }

    constexpr tstepdouble tstepdouble::operator +(const tstepdouble& other) const {
        DEBUG_ASSERT(!isUndefined() && !other.isUndefined());
        return ts + other.ts;
    }

    constexpr tstepdouble tstepdouble::operator -(const tstepdouble& other) const {
        DEBUG_ASSERT(!isUndefined() && !other.isUndefined());
        return ts - other.ts;
    }

    constexpr bool tstepdouble::canMultiplyBy(const tstepdouble& other) const {
        return other <= *this;
    }

    constexpr bool tstepdouble::canAddBy(const tstepdouble& other) const {
        return other <= *this;
    }

    constexpr bool tstepdouble::canMultiplyTo(const tstepdouble& other) const {
        return *this <= other;
    }

    constexpr bool tstepdouble::canAddTo(const tstepdouble& other) const {
        return *this <= other;
    }

    constexpr bool operator ==(const double& val, const tstepdouble& ts) {
        return ts == val;
    }
}
