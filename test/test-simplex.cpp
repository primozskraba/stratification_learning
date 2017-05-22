#include "topology.h"

#include "gtest/gtest.h"


using namespace top;

TEST(Simplex,initializer_list){
  Simplex<int> x = {0,1,2,3,4};

  for(int i = 0 ; i<5;++i){
	 ASSERT_EQ(i,x[i]);
  }

}


TEST(Simplex,initializer_list1){
  Simplex<int> x = {4,2,3,1,0};

  for(int i = 0 ; i<5;++i){
	 ASSERT_EQ(i,x[i]);
  }

}


TEST(Simplex,initializer_list2){
  Simplex<int> x = {0,0,3,4,1,2,3,4};

  for(int i = 0 ; i<5;++i){
	 ASSERT_EQ(i,x[i]);
  }

}


TEST(Simplex,lexographical_order){
  Simplex<int> x = {0,2,3,4,5};
  Simplex<int> y = {0,2,3,4,5};
  
  Simplex<int> z = {0,2,3,4};
  Simplex<int> w = {1,2,3,4,5};

  Simplex<int> t = {0,1,2,3,4};


  ASSERT_EQ(true, x==y);
  ASSERT_EQ(true, z<x);
  ASSERT_EQ(true, x<w);
  ASSERT_EQ(true, t<x);
  ASSERT_EQ(true, z<=x);
  ASSERT_EQ(true, x<=w);
  ASSERT_EQ(true, t<=x);
  ASSERT_EQ(true, x<=y);
  ASSERT_EQ(true, y<=x);


}

TEST(Simplex, erase){
  Simplex<int> x = {0,1,2,3,4,5};
  Simplex<int> dx0 = {1,2,3,4,5};
  Simplex<int> dx1 = {0,2,3,4,5};
  Simplex<int> dx2 = {0,1,3,4,5};
  Simplex<int> dx3 = {0,1,2,4,5};
  Simplex<int> dx4 = {0,1,2,3,5};
 

  
  Simplex<int> t = x.erase(0);		
  ASSERT_EQ(dx0,t);

  t = x.erase(1);		
  ASSERT_EQ(dx1,t);

  t = x.erase(2);		
  ASSERT_EQ(dx2,t);

  t = x.erase(3);		
  ASSERT_EQ(dx3,t);

  t = x.erase(4);		
  ASSERT_EQ(dx4,t);

}





