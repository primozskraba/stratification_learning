#ifndef __GEOMETRY_H
#define __GEOMETRY_H


#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>
#include <iterator>


#include <ctime>
#include <random>


#include <CGAL/algorithm.h>
#include <CGAL/Timer.h>
#include <CGAL/assertions.h>
#include <CGAL/point_generators_d.h>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/Epick_d.h>


#include "topology.h"
#include "Miniball.h"
#include "tstepdouble.h"

typedef double* const* PointIterator;
typedef const double* CoordIterator;
typedef Miniball:: Miniball <Miniball::CoordAccessor<PointIterator, CoordIterator> >  MB;


// CGAL doesnt like it if we wrap this in a new namespace



class geometricTriangulation2{
private:
	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
	// move typedefs to latter
	typedef CGAL::Delaunay_triangulation_2<K>  DelT;
	typedef DelT::Point                        Point;

	DelT DT;
	std::map<Point,int> point_map;
	std::map<int,Point> point_lookup;

	double *coord[3];

public:
	geometricTriangulation2();
	geometricTriangulation2(std::initializer_list<std::vector<double>>);


	void insertPoint(const std::vector<double>& );

	int vertexMap(const Point& p) {
		return point_map[p];
	}

private:
	top::Simplex<int> makeSimplex(const std::vector<Point>&) ;
	void subComplex(top::Complex<ts::tstepdouble,int>&,std::vector<Point>);
	double findTime(const std::vector<Point>&) ;

public:
	top::Complex<ts::tstepdouble,int> outputComplex();

};


class geometricTriangulation3{
private:
	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
	// move typedefs to latter
	typedef CGAL::Delaunay_triangulation_3<K>  DelT;
	typedef DelT::Point                        Point;

	DelT DT;
	std::map<Point,int> point_map;
	std::map<int,Point> point_lookup;

	double *coord[4];

public:
	geometricTriangulation3();
	geometricTriangulation3(std::initializer_list<std::vector<double>>);


	void insertPoint(const std::vector<double>& );

	int vertexMap(const Point& p) {
		return point_map[p];
	}

private:
	top::Simplex<int> makeSimplex(const std::vector<Point>&) ;
	void subComplex(top::Complex<ts::tstepdouble,int>&,std::vector<Point>);
	double findTime(const std::vector<Point>&) ;

public:
	top::Complex<ts::tstepdouble,int> outputComplex();

};



template<const int D>
class geometricTriangulationD{
private:

	typedef CGAL::Epick_d< CGAL::Dimension_tag<D> >               K;
	typedef CGAL::Delaunay_triangulation<K>                       DelT;
	typedef typename DelT::Point                       	      Point;

	DelT DT;
	std::map<Point,int> point_map;
	std::map<int,Point> point_lookup;

	double *coord[D+1];

public:
	geometricTriangulationD();
	geometricTriangulationD(std::initializer_list<std::vector<double>>);


	void insertPoint(const std::vector<double>& );

	int vertexMap(const Point& p) {
		return point_map[p];
	}

private:
	top::Simplex<int> makeSimplex(const std::vector<Point>&) ;
	void subComplex(top::Complex<ts::tstepdouble,int>&,std::vector<Point>);
	double findTime(const std::vector<Point>&) ;

public:
	top::Complex<ts::tstepdouble,int> outputComplex();

};



#include "geometry.hpp"


#endif
