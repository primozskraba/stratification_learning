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
				std::cerr<<i<<"  "<<(*this)[i]<<"  "<<other[i]<<std::endl;
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
   template<typename time,typename indextype>
   Complex<time,indextype>::Complex(const int& num_simp):
	reverse_map(),
 	data(),
 	num_simplices(),
	finalized(false){}	
	 
   
	
   template<typename time,typename indextype>
	Complex<time,indextype>::Complex(std::initializer_list<std::pair<std::vector<indextype>,time>> simplices):
   	reverse_map(),
 	data(),
 	num_simplices(),
	finalized(false){
	    for (auto simp_ptr = simplices.begin(); simp_ptr != simplices.end(); ++simp_ptr) {
		simplex S;
		for (auto index_ptr = simp_ptr->first.begin(); index_ptr != simp_ptr->first.end(); ++index_ptr){
			S.insert(*index_ptr);   
                }
		this->insert(S,simp_ptr->second);
            }
        }
 


    

	//TODO add copy/move, a complex with empty 

   template<typename time,typename indextype>   
   void Complex<time,indextype>::insert(const Simplex<indextype>& simp,const time& t){
   	data.push_back(entry(simp,t));
	num_simplices++;
   }

   template<typename time,typename indextype>
   void Complex<time,indextype>::finalize(){
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
	std::unique(data.begin(),data.end());
	for(int i=0;i<num_simplices;++i){
		reverse_map.insert(std::make_pair(data[i].first,i));

	}
   	finalized=true;
   }

   template<typename time, typename indextype>
   bool Complex<time,indextype>::is_defined(const Simplex<indextype>& simp) const{
   	return !(reverse_map.find(simp)==reverse_map.end());
   }
   
   template<typename time,typename indextype> 
   void Complex<time,indextype>::verify() const{
   	if(is_finalized()) 
		return false;

   	for(int i = 0;i<num_simplices;++i){
		int dim = data[i].first.dim();
		if(dim>0){
			for(int j=0;j<=dim;++j){
				if(!defined(data[i].first.erase(j)))
					return false;
			}
		}
	}
	return true;
   }


   template<typename time,typename indextype> 
   time& Complex<time,indextype>::getTime(const int& index) const{
   	return data[index].second;
   }


   template<typename time,typename indextype> 
   time& Complex<time,indextype>::getTime(const Simplex<indextype>& simp) const{
   	return data[reverse_map[simp]].second;
   }

   template<typename time,typename indextype> 
   int& Complex<time,indextype>::getIndex(const Simplex<indextype>& simp) const{
   	return reverse_map[simp];
   }

  template<typename time, typename indextype>
   const Simplex<indextype>& Complex<time,indextype>::operator [](const int& index) const{ 
	 assert(0 <= index && index< this->num_simplices);
	return data[index].first;
   }
 
	


}
