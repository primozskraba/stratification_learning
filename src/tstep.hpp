namespace ts {

    constexpr tstep::tstep(const int& _ts) : ts(_ts) {}

    constexpr bool tstep::isUndefined() const {
        return ts == UNDEFINED;
    }

    constexpr bool tstep::isInfinity() const {
        return ts == INFINITY;
    }

    constexpr bool tstep::operator ==(const tstep& other) const {
        return ts == other.ts;
    }

    constexpr bool tstep::operator ==(const int& other) const {
        return ts == other;
    }

    constexpr bool tstep::operator !=(const tstep& other) const {
        return ts != other.ts;
    }

    constexpr bool tstep::operator <(const tstep& other) const {
        assert(!isUndefined() && !other.isUndefined());
        return ts < other.ts;
    }

    constexpr bool tstep::operator <=(const tstep& other) const {
        assert(!isUndefined() && !other.isUndefined());
        return ts <= other.ts;
    }

    constexpr tstep tstep::operator +(const tstep& other) const {
        assert(!isUndefined() && !other.isUndefined());
        return ts + other.ts;
    }

    constexpr tstep tstep::operator -(const tstep& other) const {
        assert(!isUndefined() && !other.isUndefined());
        return ts - other.ts;
    }

    constexpr bool tstep::canMultiplyBy(const tstep& other) const {
        return other <= *this;
    }

    constexpr bool tstep::canAddBy(const tstep& other) const {
        return other <= *this;
    }

    constexpr bool tstep::canMultiplyTo(const tstep& other) const {
        return *this <= other;
    }

    constexpr bool tstep::canAddTo(const tstep& other) const {
        return *this <= other;
    }

    constexpr bool operator ==(const int& val, const tstep& ts) {
        return ts == val;
    }
}
