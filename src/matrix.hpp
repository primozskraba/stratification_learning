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
    Vector<number>::Vector(const Vec& other):
            vect(other.vect),
            dimension(other.dimension) {}

    template <typename number>
    Vector<number>& Vector<number>::operator =(const Vec& other) {
        // strong guarantee (no data should be changed in case of exception)
        Vec temp(other);
        std::swap(*this, other);
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
    void Vector<number>::add(const Vec& b, const Vec& result) const {
        assert(dim() == b.dim());
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
        assert(cols() == other.rows());

        const int out_rows = rows();
        const int out_cols = B.cols();

        C.resize(out_rows, out_cols);

        // index this by rows, so it will be easier to multiply
        const Mat transposed = la::transpose(*this);

        const sparse_matrix A_rows = transposed.mat;
        const sparse_matrix B_cols = B.mat;

        sparse_matrix& C_cols = C.mat;

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
