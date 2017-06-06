#include "tstep.h"

namespace ts {

    const int tstep::INFINITY;
    const int tstep::UNDEFINED;

    std::ostream& operator <<(std::ostream& os, const tstep& ts) {
        if (ts.isUndefined()) { os << "undefined"; }
        else if (ts.isInfinity()) { os << "inf"; }
        else { os << "t" << ts.step(); }
        return os;
    }
}
