#include "topology.h"

#include "gtest/gtest.h"
#include "tstepdouble.h"

#include "tstep.h"

using namespace top;


TEST(Complex, Initialize_list){

Complex<ts::tstepdouble,int> C = { { {0},0  }, {{1},0},{{0,1},1 } };


Simplex<int> s0 = {0};
Simplex<int> s1 = {1};
Simplex<int>  s01 = {0,1};
Simplex<int>  s12  = {1,2};
C.finalize();

ASSERT_EQ(C.is_defined(s0), true);
ASSERT_EQ(C.is_defined(s1), true);
ASSERT_EQ(C.is_defined(s01), true);

ASSERT_EQ(C.is_defined(s12), false);



}



TEST(Complex, Insert){

Complex<ts::tstep,int> C;

C.insert(Simplex<int>(0),0);
C.insert(Simplex<int>(1),0);
std::vector<int> s10 = {0,1};
C.insert(s10,2);


//
//check that6 time doesnt matter
//


Simplex<int> s0 = {0};
Simplex<int> s1 = {1};
Simplex<int>  s12  = {1,2};
C.finalize();

ASSERT_EQ(C.is_defined(s0), true);
ASSERT_EQ(C.is_defined(s1), true);
ASSERT_EQ(C.is_defined(s10), true);

ASSERT_EQ(C.is_defined(s12), false);






}



TEST(Complex,finalize){
Complex<ts::tstep,int> C = { { {0},0  }, {{1},0},{{0,1},1 } };            

ASSERT_EQ(C.is_finalized(), false);
C.finalize();
ASSERT_EQ(C.is_finalized(), true);

}


TEST(Complex,duplicate){

Complex<int,int> C = { { {0},0  }, {{1},0},{{0,1},1 } };            

C.insert(Simplex<int>(0),0);

ASSERT_EQ(C.is_finalized(), false);
C.finalize();
ASSERT_EQ(C.is_finalized(), true);

ASSERT_EQ(C.size(),3);

}




TEST(Complex,Boundary){

Complex<ts::tstep,int> C = { { {0},0  }, {{1},0},{{0,1},1 } };            
C.finalize();
ASSERT_EQ(C.is_finalized(), true);
auto A = boundary<ternary,ts::tstep>(C);

//std::cout<<A<<std::endl;
std::vector<std::pair<ts::tstep,ts::tstep> > bc;
toprep::Module<ternary,ts::tstep> M(A);

M.getBarcode(bc);
for(auto i:bc){
	std::cout<<i.first<<"  "<<i.second<<std::endl;
}

//ASSERT_EQ(MatrixEntry<ternary>({1,1}),   A(2,0));
//ASSERT_EQ(MatrixEntry<ternary>({-1,1}),   A(0,0));          

//todo : add zero check



}


