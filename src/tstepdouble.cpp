#include "tstepdouble.h"

namespace ts {

    /* const double tstepdouble::INF; */
    /* const double tstepdouble::UNDEFINED; */

    std::ostream& operator <<(std::ostream& os, const tstepdouble& ts) {

        if (ts.isUndefined()) { os << "undefined"; }
        else if (ts.isInfinity()) { os << "inf"; }
        else { os << "t" << ts.step(); }
        return os;
    }
}
