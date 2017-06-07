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

    template <typename number>
    std::ostream& operator <<(std::ostream& os, const Space<number>& space) {
        return os << static_cast<la::Matrix<number>>(space);
    }

    ////////////////////////////////////////
    /// Map: a map between two spaces
    template <typename number>
    class Map : private la::Matrix<number> {
    private:
        // type aliases
        using Mat = la::Matrix<number>;
        /* using TimeVector = typename Mat::TimeVector; */

    public:
        // inherit all the constructors from matrix
        using Mat::Mat;
	
        // lazy insertion - no guarantee time vectors are ok
//	void insert(TimeVector<num>)

	// low level check the map is well formed
	bool verify();

        /// applies itself to the space
        Space<number> operator ()(const Space<number>& space) const;
        /// maps the vector
        la::TimeVector<number> operator () (const la::IVector<number>&) const;
        
        void decompose(Space<number>& kernel, Space<number>& image) const;
        void apply(const Space<number>&, Space<number>&) const;
        void apply(const la::IVector<number>&, la::TimeVector<number>&) const;
    };

    using BinaryMap = Map<num::binary>;
    using TernaryMap = Map<num::ternary>;


    ////////////////////////////////////////
    /// Module
 //   template <typename number>
   // class Module {
  //  private:
    //    Space<number> generators;
      //  Map<number> map;
      //  Space<number> relations;

   // public:
   // };

  //  using BinaryModule = Module<num::binary>;
   // using TernaryModule = Module<num::ternary>;
}

#include "struct.hpp"

#endif
