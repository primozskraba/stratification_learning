#include <algorithm>
#include <cassert>


namespace top{

	//
	// Simpelx class
	//

	template<typename indextype>
	Simplex<indextype>::Simplex():
		vertex_list() {}

	template<typename indextype>
	Simplex<indextype>::Simplex(const indextype& vertex):
		vertex_list() {	
		vertex_list.push_back(vertex);
	}

	
	template<typename indextype>
	Simplex<indextype>::Simplex(std::initializer_list<indextype> vertices):
		vertex_list() {
		
		for(auto v : vertices){
			vertex_list.push_back(v);
		}
		
		std::sort(vertex_list.begin(),vertex_list.end());
		std::unique(vertex_list.begin(),vertex_list.end());
	}

	template<typename indextype>
	Simplex<indextype>::Simplex(std::vector<indextype> vertices):
		vertex_list() {
		
		for(auto v : vertices){
			vertex_list.push_back(v);
		}
		
		std::sort(vertex_list.begin(),vertex_list.end());
		std::unique(vertex_list.begin(),vertex_list.end());
	}


	template<typename indextype>
	Simplex<indextype>::Simplex(const Simp& other):
		vertex_list(other.vertex_list) {}

	template<typename indextype>
	Simplex<indextype>& Simplex<indextype>::operator =(const Simp& other){
 		Simp temp(other);
		std::swap(*this,temp);
		return *this;	
	}


	template<typename indextype>
	Simplex<indextype>::Simplex(Simp&& other):
		vertex_list(std::move(other.vertex_list)) {}


	template<typename indextype>
	Simplex<indextype>& Simplex<indextype>::operator =(Simp&& other) {
		if(this != &other){
			std::swap(vertex_list,other.vertex_list);
		}
		return *this;
	}
	

	template<typename indextype>
	bool Simplex<indextype>::operator ==(const Simp& other) const {
		return this->vertex_list==other.vertex_list;
	} 
	
	template<typename indextype>
	bool Simplex<indextype>::operator !=(const Simp& other) const {
		return this->vertex_list!=other.vertex_list;
	}

	template<typename indextype>
	bool Simplex<indextype>::operator <(const Simp& other) const {
		const int dim = this->dim();
		if(other.dim()!=dim){
			return dim<other.dim();
		}
		for(int i = 0; i<=dim;++i){
			if((*this)[i]!=other[i]){
			    return (*this)[i]<other[i];
			}

		}
		return false;
	}


	// there is some code repitition here but it should be more
	// efficient than calling both equality and <
	template<typename indextype>
	bool Simplex<indextype>::operator <=(const Simp& other) const {
		const int dim  = this->dim();
		if(other.dim()!=dim){
			return dim<other.dim();
		}
		for(int i = 0; i<=dim;++i){
			if((*this)[i]!=other[i]){
			    return (*this)[i]<other[i];
			}
		}
		return true;
	}


	template<typename indextype>
	indextype Simplex<indextype>::operator [](const int& index) const {
		assert(0 <= index && index<= this->dim());

		return vertex_list[index];
	}



 


	template<typename indextype>
	Simplex<indextype> Simplex<indextype>::erase(const int& index) const {
		Simp temp(*this);
		assert(0 <= index && index<= this->dim());
		
		temp.vertex_list.erase(temp.vertex_list.begin()+index);
		return temp;
	}

	template<typename indextype>
	void Simplex<indextype>::insert(const indextype& vertex) {
		vertex_list.push_back(vertex);
		std::sort(vertex_list.begin(),vertex_list.end());
		std::unique(vertex_list.begin(),vertex_list.end());
	
	}


    // I/O
    template <typename indextype> 
    std::ostream& operator <<(std::ostream& os, const Simplex<indextype>& simp){
        const int dim = simp.dim();
        for (int i = 0; i <= dim; ++i) {
            os << simp[i];
            if (i < dim ) {
                os << ", ";
            }
        }
        return os;
    }


// change this to 0 but reserve the size
   template<typename timeunit,typename indextype>
   Complex<timeunit,indextype>::Complex(const int& num_simp):
 	data(),
 	num_simplices(num_simp),
	finalized(false),
	reverse_map(){}	
	 
   
	
   template<typename timeunit,typename indextype>
	Complex<timeunit,indextype>::Complex(std::initializer_list<std::pair<std::vector<indextype>,timeunit>> simplices):
        data(),
 	num_simplices(),
	finalized(false),
	reverse_map(){
	    for (auto simp_ptr = simplices.begin(); simp_ptr != simplices.end(); ++simp_ptr) {
		simplex S;
		for (auto index_ptr = simp_ptr->first.begin(); index_ptr != simp_ptr->first.end(); ++index_ptr){
			S.insert(*index_ptr);   
                }
		this->insert(S,simp_ptr->second);
            }
        }
 


    

	//TODO add copy/move, a complex with empty 

   template<typename timeunit,typename indextype>   
   void Complex<timeunit,indextype>::insert(const Simplex<indextype>& simp,const timeunit& t){
   	data.push_back(entry(simp,t));
	num_simplices++;
   }

   template<typename timeunit,typename indextype>
   void Complex<timeunit,indextype>::finalize(){
   	struct filt_order
	{
	    inline bool operator() (const entry& a, const entry& b)
	    {
		    if(a.second==b.second)
		    	return a.first<b.first;
		    else
	    		return a.second<b.second;    
	    }
	};

	std::sort(data.begin(),data.end(),filt_order());


	// unique does not change size of vector - so we need to 
	// double check - also it may be smart to swap ... this is just a test that will be removed
	// as memory is our main issue...maybe
	auto end_it = std::unique(data.begin(),data.end());
	num_simplices =  end_it - data.begin();

	for(int i=0;i<num_simplices;++i){
		reverse_map.insert(std::make_pair(std::cref(data[i].first),i));

	}

   	finalized=true;
   }
   
   template<typename timeunit, typename indextype>
   bool Complex<timeunit,indextype>::is_finalized() const{
   	return finalized;
   }
   
 
   template<typename timeunit, typename indextype>
   bool Complex<timeunit,indextype>::is_defined(const Simplex<indextype>& simp) const{
   	return !(reverse_map.find(simp)==reverse_map.end());
   }
   
   template<typename timeunit,typename indextype> 
   bool Complex<timeunit,indextype>::verify() const{
 	if(!is_finalized()) 
		return false;

   	for(int i = 0;i<num_simplices;++i){
		int dim = data[i].first.dim();
		if(dim>0){
			for(int j=0;j<=dim;++j){
				if(!is_defined(data[i].first.erase(j)))
					return false;
			}
		}
	}
	return true;
   }


   template<typename timeunit,typename indextype> 
   timeunit Complex<timeunit,indextype>::getTime(const int& index) const {
   	return data.at(index).second;
   }


   template<typename timeunit,typename indextype> 
   timeunit Complex<timeunit,indextype>::getTime(const Simplex<indextype>& simp) const {
   	return data[reverse_map.at(simp)].second;
   }

   template<typename timeunit,typename indextype> 
   int Complex<timeunit,indextype>::getIndex(const Simplex<indextype>& simp) const {
   	return reverse_map.at(simp);
   }

 
   template<typename timeunit, typename indextype>
   const Simplex<indextype>& Complex<timeunit,indextype>::operator [](const int& index) const{ 
	 assert(0 <= index && index< this->num_simplices);
	return data[index].first;
   } 	

 template<typename number, typename timeunit, typename indextype>
   strct::Map<number,timeunit> boundary(Complex<timeunit,indextype>& C){
        assert(C.is_finalized());
        assert(C.verify());
	int complex_size = C.size();
	strct::Map<number,timeunit> D(complex_size,complex_size);
	for(auto i = 0; i< complex_size;++i){
		la::Vector<number,timeunit> chain(complex_size);
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
		timeunit t = C.getTime(i);
		chain.sort();
		D.lazyInsert(chain,t,i);
	
    	}
	D.copyTimes();

	return D;
   }

 template<typename number, typename timeunit, typename indextype>
    strct::Map<number,timeunit> relativeBoundary(Complex<timeunit,indextype>& C_A,Complex<timeunit,indextype>& C_B){
	assert(C_A.is_finalized());
        assert(C_A.verify());
	assert(C_B.is_finalized());
        assert(C_B.verify());

	int complex_size = C_A.size() + C_B.size();

	strct::Map<number,timeunit> D(complex_size,complex_size);
	int ind_A=0;
	int ind_B=0;

	for(auto i = 0; i< complex_size;++i){
		la::Vector<number,timeunit> chain(complex_size);
		number coeff = -1;
		const number neg = -1;
		timeunit t; 
		
		if(C_A.getTime(ind_A)<=C_B.getTime(ind_B)){
			if(C_A[ind_A].dim()>0){
				for(auto j=0; j<=C_A[ind_A].dim(); ++j){
		    			const Simplex<indextype> s =  C_A[i].erase(j);
					int indx = C_A.getIndex(s); 
		        		chain.pushBack(indx,coeff);
		                	coeff=coeff*neg;	   
				}
			}
			t = C_A.getTime(ind_A++);
		}
		else{
			int indx = C_B.getIndex(C_B[ind_B]); 
		        chain.pushBack(indx,coeff*neg);
			t = C_B.getTime(ind_B++);
		}
		
		
		chain.sort();
		D.lazyInsert(chain,t,i);
	
    	}
	D.copyTimes();

	return D;


	    
  }
		    




}
