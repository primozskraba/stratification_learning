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
    //  template <typename time> Complex;

    template<typename indextype>
    class Simplex{
        //      friend class Complex<time>;
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
        struct mp_cmp 
	{
		inline bool operator()(const simplex& a,const simplex& b) const { 
			return a<b;
		}	
	};	
	std::map<const std::reference_wrapper<const simplex> ,int,mp_cmp> reverse_map;

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

    bool is_finalized() const; 
    bool is_defined(const simplex&) ;
    time getTime(const int&) ;
    time getTime(const simplex&) ;

    int getIndex(const simplex&) ;
    const simplex& operator [](const int&) const;

    int empty() const { return num_simplices==0;}

    int size() const {return num_simplices;}
    // should we add begin/end
    };




   template<typename number, typename time, typename indextype>
   la::Matrix<number>& boundary(Complex<time,indextype>& C){
    assert(C.is_finalized());
#ifdef _VERIFY_CONSTRUCTION
    assert(C.verify());
#endif
	int complex_size = C.size();
	la::Matrix<number> D(complex_size,complex_size);
	for(auto i = 0; i< complex_size;++i){
		la::Vector<number> chain(complex_size);
		number coeff = -1;
		const number neg = -1;


		if(C[i].dim()>0){
			for(auto j=0; j<=C[i].dim(); ++j){
		    		const Simplex<indextype> s =  C[i].erase(j);
				int indx = C.getIndex(s); 
		        	chain.pushBack(indx,coeff);
		        coeff=coeff*neg;	   
			}
		}
		
		la::TimeVector<number> fchain(chain,C.getTime(i));

		D.insert(fchain,i);
	
    }
   }


    // I/O
    template <typename indextype>
    std::ostream& operator <<(std::ostream&, const Simplex<indextype>&);


}

#include "topology.hpp"
#endif


