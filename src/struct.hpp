#include <cassert>

namespace strct {

    template <typename number,typename timeunit>
    int Space<number,timeunit>::vector_count() const {
        return Mat::cols();
    }

    template <typename number,typename timeunit>
    Space<number,timeunit> Map<number,timeunit>::operator ()(const Space<number,timeunit>& space) const {
        Space<number,timeunit> result; apply(space, result);
        return result;
    }

 
    //template <typename number>
    //Map<number>::Map(int nrows,int ncols){
   // }

   // template <typename number>
    //void Map<number>::insert(la::TimeVector<number>& tvect){
     //     lazyInsert()
   // }

   // template <typename number>
    //bool Map<number>::verify(){
  // 	return true;
   // }
    





    template <typename number,typename timeunit>
    TimeVector<number,timeunit> Map<number,timeunit>::operator ()(const IVector<number,timeunit>& vec) const {
        la::TimeVector<number,timeunit> result(Mat::rows());  apply(vec, result);
        return result;
    }

    template <typename number,typename timeunit>
    void Map<number,timeunit>::decompose(Space<number,timeunit>& kernel, Space<number,timeunit>& image) const {
        Matrix<number,timeunit>::decompose(kernel, image);
    }

    template <typename number, typename timeunit>
    void Map<number,timeunit>::kernel(Space<number,timeunit>& kernel) const {
        Matrix<number,timeunit>::kernel(kernel);
    }


    template <typename number,typename timeunit>
    void Map<number,timeunit>::apply(const Space<number,timeunit>& space, Space<number,timeunit>& result) const {
        multiply(*this, space, result);
    }

    template <typename number,typename timeunit>
    void Map<number,timeunit>::apply(const IVector<number,timeunit>& vec, TimeVector<number,timeunit>& result) const {
        multiply(*this, vec, result);
    }

    template <typename number,typename timeunit>
    void Map<number,timeunit>::getDomainImgTimeDiffs(std::vector<std::pair<timeunit,timeunit>>& intervals) const {
        assert(isReducedForm());

        const int rows = Matrix<number,timeunit>::rows();
        const int cols = Matrix<number,timeunit>::cols();

        if (!intervals.empty()) { intervals.clear(); }

        intervals.reserve(rows);
        for (int rowN = 0; rowN < rows; rowN++) {
            intervals.push_back({ Matrix<number,timeunit>::getRowTime(rowN), tstep::INF });
        }

        for (int colN = 0; colN < cols; colN++) {
            const int killedN = Matrix<number,timeunit>::operator [](colN).pivotDim();
            intervals[killedN].second = Matrix<number,timeunit>::getColTime(colN);
        }
    }

    template <typename number, typename timeunit>
    Map<number,timeunit> Map<number,timeunit>::operator +(const Map<number,timeunit>& other) const {
        assert(Mat::row_times == other.row_times);

        Map<number,timeunit> result(Mat::rows(), Mat::cols() + other.cols());
        int i = 0, j = 0;
        while (i < Mat::cols() && j < other.cols()) {
            const timeunit timeA = Mat::getColTime(i);
            const timeunit timeB = other.getColTime(j);
            if (timeA <= timeB) {
                const int colN = i + j;
                result.mat[colN] = Mat::mat[i];
                result.col_times[colN] = timeA;
                ++i;
            }
            if (timeB <= timeA) {
                const int colN = i + j;
                result.mat[colN] = other.mat[j];
                result.col_times[colN] = timeB;
                ++j;
            }
        }
        while (i < Mat::cols()) {
            const int colN = i + j;
            result.mat[colN] = Mat::mat[i];
            result.col_times[colN] = Mat::getColTime(i);
            ++i;
        }
        while (j < other.cols()) {
            const int colN = i + j;
            result.mat[colN] = other.mat[j];
            result.col_times[colN] = other.getColTime(j);
            ++j;
        }

        // copy the row times (they should be the same for all the maps)
        result.row_times = Mat::row_times;

        result.reduce(true);

        return result;
    }

    template <typename number, typename timeunit>
    Map<number,timeunit> Map<number,timeunit>::operator -(const Map<number,timeunit>& other) const {
        assert(Mat::row_times == other.row_times);

        Map<number,timeunit> result(Mat::rows(), Mat::cols() + other.cols());
        int i = 0, j = 0;
        while (i < Mat::cols() && j < other.cols()) {
            const timeunit timeA = Mat::getColTime(i);
            const timeunit timeB = other.getColTime(j);
            if (timeA <= timeB) {
                const int colN = i + j;
                result.mat[colN] = Mat::mat[i];
                result.col_times[colN] = Mat::timeA;
                ++i;
            }
            if (timeB <= timeA) {
                const int colN = i + j;
                result.mat[colN] = other.mat[j];
                result.mat[colN].makeNegative();
                result.col_times[colN] = timeB;
                ++j;
            }
        }
        while (i < Mat::cols()) {
            const int colN = i + j;
            result.mat[colN] = Mat::mat[i];
            result.col_times[colN] = Mat::getColTime(i);
            ++i;
        }
        while (j < other.cols()) {
            const int colN = i + j;
            result.mat[colN] = other.mat[j];
            result.mat[colN].makeNegative();
            result.col_times[colN] = other.getColTime(j);
            ++j;
        }

        // copy the row times (they should be the same for all the maps)
        result.row_times = Mat::row_times;

        // reduce the result
        result.reduce(true);

        return result;
    }

    template <typename number,typename timeunit>
    void Map<number,timeunit>::find(const Space<number,timeunit>& domain, Map<number,timeunit>& map, const Space<number,timeunit>& image) {
        solve(domain, map, image);
    }

    template <typename number,typename timeunit>
    std::ostream& operator <<(std::ostream& os, const Map<number,timeunit>& map) {
        return os << static_cast<Matrix<number,timeunit>>(map);
    }

    template <typename number,typename timeunit>
    Module<number,timeunit>::Module(const Map<number,timeunit>& boundry) {
        boundry.decompose(generators, relations);
        Map<number,timeunit>::find(generators, map, relations);
    }

    template <typename number,typename timeunit>
    void Module<number,timeunit>::getBarcode(std::vector<std::pair<timeunit,timeunit>>& barcode) const {
        map.getDomainImgTimeDiffs(barcode);
    }

    template <typename number, typename timeunit>
    void relativeHomology(const Module<number,timeunit>& m1, const Map<number,timeunit>& f,
            const Module<number,timeunit>& m2, const Map<number,timeunit>& g,
            const Module<number,timeunit>& m3, Module<number,timeunit>& result) {
        // 1) compute the generators
        (f - g).kernel(result.generators);
    }
}
