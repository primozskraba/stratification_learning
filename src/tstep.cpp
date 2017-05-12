#include "tstep.h"

namespace ts {
    std::ostream& operator <<(std::ostream& os, const tstep& ts) {
        if (ts != 0) {
            os << 't' << ts.step();
        }
        return os;
    }
}
