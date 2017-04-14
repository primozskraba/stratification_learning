#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>
#include <initializer_list>
#include <iostream>

#include "num.h"

namespace la {

    using namespace num;

    // forward declarations
    template <typename number> class Matrix;

    ////////////////////////////////////////////
    /// Sparse vector implementation
    template <typename number>
    class Vector {
        friend class Matrix<number>;
    private:
        using entry = std::pair<int,number>;
        using vector = std::vector<entry>;
        using Vec = Vector<number>;

        vector vect;
        int dimension;
    public:
        Vector(const int& dim);
        Vector(std::initializer_list<number>);

        // COPY/MOVE operations
        // copy
        Vector(const Vec&);
        Vec& operator =(const Vec&);
        // move
        Vector(Vec&&);
        Vec& operator =(Vec&&);

        bool operator ==(const Vec&) const;
        bool operator !=(const Vec&) const;

        /// makes the vector [0,0,...,0]
        void make_zero();

        int dim() const { return dimension; }

        /// access the element at the specified dimension
        number operator [](const int&) const;

        /// dot product
        number operator *(const Vec&) const;

        /// summation
        void add(const Vec&, Vec&) const;
    };

    using BinaryVector = Vector<binary>;
    using TernaryVector = Vector<ternary>;

    ////////////////////////////////////////////
    /// Sparse matrix implementation
    template <typename number>
    class Matrix {
    private:
        // type aliases
        using vector = Vector<number>;
        using entry = typename vector::entry;

        using sparse_matrix = std::vector<vector>;
        using Mat = Matrix<number>;

        int row_dim;
        sparse_matrix mat;     // stores the column vectors
    public:
        /// constructs an empty matrix with the given number of rows and columns
        Matrix(const int& rows=0, const int& cols=0);
        /// constructs a matrix from a list of (dense) row vectors
        Matrix(std::initializer_list<std::vector<number>>);

        // COPY/MOVE operations
        // copy
        Matrix(const Mat&);
        Mat& operator =(const Mat&);
        // move
        Matrix(Mat&&);
        Mat& operator =(Mat&&);

        // equality operator
        bool operator ==(const Mat&) const;

        /// access element at position row_n, col_n
        number operator ()(const int& row_n, const int& col_n) const;

        /// returns the number of rows
        int rows() const;
        /// returns the number of columns
        int cols() const;
        /// check if the matrix has any elements
        int empty() const { return rows() == 0 && cols() == 0; }
        /// changes the dimensions of the matrix, clears the matrix in the process
        void resize(const int& rows, const int& cols);

        // OPERATIONS

        /// transpose
        void transpose(Mat&) const;

        /// mulitplication
        void multiply(const Mat& other, Mat& result) const;
    };

    template <typename number>
    Matrix<number> transpose(const Matrix<number>&);

    template <typename number>
    Matrix<number> operator *(const Matrix<number>&, const Matrix<number>&);

    // I/O
    template <typename number>
    std::ostream& operator <<(std::ostream&, const Matrix<number>&);

    using BinaryMatrix = Matrix<binary>;
    using TernaryMatrix = Matrix<ternary>;
}

#include "matrix.hpp"

#endif
