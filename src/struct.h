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
    template <typename number>
    class Space : public Matrix<number> {
    private:
        // type aliases
        using Mat = Matrix<number>;

    public:
        // inherit all the constructors from matrix
        using Mat::Mat;

        /// returns the number of column vectors that the space contains
        int vector_count() const;
    };

    using BinarySpace = Space<num::binary>;
    using TernarySpace = Space<num::ternary>;

    template <typename number>
    std::ostream& operator <<(std::ostream& os, const Space<number>& space) {
        return os << static_cast<la::Matrix<number>>(space);
    }

    ////////////////////////////////////////
    /// Map: a map between two spaces
    template <typename number>
    class Map : private Matrix<number> {
    private:
        // type aliases
        using Mat = Matrix<number>;

    public:
        // inherit all the constructors from matrix
        using Mat::Mat;

        /// applies itself to the space
        Space<number> operator ()(const Space<number>& space) const;
        /// maps the vector
        TimeVector<number> operator () (const IVector<number>&) const;

        /// finds this maps kernal and image
        void decompose(Space<number>& kernel, Space<number>& image) const;
        /// maps the basis vectors of the input space
        void apply(const Space<number>&, Space<number>&) const;
        /// maps the vector
        void apply(const IVector<number>&, TimeVector<number>&) const;

        /// returns the barcode
        /// TODO: Luka to Primoz: please think of a good name for this function and write a comment :)
        void getDomainImgTimeDiffs(std::vector<std::pair<tstep,tstep>>&) const;

        /// finds a map from the domain space to the image space
        static void find(const Space<number>& domain, Map<number>&, const Space<number>& image);

        template <typename num>
        friend std::ostream& operator <<(std::ostream& os, const Map<num>& map);
    };

    using BinaryMap = Map<binary>;
    using TernaryMap = Map<ternary>;


    ////////////////////////////////////////
    /// Module
    template <typename number>
    class Module {
    private:
        Space<number> generators;
        Map<number> map;
        Space<number> relations;

    public:
        /// extracts the persistence module from the boundry operator
        Module(const Map<number>& boundry);

        /// extracts the barcode from this boundry map
        void getBarcode(std::vector<std::pair<tstep,tstep>>&) const;
    };

    using BinaryModule = Module<binary>;
    using TernaryModule = Module<ternary>;
}

#include "struct.hpp"

#endif
