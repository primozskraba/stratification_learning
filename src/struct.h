#ifndef _STRUCT_H
#define _STRUCT_H

#include "linalg.h"
#include "tstep.h"

namespace strct {

    using namespace la;
    using namespace ts;
    using namespace num;

    ////////////////////////////////////////
    /// Space: represented by basis vectors
    template <typename number,typename timeunit=tstep>
    class Space : public Matrix<number,timeunit> {
    private:
        // type aliases
        using Mat = Matrix<number,timeunit>;

    public:
        // inherit all the constructors from matrix
        using Mat::Mat;

        /// returns the number of column vectors that the space contains
        int vector_count() const;
    };

    using BinarySpace = Space<num::binary>;
    using TernarySpace = Space<num::ternary>;

    template <typename number,typename timeunit=tstep>
    std::ostream& operator <<(std::ostream& os, const Space<number,timeunit>& space) {
        return os << static_cast<la::Matrix<number,timeunit>>(space);
    }

    ////////////////////////////////////////
    /// Map: a map between two spaces
    template <typename number,typename timeunit=tstep>
    class Map : private Matrix<number,timeunit> {
    private:
        // type aliases
        using Mat = Matrix<number,timeunit>;

    public:
        // inherit all the constructors from matrix
        using Mat::Mat;
        using Mat::lazyInsert;
        using Mat::copyTimes;
        using Mat::isReducedForm;	
        /// applies itself to the space
        Space<number,timeunit> operator ()(const Space<number,timeunit>& space) const;
        /// maps the vector
        TimeVector<number,timeunit> operator () (const IVector<number,timeunit>&) const;

        /// finds this maps kernal and image
        void decompose(Space<number,timeunit>& kernel, Space<number,timeunit>& image) const;
        /// maps the basis vectors of the input space
        void apply(const Space<number,timeunit>&, Space<number,timeunit>&) const;
        /// maps the vector
        void apply(const IVector<number,timeunit>&, TimeVector<number,timeunit>&) const;

        /// returns the barcode
        /// TODO: Luka to Primoz: please think of a good name for this function and write a comment :)
        void getDomainImgTimeDiffs(std::vector<std::pair<timeunit,timeunit>>&) const;

        /// finds a map from the domain space to the image space
        static void find(const Space<number,timeunit>& domain, Map<number,timeunit>&, const Space<number,timeunit>& image);

        template <typename num,typename tmunit>
        friend std::ostream& operator <<(std::ostream& os, const Map<num,tmunit>& map);
    };

    using BinaryMap = Map<binary>;
    using TernaryMap = Map<ternary>;


    ////////////////////////////////////////
    /// Module
    template <typename number,typename timeunit=tstep>
    class Module {
    private:
        Space<number,timeunit> generators;
        Map<number,timeunit> map;
        Space<number,timeunit> relations;

    public:
        /// extracts the persistence module from the boundry operator
        Module(const Map<number,timeunit>& boundry);

        /// extracts the barcode from this boundry map
        void getBarcode(std::vector<std::pair<timeunit,timeunit>>&) const;
    };

    using BinaryModule = Module<binary>;
    using TernaryModule = Module<ternary>;
}

#include "struct.hpp"

#endif
