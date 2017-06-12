#ifndef _COMPLEX_H
#define _COMPLEX_H


#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <iostream>
#include <functional>
#include <boost/functional/hash.hpp>


#include "struct.h"
namespace top{

    // simplex class - mainly for templating
    //  template <typename time> Complex;

    template<typename indextype>
    class Simplex{
        //      friend class Complex<time>
    private:
        using simplex = std::vector<indextype>;
        using Simp = Simplex<indextype>;
        simplex vertex_list;

    public:
        Simplex();
        explicit Simplex(const indextype&);

        Simplex(std::initializer_list<indextype>);
        Simplex(std::vector<indextype>);
       
        // can only look at simplex as const  
	// it is only  meant to check - one 
	// should never construct a simplex using iterators
	// but rather insert or constructers
        using iterator=typename simplex::const_iterator;
	
	iterator begin() const {return vertex_list.cbegin();}
	iterator end() const {return vertex_list.cend();}

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


    template<typename timeunit,typename indextype>
    class Complex{
    private:

	using simplex = Simplex<indextype>;     
    	using entry = std::pair<simplex,timeunit>;
    	using fvector = std::vector<entry>;
	fvector data;
        int num_simplices;
        bool finalized;

	//
	// unordered map structures
	//
	struct simpEqual {
    		bool operator()(const simplex& lhs,
		               const simplex& rhs) const{
            		return lhs == rhs;
        	}
	};

	struct simpHash{
 		std::size_t operator()(const simplex &s) const{
			std::size_t hashval = 0 ;
	    		for(auto& i: s){
				//magic hashing to reduce collisions
	    	  		boost::hash_combine(hashval, i*2654435761);
	    		}
            		return hashval ;
 		}
	};



	std::unordered_map<const std::reference_wrapper<const simplex>,
			 	int,
	    			simpHash,
	        		simpEqual> reverse_map;
	
    public:

    explicit Complex(const int& num_simp=0);

    Complex(std::initializer_list<std::pair<std::vector<indextype>,timeunit>>);

    //TODO add copy/move, a complex with empty

    void insert(const simplex&, const timeunit&);

    void finalize();
    bool verify() const;

    bool is_finalized() const; 
    bool is_defined(const simplex&) const ;
    timeunit getTime(const int&) const ;
    timeunit getTime(const simplex&) const ;

    int getIndex(const simplex&) const ;
    const simplex& operator [](const int&) const;

    int empty() const { return num_simplices==0;}

    int size() const {return num_simplices;}

};




   template<typename number, typename timeunit, typename indextype>
   strct::Map<number,timeunit> boundary(Complex<timeunit,indextype>& C);
       

    // I/O
    template <typename indextype>
    std::ostream& operator <<(std::ostream&, const Simplex<indextype>&);


    template<typename number, typename timeunit, typename indextype>
    strct::Map<number,timeunit> relativeBoundary(Complex<timeunit,indextype>& C_A,Complex<timeunit,indextype>& C_B);



    
}

#include "topology.hpp"


#endif


