#ifndef _STRUCT_H
#define _STRUCT_H

#include "linalg.h"

namespace strct {

    ////////////////////////////////////////
    /// Space: represented by basis vectors
    template <typename number>
    class Space : public la::Matrix<number> {
    private:
        // type aliases
        using Mat = la::Matrix<number>;

    public:
        // inherit all the constructors from matrix
        using Mat::Mat;

        /// returns the number of column vectors that the space contains
        int vector_count() const;
    };

    using BinarySpace = Space<num::binary>;
    using TernarySpace = Space<num::ternary>;

    ////////////////////////////////////////
    /// Map: a map between two spaces
    template <typename number>
    class Map : private la::Matrix<number> {
    private:
        // type aliases
        using Mat = la::Matrix<number>;
        using vector = typename Mat::vector;

    public:
        // inherit all the constructors from matrix
        using Mat::Mat;

        /// applies itself to the space
        Space<number> operator ()(const Space<number>& space) const;
        /// maps the vector
        vector operator () (const vector&) const;
        
        void decompose(Space<number>& kernel, Space<number>& image) const;
        void apply(const Space<number>&, Space<number>&) const;
        void apply(const vector&, vector&) const;
    };

    using BinaryMap = Map<num::binary>;
    using TernaryMap = Map<num::ternary>;


    ////////////////////////////////////////
    /// Module
    template <typename number>
    class Module {
    private:
        Space<number> generators;
        Map<number> map;
        Space<number> relations;

    public:
    };

    using BinaryModule = Module<num::binary>;
    using TernaryModule = Module<num::ternary>;
}

#include "struct.hpp"

#endif
