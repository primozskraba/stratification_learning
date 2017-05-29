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

namespace geometric{

// eventually move this somewhere else - but right now its the fastest possible way

typedef double* const* PointIterator;
typedef const double* CoordIterator;
typedef Miniball:: Miniball <Miniball::CoordAccessor<PointIterator, CoordIterator> >  MB;

class Simplex{
};

class filteredComplex{
};


double simplexValue(Simplex S){
	double *coords[S.size()];
        for(int i = 0 ; i<S.size();++i){
		coords[i] = new double[];
		
	}	

coords[0] = new double[2];
    coords[1] = new double[2];
    coords[2] = new double[2];
	
}


template<const int D>
class geometricComplex{
protected:
        typedef CGAL::Epick_d< CGAL::Dimension_tag<D> >               K;
        typedef CGAL::Delaunay_triangulation<K>                       T;
	
	T triangulation(D);

public:
	void insert(T){
		triangulation.insert(points.begin(), points.end());
	}    
     	
	void filterComplex(){
		// code is simpler with a recursive call 
		// allocate 
	}
     
	readInPoints();
    
    
     writeOut();
     boundary();
     relativeBoundary();
    
};

template<>
class  geometricComplex<2>
{
protected:
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

public:
	void insert(){
		std::cout<<"silly1"<<std::endl;
	}
};

template<>
class  geometricComplex<2>
{
protected:
	struct K: CGAL::Exact_predicates_exact_constructions_kernel {};

	typedef CGAL::Delaunay_triangulation_3<K>           Delaunay3D;
	typedef Delaunay3D::Point                           Point;

	typedef Delaunay3D::Vertex_handle                   Vertex_handle;
	typedef Delaunay3D::Cell_handle                     Cell_handle;
	typedef K::FT                                       RealValue;

	typedef Delaunay3D::Finite_vertices_iterator        Vertex_iterator;
	typedef Delaunay3D::Finite_edges_iterator           Edge_iterator;
	typedef Delaunay3D::Finite_facets_iterator          Facet_iterator;
	typedef Delaunay3D::Finite_cells_iterator           Cell_iterator;
	typedef Delaunay3D::Facet_circulator                Facet_circulator;

public:
	void insert(){
		std::cout<<"silly2"<<std::endl;
	}


    int k=0;
    map<Vertex_handle, int> dmap;
    for(Vertex_iterator cur = Dt.finite_vertices_begin(); cur != Dt.finite_vertices_end(); ++cur){
        dmap[cur] = k;
        Simplex s;
        s.insert(k);
        C.insert(FSimplex(s,0));
        k++;
    }
    
    for(Edge_iterator cur = Dt.finite_edges_begin(); cur != Dt.finite_edges_end(); ++cur){
        Face_handle f = cur->first;
        int i = cur->second;
        Vertex_handle vs = f->vertex((i+1)%3);
        Vertex_handle vt = f->vertex((i+2)%3);
        double squared_distance = CGAL::to_double(CGAL::squared_radius(vs->point(), vt->point()));
        Simplex s;
        s.insert(dmap[vs]);
        s.insert(dmap[vt]);
        C.insert(FSimplex(s,squared_distance));
       
    }
    double *coords[3];
    
    coords[0] = new double[2];
    coords[1] = new double[2];
    coords[2] = new double[2];

    for(Face_iterator cur = Dt.finite_faces_begin(); cur != Dt.finite_faces_end(); ++cur){
       
        Vertex_handle vs = cur->vertex(0);
        Vertex_handle vt = cur->vertex(1);
        Vertex_handle vy = cur->vertex(2);

        
        coords[0][0]=CGAL::to_double(vs->point().x());
        coords[0][1]=CGAL::to_double(vs->point().y());
        coords[1][0]=CGAL::to_double(vt->point().x());
        coords[1][1]=CGAL::to_double(vt->point().y());
        coords[2][0]=CGAL::to_double(vy->point().x());
        coords[2][1]=CGAL::to_double(vy->point().y());

        
        MB mb(2,coords,coords+3);

        
        double squared_distance = mb.squared_radius();
        //double squared_distance = CGAL::to_double(CGAL::squared_radius(vs->point(), vt->point(),vy->point()));
        
      //  double squared_distance = CGAL::to_double(ms.squared_radius());
        Simplex s;
        s.insert(dmap[vs]);
        s.insert(dmap[vt]);
        s.insert(dmap[vy]);
        C.insert(FSimplex(s,squared_distance));
//        cout<<squared_distance<<endl;
       // if(squared_distance>1){
        //    cout<<vs->point()<<" | "<<vt->point()<<" | "<<vy->point()<<endl;
//       //     cout<<CGAL::squared_radius(vs->point(), vt->point(),vy->point())<<endl;
        //}
        
    }
   

    delete(coords[0]);
    delete(coords[1]);
    delete(coords[2]);


};
}
