


geometricTriangulation2::geometricTriangulation2():
	DT(),
	point_map(),
	point_lookup() {
		for(int i=0;i<3;++i){
			coord[i] = new double[2];
		}
	}


geometricTriangulation2::geometricTriangulation2(std::initializer_list<std::vector<double>> points):
	DT(),
	point_map(),
	point_lookup() {

	for(int i=0;i<3;++i){
		coord[i] = new double[2];
	}

	for(auto p : points ){
		DT.insert(Point(p[0],p[1]));
	}
}

void geometricTriangulation2::insertPoint(const std::vector<double> &p){
	DEBUG_ASSERT(p.size()==2);
	DT.insert(Point(p[0],p[1]));
}




top::Simplex<int> geometricTriangulation2::makeSimplex(const std::vector<Point>& pArray) {
	std::vector<int> tmp;
	for(auto x : pArray){
		tmp.push_back(point_map[x]);
	}
	return top::Simplex<int>(tmp);
}

double geometricTriangulation2::findTime(const std::vector<Point>& pArray ) {
 // first make double
  // this should be a persistena



   int j=0;
   for(auto p : pArray){
	coord[j][0]=CGAL::to_double(p.x());
	coord[j][1]=CGAL::to_double(p.y());
    	j++;
   }
	
   MB mb(2,coord,coord+j);

   return std::sqrt(mb.squared_radius()); 
}


void geometricTriangulation2::subComplex(top::Complex<ts::tstepdouble,int> &C, std::vector<Point> listPoints) {
	
	if(listPoints.size()==1){
		C.insert(top::Simplex<int>(makeSimplex(listPoints)),0);
		return;
	}
	int dim = listPoints.size();

	C.insert(top::Simplex<int>(makeSimplex(listPoints)),findTime(listPoints));  

	for(int i = 0; i<dim; ++i){
		Point p = listPoints.back();
		listPoints.pop_back();
		subComplex(C, listPoints);
		listPoints.push_back(p);
		std::swap(listPoints[i],listPoints[dim-1]);
	}

}

top::Complex<ts::tstepdouble,int> geometricTriangulation2::outputComplex(){
	
	top::Complex<ts::tstepdouble,int> C;
	
	int i=0;
	for(auto it  = DT.finite_vertices_begin(); it!=DT.finite_vertices_end();++it){
		point_map[it->point()]=i;
		point_lookup[i++]=it->point();

	}	

	for(auto it = DT.finite_faces_begin(); it!=DT.finite_faces_end();it++){
		std::vector<Point> listPoints;
        	for(int i=0;i<3;++i){
     			typename DelT::Vertex_handle vh = it->vertex(i);
             		listPoints.push_back(vh->point());
		}
		// make top simplex
		subComplex(C,listPoints);
	}

return C;
}


//
//
// 3 dimensional Delauanay
//
//




geometricTriangulation3::geometricTriangulation3():
	DT(),
	point_map(),
	point_lookup() {
		for(int i=0;i<4;++i){
			coord[i] = new double[3];
		}
	}


geometricTriangulation3::geometricTriangulation3(std::initializer_list<std::vector<double>> points):
	DT(),
	point_map(),
	point_lookup() {

	for(int i=0;i<4;++i){
		coord[i] = new double[3];
	}

	for(auto p : points ){
		DT.insert(Point(p[0],p[1],p[2]));
	}
}

void geometricTriangulation3::insertPoint(const std::vector<double> &p){
	DEBUG_ASSERT(p.size()==3);
	DT.insert(Point(p[0],p[1],p[2]));
}




top::Simplex<int> geometricTriangulation3::makeSimplex(const std::vector<Point>& pArray) {
	std::vector<int> tmp;
	for(auto x : pArray){
		tmp.push_back(point_map[x]);
	}
	return top::Simplex<int>(tmp);
}

double geometricTriangulation3::findTime(const std::vector<Point>& pArray ) {
 // first make double
  // this should be a persistena

   int j=0;
   for(auto p : pArray){
	coord[j][0]=CGAL::to_double(p.x());
	coord[j][1]=CGAL::to_double(p.y());
    	coord[j][2]=CGAL::to_double(p.z());
    	j++;
   }
	
   MB mb(3,coord,coord+j);

   return std::sqrt(mb.squared_radius()); 
}


void geometricTriangulation3::subComplex(top::Complex<ts::tstepdouble,int> &C, std::vector<Point> listPoints) {
	
	if(listPoints.size()==1){
		C.insert(top::Simplex<int>(makeSimplex(listPoints)),0);
		return;
	}
	int dim = listPoints.size();

	C.insert(top::Simplex<int>(makeSimplex(listPoints)),findTime(listPoints));  

	for(int i = 0; i<dim; ++i){
		Point p = listPoints.back();
		listPoints.pop_back();
		subComplex(C, listPoints);
		listPoints.push_back(p);
		std::swap(listPoints[i],listPoints[dim-1]);
	}

}

top::Complex<ts::tstepdouble,int> geometricTriangulation3::outputComplex(){
	
	top::Complex<ts::tstepdouble,int> C;
	
	int i=0;
	for(auto it  = DT.finite_vertices_begin(); it!=DT.finite_vertices_end();++it){
		point_map[it->point()]=i;
		point_lookup[i++]=it->point();

	}	

	for(auto it = DT.finite_cells_begin(); it!=DT.finite_cells_end();it++){
		std::vector<Point> listPoints;
        	for(int i=0;i<4;++i){
     			typename DelT::Vertex_handle vh = it->vertex(i);
             		listPoints.push_back(vh->point());
		}
		// make top simplex
		subComplex(C,listPoints);
	}

return C;
}

//
//
// D dimensional complex
//
//
//



template<const int D>
geometricTriangulationD<D>::geometricTriangulationD():
	DT(D),
	point_map(),
	point_lookup() {
		for(int i=0;i<D+1;++i){
			coord[i] = new double[D];
		}
	}



template<const int D>
geometricTriangulationD<D>::geometricTriangulationD(std::initializer_list<std::vector<double>> points):
	DT(D),
	point_map(),
	point_lookup() {

	for(int i=0;i<D+1;++i){
		coord[i] = new double[D];
	}

	for(auto p : points ){
		DT.insert(Point(p.begin(),p.end()));
	}
}


template<const int D>
void geometricTriangulationD<D>::insertPoint(const std::vector<double> &p){
	DEBUG_ASSERT(p.size()==D);

	DT.insert(Point(p.begin(),p.end()));
}





template<const int D>
top::Simplex<int> geometricTriangulationD<D>::makeSimplex(const std::vector<Point>& pArray) {
	std::vector<int> tmp;
	for(auto x : pArray){
		tmp.push_back(point_map[x]);
	}
	return top::Simplex<int>(tmp);
}


template<const int D>
double geometricTriangulationD<D>::findTime(const std::vector<Point>& pArray ) {
 // first make double
  // this should be a persistena

   int j=0;

   for(auto p : pArray){
     for(int i=0;i<D;++i){
	coord[j][i]=CGAL::to_double(p[i]);
     }
     j++;
   } 
	
   MB mb(D,coord,coord+j);

   return std::sqrt(mb.squared_radius()); 
}



template<const int D>
void geometricTriangulationD<D>::subComplex(top::Complex<ts::tstepdouble,int> &C, std::vector<Point> listPoints) {
	
	if(listPoints.size()==1){
		C.insert(top::Simplex<int>(makeSimplex(listPoints)),0);
		return;
	}
	int dim = listPoints.size();

	C.insert(top::Simplex<int>(makeSimplex(listPoints)),findTime(listPoints));  

		for(int i = 0; i<dim; ++i){
			Point p = listPoints.back();
			listPoints.pop_back();
			subComplex(C, listPoints);
			listPoints.push_back(p);
			std::swap(listPoints[i],listPoints[dim-1]);
		}

	}


	template<const int D>
	top::Complex<ts::tstepdouble,int> geometricTriangulationD<D>::outputComplex(){
		
		top::Complex<ts::tstepdouble,int> C;
		
		int i=0;
		for(auto it  = DT.finite_vertices_begin(); it!=DT.finite_vertices_end();++it){
			point_map[it->point()]=i;
		point_lookup[i++]=it->point();

	}	

	for(auto it = DT.full_cells_begin(); it!=DT.full_cells_end();it++){
		std::vector<Point> listPoints;
        	for(int i=0;i<4;++i){
     			typename DelT::Vertex_handle vh = it->vertex(i);
             		listPoints.push_back(vh->point());
		}
		// make top simplex
		subComplex(C,listPoints);
	}

return C;
}


