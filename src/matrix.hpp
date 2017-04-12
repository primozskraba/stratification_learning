#include <algorithm>
#include <cassert>

#include "util.h"

namespace la {

    template <typename number>
    Matrix<number>::Matrix(const int& _row_dim, const int& col_dim):
            row_dim(_row_dim),
            mat(col_dim) {}

    template <typename number>
    Matrix<number>::Matrix(std::initializer_list<std::vector<number>> rows):
            row_dim(rows.size()),
            mat() {
        if (row_dim == 0) return;

        const int col_dim = rows.begin()->size();

        mat.resize(col_dim);
        for (auto row_ptr = rows.begin(); row_ptr != rows.end(); ++row_ptr) {
            const int row_n = row_ptr - rows.begin();
            const auto& row = *row_ptr;
            for (size_t col_n = 0; col_n < row.size(); col_n++) {
                const number& value = row[col_n];
                if (value != 0) {
                    mat[col_n].push_back({ row_n, value });
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
    number Matrix<number>::operator () (const int& row_n, const int& col_n) const {
        assert(0 <= row_n && row_n < rows() && 0 <= col_n && col_n < cols());

        const col_vector& col = mat[col_n];

        const auto cmp = [](const entry& el, const int& row_n) { return el.first < row_n; };
        const auto el = std::lower_bound(col.begin(), col.end(), row_n, cmp);

        return el->first == row_n ? el->second : number(0);
    }

    template <typename number>
    int Matrix<number>::rows() const {
        return row_dim;
    }

    template <typename number>
    int Matrix<number>::cols() const {
        return mat.size();
    }
}
