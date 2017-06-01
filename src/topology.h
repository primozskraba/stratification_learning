#ifndef _COMPLEX_H
#define _COMPLEX_H


#include <vector>
#include <map>
#include <initializer_list>
#include <iostream>
#include <functional>

#include "linalg.h"
namespace top{
    
    // simplex class - mainly for templating
    //	template <typename time> Complex;
    
    template<typename indextype>
    class Simplex{
        //		friend class Complex<time>;
    private:
        using simplex = std::vector<indextype>;
        using Simp = Simplex<indextype>;
        simplex vertex_list;
        
    public:
        Simplex();
        explicit Simplex(const indextype&);
        
        Simplex(std::initializer_list<indextype>);
        Simplex(std::vector<indextype>);
        
        
        // COPY/MOVE operations
        // copy
        Simplex(const Simp&);
        Simplex& operator =(const Simp&);
        // move
        Simplex(Simp&&);
        Simp& operator =(Simp&&);
        
        bool operator ==(const Simp&) const;
        bool operator !=(const Simp&) const;
        bool operator <(const Simp&) const;
        bool operator <=(const Simp&) const;

        
        /// returns true if this is a zero simplex 
       bool is_empty() const { return vertex_list.is_empty(); }
        
        int dim() const { return vertex_list.size()-1; }
        
        /// access the i-th vertex
        indextype operator [](const int&) const;
        
        Simp erase(const int&) const ;
        void insert(const indextype&);
        
       

    };
    
    template<typename time,typename indextype>   
    class Complex{
    private:
	using simplex = Simplex<indextype>;     
    	using entry = std::pair<simplex,time>;
    	using fvector = std::vector<entry>;
    
        // compare with unordered_map	
  	std::map<const std::reference_wrapper<simplex> ,int> reverse_map;

    	fvector data;
    	int num_simplices;
       bool finalized;	
    public:
    
   	explicit Complex(const int& num_simp=0); 
	
	Complex(std::initializer_list<std::pair<std::vector<indextype>,time>>);

	//TODO add copy/move, a complex with empty 
	
	void insert(const simplex&, const time&);

	void finalize();
	void verify() const;

	bool is_finalized() const {return finalized;}
	bool is_defined(const simplex&) const;
	time& getTime(const int&) const;
	time& getTime(const simplex&) const;

	int& getIndex(const simplex&) const;
 	const simplex& operator [](const int&) const;
 
	int empty() const { return num_simplices==0;} 
        
        int size() const {return num_simplices;}
	// should we add begin/end	
    };



   
   template<typename time, typename indextype, typename number>
   la::Matrix<number>& boundary(const Complex<time,indextype>& C){
	assert(C.is_finalized());
#ifdef _VERIFY_CONSTRUCTION
	assert(C.verify());
#endif
	int complex_size = C.size();
	la::Matrix<number> D(complex_size,complex_size);
	for(auto i = 0; i< complex_size;++i){
		la::Vector<number> chain(complex_size);
		if(C[i].dim()>0){
			for(auto j=0; j<=C[i].dim(); ++j){
		    	   //chain.insert(C.getIndex(C[i].erase(j)), );	
			}
		}
		else {
			// insert empty chain
		}
		
	}
   }


    // I/O
    template <typename indextype>
    std::ostream& operator <<(std::ostream&, const Simplex<indextype>&);
    
    
}

#include "topology.hpp"
#endif


