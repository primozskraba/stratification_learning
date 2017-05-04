#include <algorithm>
#include <cassert>

#include "util.h"

namespace la {

    ////////////////////////////////////////////
    /// Sparse vector
    template <typename number>
    Vector<number>::Vector(const int& _dim):
        vect(),
        dimension(_dim) {}

    template <typename number>
    Vector<number>::Vector(std::initializer_list<number> values):
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

    template <typename number>
    Vector<number>::Vector(const int& dim, const entry& entry):
            vect(),
            dimension(dim) {
        assert(0 <= entry.first && entry.first < dim);

        if (entry.second != 0) {
            vect.push_back(entry);
        }
    }

    template <typename number>
    Vector<number>::Vector(const Vec& other):
            vect(other.vect),
            dimension(other.dimension) {}

    template <typename number>
    Vector<number>& Vector<number>::operator =(const Vec& other) {
        // strong guarantee (no data should be changed in case of exception)
        Vec temp(other);
        std::swap(*this, temp);
        return *this;
    }

    template <typename number>
    Vector<number>::Vector(Vec&& other):
        vect(std::move(other.vect)),
        dimension(other.dimension) {}

    template <typename number>
    Vector<number>& Vector<number>::operator =(Vec&& other) {
        if (this != &other) {
            std::swap(vect, other.vect);
            dimension = other.dimension;
        }
        return *this;
    }

    template <typename number>
    bool Vector<number>::operator ==(const Vec& other) const {
        if (dim() != other.dim()) { return false; }
        if (vect.size() != other.vect.size()) { return false; }
        for (size_t val_n = 0; val_n < vect.size(); val_n++) {
            if (vect[val_n] != other.vect[val_n]) { return false; }
        }
        return true;
    }

    template <typename number>
    bool Vector<number>::operator !=(const Vec& other) const {
        return !(*this == other);
    }

    template <typename number>
    void Vector<number>::make_zero() {
        vect.clear();
    }

    template <typename number>
    bool Vector<number>::is_zero() const {
        return vect.empty();
    }

    template <typename number>
    int Vector<number>::pivot_dim() const {
        return is_zero() ? -1 : vect.back().first;
    }

    template <typename number>
    number Vector<number>::pivot() const {
        assert(!is_zero());
        return vect.back().second;
    }

    template <typename number>
    number Vector<number>::operator [](const int& i) const {
        assert(0 <= i && i < dim());

        const auto cmp = [](const entry& el, const int& row_n) { return el.first < row_n; };
        const auto el_ptr = std::lower_bound(vect.begin(), vect.end(), i, cmp);

        if (el_ptr == vect.end() || el_ptr->first != i) { return 0; }
        return el_ptr->second;
    }

    template <typename number>
    number Vector<number>::operator *(const Vec& other) const {
        assert(dim() == other.dim());

        const vector& vect_a = vect;
        const vector& vect_b = other.vect;

        const int size_a = vect_a.size();
        const int size_b = vect_b.size();

        int idx_a = 0;
        int idx_b = 0;

        number result = 0;
        while (idx_a < size_a && idx_b < size_b) {
            const entry& entry_a = vect_a[idx_a];
            const entry& entry_b = vect_b[idx_b];

            if (entry_a.first == entry_b.first) {
                result += entry_a.second * entry_b.second;
            }

            if (entry_a.first <= entry_b.first) { idx_a++; }
            if (entry_b.first <= entry_a.first) { idx_b++; }
        }

        return result;
    }

    template <typename number>
    void Vector<number>::add(const Vec& b, Vec& result) const {
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
            const entry& entry_a = vect[a_idx];
            const entry& entry_b = b.vect[b_idx];

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

    template <typename number>
    void Vector<number>::add(const Vec& b, const number& k, Vec& result) const {
        assert(dim() == b.dim());

        result.dimension = dim();
        vector& result_vec = result.vect;

        if (!result_vec.empty()) { result_vec.clear(); }
        result_vec.reserve(dim() + b.dim());

        size_t a_idx = 0;
        size_t b_idx = 0;

        // merge the elements the two vectors have in common
        while (a_idx < vect.size() && b_idx < b.vect.size()) {
            const entry& entry_a = vect[a_idx];
            const entry& entry_b = b.vect[b_idx];

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

    template <typename number>
    void Vector<number>::add_multiple(const Vec& vec, const number& k) {
        Vec result(dim()); add(vec, k, result);
        std::swap(*this, result);
    }

    template <typename number>
    Vector<number> operator +(const Vector<number>& v1, const Vector<number>& v2) {
        Vector<number> result {v1.dim()};  v1.add(v2, result);
        return result;
    }

    ////////////////////////////////////////////
    /// Sparse matrix
    template <typename number>
    Matrix<number>::Matrix(const int& _row_dim, const int& col_dim):
            row_dim(_row_dim),
            mat(col_dim, vector(_row_dim)) {}

    template <typename number>
    Matrix<number>::Matrix(std::initializer_list<std::vector<number>> rows):
            row_dim(rows.size()),
            mat(rows.size() > 0 ? rows.begin()->size() : 0, vector(rows.size())) {

        if (row_dim == 0) return;

        for (auto row_ptr = rows.begin(); row_ptr != rows.end(); ++row_ptr) {
            const int row_n = row_ptr - rows.begin();
            const auto& row = *row_ptr;
            for (size_t col_n = 0; col_n < row.size(); col_n++) {
                const number& value = row[col_n];
                if (value != 0) {
                    mat[col_n].vect.push_back({ row_n, value });
                }
            }
        }
    }

    template <typename number>
    template <typename... Vecs>
    Matrix<number>::Matrix(const vector& b0, Vecs const&... vects):
            Matrix<number>(vects...) {

        mat.insert(mat.begin(), b0);
    }

    template <typename number>
    Matrix<number>::Matrix(const vector& vec):
            row_dim(vec.dim()),
            mat({ vec }) {}

    template <typename number>
    Matrix<number>::Matrix(const Mat& other):
        row_dim(other.row_dim),
        mat(other.mat) {}

    template <typename number>
    Matrix<number>::Matrix(Mat&& other):
        row_dim(other.row_dim),
        mat(std::move(other.mat)) {}

    template <typename number>
    Matrix<number>& Matrix<number>::operator =(const Mat& other) {
        // provide the strong guarantee (no data should be changed in case of exception)
        Mat temp(other);
        std::swap(*this, temp);
        return *this;
    }

    template <typename number>
    Matrix<number>& Matrix<number>::operator =(Mat&& other) {
        if (this != &other) {
            row_dim = other.row_dim;
            std::swap(mat, other.mat);
        }
        return *this;
    }

    template <typename number>
    bool Matrix<number>::operator ==(const Mat& other) const {
        if (rows() != other.rows() || cols() != other.cols()) { return false; }
        for (int col_n = 0; col_n < cols(); col_n++) {
            if (mat[col_n] != other.mat[col_n]) { return false; }
        }
        return true;
    }

    template <typename number>
    number Matrix<number>::operator () (const int& row_n, const int& col_n) const {
        assert(0 <= col_n && col_n < cols());
        return mat[col_n][row_n];
    }

    template <typename number>
    const typename Matrix<number>::vector& Matrix<number>::operator [](const int& colN) const {
        assert(0 <= colN && colN < cols());
        return mat[colN];
    }

    template <typename number>
    int Matrix<number>::rows() const {
        return row_dim;
    }

    template <typename number>
    int Matrix<number>::cols() const {
        return mat.size();
    }

    template <typename number>
    void Matrix<number>::resize(const int& rows, const int& cols) {
        *this = Mat(rows, cols);
    }

    template <typename number>
    void Matrix<number>::make_identity(const int& dim) {
        row_dim = dim;

        mat.clear();
        mat.reserve(dim);

        for (int i = 0; i < dim; i++) {
            mat.push_back(vector(dim, { i, 1 }));
        }
    }

    template <typename number>
    void Matrix<number>::transpose(Mat& transposed) const {
        transposed.resize(cols(), rows());

        for (int col_n = 0; col_n < cols(); col_n++) {
            const std::vector<entry>& col = mat[col_n].vect;
            for (size_t entry_n = 0; entry_n < col.size(); entry_n++) {
                const entry& entry = col[entry_n];
                transposed.mat[entry.first].vect.push_back({ col_n, entry.second });
            }
        }
    }

    template <typename number>
    void Matrix<number>::multiply(const Mat& B, Mat& C) const {
        assert(cols() == B.rows());

        const int out_rows = rows();
        const int out_cols = B.cols();

        C.resize(out_rows, out_cols);

        // index this by rows, so it will be easier to multiply
        const Mat transposed = la::transpose(*this);

        const SparseMatrix A_rows = transposed.mat;
        const SparseMatrix B_cols = B.mat;

        SparseMatrix& C_cols = C.mat;

        for (int col_n = 0; col_n < out_cols; col_n++) {
            vector& out_col = C_cols[col_n];
            for (int row_n = 0; row_n < out_rows; row_n++) {
                const number val = A_rows[row_n] * B_cols[col_n];
                if (val != 0) {
                    out_col.vect.push_back({ row_n, val });
                }
            }
        }
    }

    template <typename number>
    void Matrix<number>::multiply(const vector& vec, vector& result) const {
        assert(vec.dim() == cols());
        assert(rows() == result.dim());

        if (!result.is_zero()) { result.make_zero(); }

        // construct the result as a linear combination of the (column) vectors
        // in this matrix. Use only the vectors with corresponding non-zero
        // entries in 'vec'
        for (size_t entryN = 0; entryN < vec.vect.size(); entryN++) {
            const entry& entry = vec.vect[entryN];
            result.add_multiple(this->operator[](entry.first), entry.second);
        }
    }

    template <typename number>
    Matrix<number> transpose(const Matrix<number>& A) {
        Matrix<number> transposed;  A.transpose(transposed);
        return transposed;
    }

    template <typename number>
    Matrix<number> operator *(const Matrix<number>& A, const Matrix<number>& B) {
        Matrix<number> C;   A.multiply(B, C);
        return C;
    }

    template <typename number>
    void Matrix<number>::decompose(Mat& kernel, Mat& image) const {
        const int col_dim = cols();
        // copy the rows of this matrix into the image and perform gaussian elimination
        image = *this;
        kernel.resize(col_dim, 0);

        Mat op_follower;    op_follower.make_identity(col_dim);

        // use gaussian elimination, to eliminate as many columns as possible
        // the ones that cannot be eliminated are in the image
        // assume a certain structure:
        // columns with lower indexes do not have pivots in rows with higher indexes
        // once a pivot is found in row k, a new one cannot appear in rows < k

        SparseMatrix& im = image.mat;
        int kernel_cols = 0;
        for (int colN = 0; colN < col_dim; colN++) {
            vector& curr_col = im[colN];
            bool change = true;

            while (!curr_col.is_zero() && change) {
                change = false;
                const int pivot_dim = curr_col.pivot_dim();

                for (int eliminatorN = colN-1; eliminatorN >= 0; eliminatorN--) {
                    const vector& eliminator = im[eliminatorN];
                    if (eliminator.pivot_dim() == pivot_dim) {
                        const number pivot_inv = eliminator.pivot().inverse();
                        curr_col.add_multiple(eliminator, pivot_inv);
                        op_follower.mat[colN].add_multiple(op_follower[eliminatorN], pivot_inv);
                        change = true;
                        break;
                    }
                }
            }

            if (curr_col.is_zero()) {
                ++kernel_cols;
            }
        }

        // construct the kernel
        kernel.resize(col_dim, kernel_cols);
        int kernel_col = 0;
        for (int colN = 0; colN < col_dim; colN++) {
            if (image[colN].is_zero()) {
                kernel.mat[kernel_col++] = std::move(op_follower[colN]);
            }
        }

        // remove the zero vectors from the image
        const static auto is_zero_pred = [](const vector& vec) { return vec.is_zero(); };
        const auto last = std::remove_if(im.begin(), im.end(), is_zero_pred);
        im.erase(last, im.end());
    }

    template <typename number>
    void multiply(const Matrix<number>& A, const Matrix<number>& B, Matrix<number>& C) {
        A.multiply(B, C);
    }

    template <typename number>
    void multiply(const Matrix<number>& A, const Vector<number>& x, Vector<number>& y) {
        A.multiply(x, y);
    }

    template <typename number>
    std::ostream& operator <<(std::ostream& os, const Vector<number>& vec) {
        const int dim = vec.dim();
        for (int i = 0; i < dim; i++) {
            os << vec[i];
            if (i < dim - 1) {
                os << ", ";
            }
        }
        return os;
    }

    template <typename number>
    std::ostream& operator <<(std::ostream& os, const Matrix<number>& mat) {    // XXX optimize
        const int rows = mat.rows();
        const int cols = mat.cols();
        for (int row_n = 0; row_n < rows; row_n++) {
            for (int col_n = 0; col_n < cols; col_n++) {
                const number val = mat(row_n, col_n);
                os << val;
                if (col_n < cols-1) { os << ", "; }
            }
            if (row_n < rows-1) { os << '\n'; }
        }
        return os;
    }
}
