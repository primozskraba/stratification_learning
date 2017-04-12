#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>
#include <initializer_list>

#include "num.h"

namespace la {

    ////////////////////////////////////////////
    /// Sparse matrix implementation
    template <typename number>
    class Matrix {
    private:
        // type aliases
        using entry = std::pair<int,number>;
        using col_vector = std::vector<entry>;
        using matrix = std::vector<col_vector>;
        using Mat = Matrix<number>;

        int row_dim;
        matrix mat;     // stores the column vectors
    public:
        /// constructs an empty matrix with the given number of rows and columns
        Matrix(const int& rows, const int& cols);
        /// constructs a matrix from a list of (dense) row vectors
        Matrix(std::initializer_list<std::vector<number>>);

        // copy/move operations
        // copy
        Matrix(const Mat&);
        Mat& operator =(const Mat&);
        // move
        Matrix(Mat&&);
        Mat& operator =(Mat&&);

        // element access
        number operator ()(const int& row_n, const int& col_n) const;

        /// returns the number of rows
        int rows() const;
        /// returns the number of columns
        int cols() const;

    private:
        static const number& binary_search(const int& row_n, const col_vector&);
    };

    using BinaryMatrix = Matrix<num::binary>;
}

#include "matrix.hpp"

#endif
