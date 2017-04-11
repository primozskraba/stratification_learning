#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>

#include <ctime>
#include <random>


#include <CGAL/Epick_d.h>
#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/algorithm.h>
#include <CGAL/Timer.h>
#include <CGAL/assertions.h>



#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/Periodic_3_triangulation_traits_3.h>
#include <CGAL/Periodic_3_Delaunay_triangulation_3.h>


#include "Miniball.h"

// eventually move this somewhere else - but right now its the fastest possible way

typedef double* const* PointIterator;
typedef const double* CoordIterator;
typedef Miniball:: Miniball <Miniball::CoordAccessor<PointIterator, CoordIterator> >  MB;


template<typename D>
class geometricComplex{
protected:
        typedef CGAL::Epick_d< CGAL::Dimension_tag<D> >               K;
        typedef CGAL::Delaunay_triangulation<K>                       T;

public:
    
        insert();
    
    filterComplex();
        readInPoints();
    
    
        writeOut();
    boundary();
    relativeBoundary();
    
};

template<>
class <>


struct K: CGAL::Exact_predicates_exact_constructions_kernel {};



// move typedefs to latter
typedef CGAL::Delaunay_triangulation_2<K>           Delaunay2D;
typedef Delaunay2D::Point                           Point;
typedef Delaunay2D::Vertex_handle                   Vertex_handle;
typedef Delaunay2D::Face_handle                     Face_handle;
typedef K::FT                                       RealValue;

typedef Delaunay2D::Finite_vertices_iterator        Vertex_iterator;
typedef Delaunay2D::Finite_edges_iterator           Edge_iterator;
typedef Delaunay2D::Finite_faces_iterator           Face_iterator;




typedef CGAL::Delaunay_triangulation_3<K>           Delaunay3D;
typedef Delaunay3D::Point                           Point;
typedef Delaunay3D::Periodic_point                  Periodic_point;

typedef Delaunay3D::Vertex_handle                   Vertex_handle;
typedef Delaunay3D::Cell_handle                     Cell_handle;
typedef Delaunay3D::Iso_cuboid                      Iso_cuboid;
typedef K::FT                                       RealValue;


typedef Delaunay3D::Finite_vertices_iterator        Vertex_iterator;
typedef Delaunay3D::Finite_edges_iterator           Edge_iterator;
typedef Delaunay3D::Finite_facets_iterator          Facet_iterator;
typedef Delaunay3D::Finite_cells_iterator           Cell_iterator;
typedef Delaunay3D::Facet_circulator                Facet_circulator;

