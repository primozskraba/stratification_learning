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


    template<typename time,typename indextype>
    class Complex{
    private:

	using simplex = Simplex<indextype>;     
    	using entry = std::pair<simplex,time>;
    	using fvector = std::vector<entry>;

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
	    	  		boost::hash_combine(hashval, i*2654435761);
	    		}
            		return hashval ;
 		}
	};

	fvector data;
        int num_simplices;
        bool finalized;

	std::unordered_map<const std::reference_wrapper<const simplex>,
			 	int,
	    			simpHash,
	        		simpEqual> reverse_map;
	
    public:

    explicit Complex(const int& num_simp=0);

    Complex(std::initializer_list<std::pair<std::vector<indextype>,time>>);

    //TODO add copy/move, a complex with empty

    void insert(const simplex&, const time&);

    void finalize();
    bool verify() const;

    bool is_finalized() const; 
    bool is_defined(const simplex&) const ;
    time getTime(const int&) const ;
    time getTime(const simplex&) const ;

    int getIndex(const simplex&) const ;
    const simplex& operator [](const int&) const;

    int empty() const { return num_simplices==0;}

    int size() const {return num_simplices;}
    // should we add begin/end
    //
    void printdebug(){
	int i=0;
    	for(auto &a: data){
		if(i>=num_simplices) break;
		std::cout<<a.first<<" |  "<<a.second<<std::endl;
		i++;
	}
    }
    };




   template<typename number, typename time, typename indextype>
   strct::Map<number> boundary(Complex<time,indextype>& C){
        assert(C.is_finalized());
        assert(C.verify());
	int complex_size = C.size();
	strct::Map<number> D(complex_size,complex_size);
	for(auto i = 0; i< complex_size;++i){
		la::Vector<number> chain(complex_size);
		number coeff = -1;
		const number neg = -1;


		if(C[i].dim()>0){
			std::cout<<"here "<< C[i].dim()<<std::endl;
			for(auto j=0; j<=C[i].dim(); ++j){
		    		const Simplex<indextype> s =  C[i].erase(j);
				int indx = C.getIndex(s); 
		        	chain.pushBack(indx,coeff);
					
		                coeff=coeff*neg;	   
			}
		}
		ts::tstep t = C.getTime(i);
		chain.sort();
		D.lazyInsert(chain,t,i);
	
    	}
	D.copyTimes();

	return D;
   }


    // I/O
    template <typename indextype>
    std::ostream& operator <<(std::ostream&, const Simplex<indextype>&);


}

#include "topology.hpp"


#endif


