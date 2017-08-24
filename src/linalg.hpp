#include <algorithm>
#include <cassert>

#include "util.h"
#include "except.h"

namespace la {

    ////////////////////////////////////////////
    /// Sparse vector
    template <typename number,typename timeunit>
    Vector<number,timeunit>::Vector(const int& _dim):
        vect(),
        dimension(_dim) {}

    template <typename number, typename timeunit>
    Vector<number,timeunit>::Vector(std::initializer_list<number> values):
            vect(),
            dimension(values.size()) {

        const auto start = values.begin();
        for (auto val_ptr = start; val_ptr != values.end(); ++val_ptr) {
            const int val_n = val_ptr - start;
            const number& value = *val_ptr;
            if (value != 0) {
                vect.push_back({ val_n, value });
            }
        }
    }

    template <typename number, typename timeunit>
    Vector<number,timeunit>::Vector(const int& dim, const SparseEntry& entry):
            vect(),
            dimension(dim) {
        assert(0 <= entry.first && entry.first < dim);

        if (entry.second != 0) {
            vect.push_back(entry);
        }
    }

    template <typename number, typename timeunit>
    Vector<number,timeunit>::Vector(const Vec& other):
            vect(other.vect),
            dimension(other.dimension) {}

    template <typename number, typename timeunit>
    Vector<number,timeunit>& Vector<number,timeunit>::operator =(const Vec& other) {
        // strong guarantee (no data should be changed in case of exception)
        Vec temp(other);
        std::swap(*this, temp);
        return *this;
    }

    template <typename number, typename timeunit>
    Vector<number,timeunit>::Vector(Vec&& other):
        vect(std::move(other.vect)),
        dimension(other.dimension) {}

    template <typename number, typename timeunit>
    Vector<number,timeunit>& Vector<number,timeunit>::operator =(Vec&& other) {
        if (this != &other) {
            std::swap(vect, other.vect);
            dimension = other.dimension;
        }
        return *this;
    }

    template <typename number, typename timeunit>
    bool Vector<number,timeunit>::operator ==(const Vec& other) const {
        if (dim() != other.dim()) { return false; }
        if (vect.size() != other.vect.size()) { return false; }
        for (size_t val_n = 0; val_n < vect.size(); val_n++) {
            if (vect[val_n] != other.vect[val_n]) { return false; }
        }
        return true;
    }

    template <typename number, typename timeunit>
    bool Vector<number,timeunit>::operator !=(const Vec& other) const {
        return !(*this == other);
    }

    template <typename number, typename timeunit>
    void Vector<number,timeunit>::resize(const int& dim) {
        *this = Vec(dim);
    }

    template <typename number, typename timeunit>
    void Vector<number,timeunit>::makeZero() {
        vect.clear();
    }

    template <typename number, typename timeunit>
    void Vector<number,timeunit>::setZero(const int& valN) {
        assert(0 <= valN && valN < dim());

        const auto cmp = [](const SparseEntry& entry, const int& dim) { return entry.first < dim; };
        const auto entry_ptr = std::lower_bound(vect.begin(), vect.end(), valN, cmp);

        if (entry_ptr != vect.end() && entry_ptr->first == valN) {
            vect.erase(entry_ptr);
        }
    }

    template <typename number, typename timeunit>
    void Vector<number,timeunit>::setZero(const std::vector<int>& dimensions) {
        for (const int& valN : dimensions) {
            setZero(valN);
        }
    }

    template <typename number, typename timeunit>
    bool Vector<number,timeunit>::isZero() const {
        return vect.empty();
    }

    template <typename number, typename timeunit>
    void Vector<number,timeunit>::makeNegative() {
        for (int valN = 0; valN < vect.size(); ++valN) {
            vect[valN].second = -vect[valN].second;
        }
    }

    template <typename number, typename timeunit>
    int Vector<number,timeunit>::pivotDim() const {
        return isZero() ? -1 : vect.back().first;
    }

    template <typename number, typename timeunit>
    number Vector<number,timeunit>::pivot() const {
        assert(!isZero());
        return vect.back().second;
    }

    template <typename number, typename timeunit>
    number Vector<number,timeunit>::operator [](const int& i) const {
        assert(0 <= i && i < dim());

        const auto cmp = [](const SparseEntry& el, const int& row_n) { return el.first < row_n; };
        const auto el_ptr = std::lower_bound(vect.begin(), vect.end(), i, cmp);

        if (el_ptr == vect.end() || el_ptr->first != i) { return 0; }
        return el_ptr->second;
    }

    template <typename number, typename timeunit>
    number Vector<number,timeunit>::operator *(const Vec& other) const {
        assert(dim() == other.dim());

        const vector& vect_a = vect;
        const vector& vect_b = other.vect;

        const int size_a = vect_a.size();
        const int size_b = vect_b.size();

        int idx_a = 0;
        int idx_b = 0;

        number result = 0;
        while (idx_a < size_a && idx_b < size_b) {
            const SparseEntry& entry_a = vect_a[idx_a];
            const SparseEntry& entry_b = vect_b[idx_b];

            if (entry_a.first == entry_b.first) {
                result += entry_a.second * entry_b.second;
            }

            if (entry_a.first <= entry_b.first) { idx_a++; }
            if (entry_b.first <= entry_a.first) { idx_b++; }
        }

        return result;
    }

    template <typename number, typename timeunit>
    void Vector<number,timeunit>::add(const Vec& b, Vec& result) const {
        assert(dim() == b.dim());

        // dimension
        result.dimension = dim();

        // the values
        vector& result_vec = result.vect;

        if (!result_vec.empty()) { result_vec.clear(); }
        result_vec.reserve(dim() + b.dim());

        size_t a_idx = 0;
        size_t b_idx = 0;

        // merge the elements the two vectors have in common
        while (a_idx < vect.size() && b_idx < b.vect.size()) {
            const SparseEntry& entry_a = vect[a_idx];
            const SparseEntry& entry_b = b.vect[b_idx];

            if (entry_a.first == entry_b.first) {
                const number sum = entry_a.second + entry_b.second;
                if (sum != 0) {
                    result_vec.push_back({ entry_a.first, sum });
                }
                a_idx++;
                b_idx++;
            }
            else if (entry_a.first < entry_b.first) {
                result_vec.push_back(entry_a);
                a_idx++;
            }
            else {
                result_vec.push_back(entry_b);
                b_idx++;
            }
        }

        // merge the rest
        while (a_idx < vect.size()) {
            result_vec.push_back(vect[a_idx]);
            a_idx++;
        }
        while (b_idx < b.vect.size()) {
            result_vec.push_back(b.vect[b_idx]);
            b_idx++;
        }
    }

    template <typename number, typename timeunit>
    void Vector<number,timeunit>::add(const Vec& b, const number& k, Vec& result) const {
        assert(dim() == b.dim());

        result.dimension = dim();
        vector& result_vec = result.vect;

        if (!result_vec.empty()) { result_vec.clear(); }
        result_vec.reserve(dim() + b.dim());

        size_t a_idx = 0;
        size_t b_idx = 0;

        // merge the elements the two vectors have in common
        while (a_idx < vect.size() && b_idx < b.vect.size()) {
            const SparseEntry& entry_a = vect[a_idx];
            const SparseEntry& entry_b = b.vect[b_idx];

            if (entry_a.first == entry_b.first) {
                const number sum = entry_a.second + k*entry_b.second;
                if (sum != 0) {
                    result_vec.push_back({ entry_a.first, sum });
                }
                a_idx++;
                b_idx++;
            }
            else if (entry_a.first < entry_b.first) {
                result_vec.push_back(entry_a);
                a_idx++;
            }
            else {
                const number prod = k*entry_b.second;
                if (prod != 0) { result_vec.push_back({ entry_b.first, prod }); }
                b_idx++;
            }
        }

        // merge the rest
        while (a_idx < vect.size()) {
            result_vec.push_back(vect[a_idx]);
            a_idx++;
        }
        while (b_idx < b.vect.size()) {
            const number prod = k*b.vect[b_idx].second;
            if (prod != 0) { result_vec.push_back({ b.vect[b_idx].first, prod }); }
            b_idx++;
        }
    }

    template <typename number, typename timeunit>
    void Vector<number,timeunit>::addMultiple(const Vec& vec, const number& k) {
        Vec result(dim()); add(vec, k, result);
        std::swap(*this, result);
    }

    template <typename number, typename timeunit>
    Vector<number,timeunit> operator +(const Vector<number,timeunit>& v1, const Vector<number,timeunit>& v2) {
        Vector<number,timeunit> result {v1.dim()};  v1.add(v2, result);
        return result;
    }

    ////////////////////////////////////////////
    /// Matrix Entry
    template <typename number,typename timeunit>
    constexpr MatrixEntry<number,timeunit>::MatrixEntry(const number& _val, const timeunit& _tm):
        val(_val),
        tm(_tm) {}

    template <typename number,typename timeunit>
    constexpr MatrixEntry<number,timeunit>::MatrixEntry(const int& _val): val(_val) {}

    template <typename number,typename timeunit>
    constexpr bool operator ==(const MatrixEntry<number,timeunit>& entry1, const MatrixEntry<number,timeunit>& entry2) {
        return entry1.value() == entry2.value() &&
               entry1.time() == entry2.time();
    }

    template <typename number,typename timeunit>
    constexpr bool operator !=(const MatrixEntry<number,timeunit>& entry1, const MatrixEntry<number,timeunit>& entry2) {
        return !(entry1 == entry2);
    }

    template <typename number,typename timeunit>
    constexpr bool operator ==(const MatrixEntry<number,timeunit>& entry, const number& num) {
        return entry.value() == num &&
               (
                   num == 0 ||
                   entry.time() == 0
               );
    }

    template <typename number,typename timeunit>
    constexpr bool operator ==(const MatrixEntry<number,timeunit>& entry, const std::pair<number,timeunit>& pr) {
        return entry.value() == pr.first && entry.time() == pr.second;
    }

    template <typename number,typename timeunit>
    constexpr bool operator ==(const number& num, const MatrixEntry<number,timeunit>& entry) {
        return entry == num;
    }

    template <typename number,typename timeunit>
    constexpr bool operator ==(const int& num, const MatrixEntry<number,timeunit>& entry) {
        return entry == number(num);
    }

    template <typename number,typename timeunit>
    constexpr bool operator ==(const std::pair<number,timeunit>& pr, const MatrixEntry<number,timeunit>& entry) {
        return entry == pr;
    }

    template <typename number,typename timeunit>
    std::ostream& operator <<(std::ostream& os, const MatrixEntry<number,timeunit>& entry) {
        os << entry.value();
        if (entry.value() != 0) {
            os << entry.time();
        }
        return os;
    }

    ////////////////////////////////////////////
    /// Vector Interface
    template <typename number,typename timeunit>
    MatrixEntry<number,timeunit> IVector<number,timeunit>::operator [](const int& valN) const {
        const Vector<number,timeunit>& vec = getVector();
        return { vec[valN], getTime(valN) };
    }

    template <typename number,typename timeunit>
    timeunit IVector<number,timeunit>::getTime(const int& valN) const {
        return getTimeStep() - getSimplexTimes()[valN];
    }

    ////////////////////////////////////////////
    /// Vector wrapper
    template <typename number,typename timeunit>
    VectorWrapper<number,timeunit>::VectorWrapper(const Vec& _vec, const std::vector<timeunit>& _simplex_times,
            const timeunit& _vector_time):
        vec(_vec),
        simplex_times(_simplex_times),
        vector_time(_vector_time) {}


    ////////////////////////////////////////////
    /// Vector with time
    template <typename number,typename timeunit>
    TimeVector<number,timeunit>::TimeVector(const int& dim):
            vec(dim),
            simplex_times(dim, 0),
            vector_time(0) {}

    template <typename number,typename timeunit>
    TimeVector<number,timeunit>::TimeVector(const Vec& _vec, const timeunit& _vector_time):
        vec(_vec),
        simplex_times(_vec.dim(),0),
        vector_time(_vector_time) {}


    template <typename number,typename timeunit>
    TimeVector<number,timeunit>::TimeVector(const Vec& _vec, const std::vector<timeunit>& _simplex_times,
            const timeunit& _vector_time):
        vec(_vec),
        simplex_times(_simplex_times),
        vector_time(_vector_time) {}

    template <typename number,typename timeunit>
    void TimeVector<number,timeunit>::resize(const int& dim, const std::vector<timeunit>& _simplex_times,
            const timeunit& _vector_time) {
        assert(_simplex_times.size() == size_t(dim));
        vec.resize(dim);
        simplex_times = _simplex_times;
        vector_time = _vector_time;
    }

    template <typename number,typename timeunit>
    void TimeVector<number,timeunit>::makeZero() {
        vec.makeZero();
    }

    template <typename number,typename timeunit>
    bool operator ==(const IVector<number,timeunit>& a, const IVector<number,timeunit>& b) {
        return a.getVector() == b.getVector() &&
               a.getSimplexTimes() == b.getSimplexTimes() &&
               a.getTimeStep() == b.getTimeStep();
    }

    template <typename number,typename timeunit>
    bool operator !=(const IVector<number,timeunit>& a, const IVector<number,timeunit>& b) {
        return !(a == b);
    }


    ////////////////////////////////////////////
    /// Sparse matrix
    template <typename number,typename timeunit>
    Matrix<number,timeunit>::Matrix(const int& row_dim, const int& col_dim):
            mat(col_dim, Vec(row_dim)),
            row_times(row_dim, 0),
            col_times(col_dim, 0) {}

    template <typename number,typename timeunit>
    Matrix<number,timeunit>::Matrix(const int& row_dim, const int& col_dim,
                const std::vector<timeunit>& _row_times, const std::vector<timeunit>& _col_times):
            mat(col_dim, Vec(row_dim)),
            row_times(_row_times),
            col_times(_col_times) {}

    template <typename number,typename timeunit>
    Matrix<number,timeunit>::Matrix(std::initializer_list<std::vector<number>> rows):
            mat(rows.size() > 0 ? rows.begin()->size() : 0, Vec(rows.size())),
            row_times(rows.size(), 0),
            col_times(rows.size() > 0 ? rows.begin()->size() : 0, 0) {

        if (rows.size() == 0) return;

        for (auto row_ptr = rows.begin(); row_ptr != rows.end(); ++row_ptr) {
            const int row_n = row_ptr - rows.begin();
            const auto& row = *row_ptr;
            for (size_t col_n = 0; col_n < row.size(); col_n++) {
                const number& value = row[col_n];
                // set the time or check if it is correct
                if (value != 0) {
                    mat[col_n].vect.push_back({ row_n, value });
                }
            }
        }
    }

    template <typename number,typename timeunit>
    Matrix<number,timeunit>::Matrix(std::initializer_list<std::vector<number>> input,
                const std::vector<timeunit>& _row_times, const std::vector<timeunit>& _col_times):
            mat(input.size() > 0 ? input.begin()->size() : 0, Vec(input.size())),
            row_times(_row_times),
            col_times(_col_times) {

        for (auto row_ptr = input.begin(); row_ptr != input.end(); ++row_ptr) {
            const int rowN = row_ptr - input.begin();
            const auto& row = *row_ptr;
            for (size_t col_n = 0; col_n < row.size(); col_n++) {
                const number& value = row[col_n];
                // set the time or check if it is correct
                if (value != 0) {
                    mat[col_n].vect.push_back({ rowN, value });
                }
            }

            // if the row time was not calculated, set it to the time
            // of the previous row
            if (rowN > 0 && row_times[rowN].isUndefined()) {
                row_times[rowN] = row_times[rowN-1];
            }
        }

        assert(rows() == static_cast<int>(input.size()));
        assert(cols() == static_cast<int>(mat.size()));

        // check that all the sequence of time steps is correct
#ifndef NDEBUG
        for (size_t rowN = 1; rowN < row_times.size(); rowN++) {
            assert(row_times[rowN-1] <= row_times[rowN]);
        }
        for (size_t colN = 1; colN < col_times.size(); colN++) {
            assert(col_times[colN-1] <= col_times[colN]);
        }
#endif
    }

    template <typename number,typename timeunit>
    template <typename... Vecs>
    Matrix<number,timeunit>::Matrix(const Vec& first, Vecs const&... vects):
        Matrix<number,timeunit>(sizeof...(vects)+1, first, vects...) {}

    template <typename number,typename timeunit>
    Matrix<number,timeunit>::Matrix(const Mat& other):
        mat(other.mat),
        row_times(other.row_times),
        col_times(other.col_times) {}

    template <typename number,typename timeunit>
    Matrix<number,timeunit>& Matrix<number,timeunit>::operator =(const Mat& other) {
        // provide the strong guarantee (no data should be changed in case of exception)
        Mat temp(other);
        std::swap(*this, temp);
        return *this;
    }

    template <typename number,typename timeunit>
    Matrix<number,timeunit>::Matrix(Mat&& other):
        mat(std::move(other.mat)),
        row_times(std::move(other.row_times)),
        col_times(std::move(other.col_times)) {}

    template <typename number,typename timeunit>
    Matrix<number,timeunit>& Matrix<number,timeunit>::operator =(Mat&& other) {
        if (this != &other) {
            std::swap(mat, other.mat);
            std::swap(row_times, other.row_times);
            std::swap(col_times, other.col_times);
        }
        return *this;
    }

    template <typename number,typename timeunit>
    bool Matrix<number,timeunit>::operator ==(const Mat& other) const {
        if (rows() != other.rows() || cols() != other.cols()) { return false; }
        for (int col_n = 0; col_n < cols(); col_n++) {
            if (mat[col_n] != other.mat[col_n]) { return false; }
        }
        return row_times == other.row_times && col_times == other.col_times;
    }

    template <typename number,typename timeunit>
    bool Matrix<number,timeunit>::operator !=(const Mat& other) const {
        return !(*this == other);
    }

    template <typename number,typename timeunit>
    typename Matrix<number,timeunit>::Entry Matrix<number,timeunit>::operator () (const int& rowN, const int& colN) const {
        assert(0 <= rowN && rowN < rows());
        assert(0 <= colN && colN < cols());
        return { mat[colN][rowN], getEntryTime(rowN, colN) };
    }

    template <typename number,typename timeunit>
    timeunit Matrix<number,timeunit>::getColTime(const int& colN) const {
        assert(0 <= colN && colN < cols());
        return col_times[colN];
    }

    template <typename number,typename timeunit>
    timeunit Matrix<number,timeunit>::getRowTime(const int& rowN) const {
        assert(0 <= rowN && rowN < rows());
        return row_times[rowN];
    }

    template <typename number,typename timeunit>
    timeunit Matrix<number,timeunit>::getEntryTime(const int& rowN, const int& colN) const {
        return getColTime(colN) - getRowTime(rowN);
    }

    template <typename number,typename timeunit>
    VectorWrapper<number,timeunit> Matrix<number,timeunit>::operator [](const int& colN) const {
        assert(0 <= colN && colN < cols());
        return VectorWrapper<number,timeunit>(mat[colN], row_times, col_times[colN]);
    }

    template <typename number,typename timeunit>
    int Matrix<number,timeunit>::rows() const {
        return row_times.size();
    }

    template <typename number,typename timeunit>
    int Matrix<number,timeunit>::cols() const {
        return col_times.size();
    }

    template <typename number,typename timeunit>
    bool Matrix<number,timeunit>::isReducedForm() const {
        std::vector<bool> pivot_rows(rows(), false);

        const int col_dim = cols();
        for (int colN = 0; colN < col_dim; colN++) {
            const int pivot_dim = mat[colN].pivotDim();
            if (pivot_dim == -1) { continue; }  // zero column
            if (pivot_rows[pivot_dim]) { return false; }
            pivot_rows[pivot_dim] = true;
        }

        return true;
    }

    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::resize(const int& rows, const int& cols) {
        *this = Mat(rows, cols);
    }

    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::resize(const int& rows, const int& cols,
            const std::vector<timeunit>& _row_times, const std::vector<timeunit>& _col_times) {
        *this = Mat(rows, cols, _row_times, _col_times);
    }


    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::make_identity(const int& dim) {
        mat.clear();
        mat.reserve(dim);

        for (int i = 0; i < dim; i++) {
            mat.push_back(Vec(dim, { i, 1 }));
        }

        row_times.resize(dim);
        col_times.resize(dim);

        for (timeunit& step : row_times) { step = 0; }
        for (timeunit& step : col_times) { step = 0; }
    }

    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::reduce(const bool& del_zeros) {
        for (int colN = 0; colN < cols(); colN++) {
            Vec& curr_col = mat[colN];
            bool change = true;

            while (!curr_col.isZero() && change) {
                change = false;
                const int pivot_dim = curr_col.pivotDim();

                for (int eliminatorN = colN-1; eliminatorN >= 0; eliminatorN--) {
                    const Vec& eliminator = mat[eliminatorN];
                    if (eliminator.pivotDim() == pivot_dim) {
                        curr_col.addMultiple(eliminator, 1 / eliminator.pivot());
                        change = true;
                        break;
                    }
                }
            }

            if (del_zeros && curr_col.isZero()) {
                mat.erase(mat.begin() + colN);
                col_times.erase(col_times.begin() + colN);
                --colN;
            }
        }
    }

    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::multiply(const Mat& B, Mat& C) const {
        assert(cols() == B.rows());

        const int out_rows = rows();
        const int out_cols = B.cols();

        C.resize(out_rows, out_cols, row_times, B.col_times);

        // index this by rows, so it will be easier to multiply
        SparseMatrix A_rows;    transpose(A_rows);
        const SparseMatrix B_cols = B.mat;

        SparseMatrix& C_cols = C.mat;

        for (int col_n = 0; col_n < out_cols; col_n++) {
            Vec& out_col = C_cols[col_n];
            for (int row_n = 0; row_n < out_rows; row_n++) {
                const number val = A_rows[row_n] * B_cols[col_n];
                if (val != 0) {
                    out_col.vect.push_back({ row_n, val });
                }
            }
        }
    }

    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::multiply(const IVector<number,timeunit>& vec, TmVector& result) const {
        assert(vec.dim() == cols());
        assert(rows() == result.dim());

        result.resize(rows(), row_times, vec.getTimeStep());

        const Vector<number,timeunit>& internal_vec = vec.getVector();
        Vector<number,timeunit>& result_vec = result.getVector();

        // construct the result as a linear combination of the (column) vectors
        // in this matrix. Use only the vectors with corresponding non-zero
        // entries in 'vec'
        for (size_t entryN = 0; entryN < internal_vec.vect.size(); entryN++) {
            const SparseEntry& entry = internal_vec.vect[entryN];
            result_vec.addMultiple(mat[entry.first], entry.second);
        }
    }

    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::decompose(Mat& kernel, Mat& image) const {
        const int col_dim = cols();
        // copy the rows of this matrix into the image and perform gaussian elimination
        image = *this;

        Mat op_follower;    op_follower.make_identity(col_dim);

        // use gaussian elimination, to eliminate as many columns as possible
        // the ones that cannot be eliminated are in the image
        // assume a certain structure:
        // columns with lower indexes do not have pivots in rows with higher indexes
        // once a pivot is found in row k, a new one cannot appear in rows < k

        SparseMatrix& im = image.mat;
        int kernel_cols = 0;
        for (int colN = 0; colN < col_dim; colN++) {
            Vec& curr_col = im[colN];
            bool change = true;

            while (!curr_col.isZero() && change) {
                change = false;
                const int pivot_dim = curr_col.pivotDim();

                for (int eliminatorN = colN-1; eliminatorN >= 0; eliminatorN--) {
                    const Vec& eliminator = im[eliminatorN];
                    if (eliminator.pivotDim() == pivot_dim) {
                        const number pivot_inv = eliminator.pivot().inverse();
                        curr_col.addMultiple(eliminator, pivot_inv);
                        op_follower.mat[colN].addMultiple(op_follower.mat[eliminatorN], pivot_inv);
                        change = true;
                        break;
                    }
                }
            }

            if (curr_col.isZero()) {
                ++kernel_cols;
            }
        }

        // construct the kernel
        kernel.resize(col_dim, kernel_cols);
        int kernel_col = 0;
        for (int colN = 0; colN < col_dim; colN++) {
            if (image[colN].isZero()) {
                kernel.mat[kernel_col] = std::move(op_follower.mat[colN]);
                kernel.col_times[kernel_col] = image.col_times[colN];
                ++kernel_col;
            }
        }
        kernel.row_times = image.col_times;

        // remove the zero vectors from the image
        // first do the column times
        int vecN = 0;
        const auto last_times = std::remove_if(image.col_times.begin(), image.col_times.end(),
                [&](const timeunit&) { return im[vecN++].isZero(); });
        const auto last = std::remove_if(im.begin(), im.end(),
                [](const Vec& vec) { return vec.isZero(); });

        image.col_times.erase(last_times, image.col_times.end());
        im.erase(last, im.end());
    }

    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::solve(const Mat& B, Mat& X) const {
        assert(isReducedForm());
        assert(row_times == B.row_times);

        const int dimA = cols();
        const int dimB = B.cols();

        X.resize(dimA, dimB, col_times, B.col_times);

        for (int vecN = 0; vecN < dimB; vecN++) {
            // find a linear combination of vectors in A which produce b (i.e. A*alpha = b)
            // alpha then represents the current column of X

            // copy B's current vector, so you can modify it
            Vec bvec = B.mat[vecN];
            typename Vec::vector alpha_rev;   // will hold the tuples that go into alpha in reverse order

            while (!bvec.isZero()) {
                const int pivot_dim = bvec.pivotDim();
                const number& pivot = bvec.pivot();

                bool found_elim = false;
                for (int eliminatorN = 0; eliminatorN < dimA; eliminatorN++) {
                    const Vec& elim = mat[eliminatorN];
                    if (elim.pivotDim() == pivot_dim) {
                        const number factor = pivot * elim.pivot().inverse();
                        bvec.addMultiple(elim, -factor);
                        alpha_rev.push_back({ eliminatorN, factor });
                        found_elim = true;
                        break;
                    }
                }

                if (!found_elim) {
                    throw except::NotInImageSpaceException("Could not find a combination for the " + std::to_string(vecN) + "-th vector!");
                }
            }

            // reverse the vector and put it into X (this implementation relies on move semantics)
            X.mat[vecN].vect = typename Vec::vector(alpha_rev.rbegin(), alpha_rev.rend());
        }
    }

    template <typename number,typename timeunit>
    template <typename... Vecs>
    Matrix<number,timeunit>::Matrix(const int& vec_count, const Vec& vec, Vecs const&... vects):
            Mat(vec_count, vects...) {

        mat[vec_count - 1 - sizeof...(vects)] = std::move(vec);
    }

    template <typename number,typename timeunit>
    Matrix<number,timeunit>::Matrix(const int& total_vecs, const Vec& vec):
            mat(total_vecs, Vec(vec.dim())),
            row_times(vec.dim(), 0),
            col_times(total_vecs, 0) {
        mat.back() = std::move(vec);
    }

    template <typename number,typename timeunit>
    void Matrix<number,timeunit>::transpose(SparseMatrix& transposed) const {
        transposed.clear();
        transposed.resize(rows(), Vec(cols()));

        for (int col_n = 0; col_n < cols(); col_n++) {
            const std::vector<SparseEntry>& col = mat[col_n].vect;

            for (size_t entry_n = 0; entry_n < col.size(); entry_n++) {
                const SparseEntry& entry = col[entry_n];
                transposed[entry.first].vect.push_back({ col_n, entry.second });
            }
        }
    }

    template <typename number, typename timeunit>
    void Matrix<number,timeunit>::zeroColumns(const std::vector<int>& column_idxs) {
        for (const int& colN : column_idxs) {
            assert(0 <= colN && colN < cols());
            mat[colN].makeZero();
        }
    }

    template <typename number, typename timeunit>
    void Matrix<number,timeunit>::zeroRows(const std::vector<int>& row_idxs) {
        const int n_cols = cols();
        for (int colN = 0; colN < n_cols; colN++) {
            mat[colN].setZero(row_idxs);
        }
    }

    template <typename number,typename timeunit>
    Matrix<number,timeunit> operator *(const Matrix<number,timeunit>& A, const Matrix<number,timeunit>& B) {
        Matrix<number,timeunit> C;   A.multiply(B, C);
        return C;
    }

    template <typename number,typename timeunit>
    TimeVector<number,timeunit> operator *(const Matrix<number,timeunit>& A, const IVector<number,timeunit>& b) {
        TimeVector<number,timeunit> result(A.rows());
        multiply(A, b, result);
        return result;
    }

    template <typename number,typename timeunit>
    void multiply(const Matrix<number,timeunit>& A, const Matrix<number,timeunit>& B, Matrix<number,timeunit>& C) {
        A.multiply(B, C);
    }

    template <typename number,typename timeunit>
    void multiply(const Matrix<number,timeunit>& A, const IVector<number,timeunit>& x, TimeVector<number,timeunit>& y) {
        A.multiply(x, y);
    }

    template <typename number,typename timeunit>
    void solve(const Matrix<number,timeunit>& A, Matrix<number,timeunit>& X, const Matrix<number,timeunit>& B) {
        A.solve(B, X);
    }

    template <typename number,typename timeunit>
    std::ostream& operator <<(std::ostream& os, const Vector<number,timeunit>& vec) {
        const int dim = vec.dim();
        for (int i = 0; i < dim; i++) {
            os << vec[i];
            if (i < dim - 1) {
                os << ", ";
            }
        }
        return os;
    }

    template <typename number,typename timeunit>
    std::ostream& operator <<(std::ostream& os, const TimeVector<number,timeunit>& vec) {
        const int dim = vec.dim();
        os << "[ ";
        for (int valN = 0; valN < dim; valN++) {
            os << vec[valN];
            if (valN < dim-1) { os << ", "; }
        }
        os << " ]";
        return os;
    }

    template <typename number,typename timeunit>
    std::ostream& operator <<(std::ostream& os, const VectorWrapper<number,timeunit>& vec) {
        const int dim = vec.dim();
        os << "[ ";
        for (int valN = 0; valN < dim; valN++) {
            os << vec[valN];
            if (valN < dim-1) { os << ", "; }
        }
        os << " ]";
        return os;
    }

    template <typename number,typename timeunit>
    std::ostream& operator <<(std::ostream& os, const Matrix<number,timeunit>& mat) {
        const int rows = mat.rows();
        const int cols = mat.cols();
        for (int row_n = 0; row_n < rows; row_n++) {
            os << '\n';
            for (int col_n = 0; col_n < cols; col_n++) {
                const MatrixEntry<number,timeunit> val = mat(row_n, col_n);
                os << val;
                if (col_n < cols-1) { os << ",\t"; }
            }
        }
        return os;
    }
}
