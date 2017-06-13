#include "geometry.h"
#include "tstepdouble.h"
#include "gtest/gtest.h"


TEST(triangulation, make){
geometricTriangulation2 T={  {0,0}, {1,0.5},{1,-0.5},{-1.4,5}};
// std::cout<<T<<std::endl;
 auto C = T.outputComplex();
C.finalize();

// std::cout<<"----------------------"<<std::endl;
//C.printdebug();
//auto A = boundary<ternary,ts::tstepdouble>(C);
//std::vector<std::pair<ts::tstepdouble,ts::tstepdouble> > bc;
//strct::Module<ternary,ts::tstepdouble> M(A);

//M.getBarcode(bc);
//for(auto i:bc){
//	std::cout<<i.first<<"  "<<i.second<<std::endl;
}//


//}


