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
        explicit Vector(const int& dim);

        Vector(std::initializer_list<number>);
        /// constructs a vector with a single non-zero entry
        Vector(const int& dim, const entry&);

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
        /// returns true if this is a zero vector
        bool is_zero() const;

        int dim() const { return dimension; }
        /// returns the index of the highest non-zero dimension
        int pivot_dim() const;
        /// returns the entry in the last non-zero dimension (assumes such an element exists)
        number pivot() const;

        /// access the element at the specified dimension
        number operator [](const int&) const;

        /// dot product
        number operator *(const Vec&) const;

        /// summation
        /// z <- this + vec
        void add(const Vec&, Vec&) const;
        /// z <- this + k*vec
        void add(const Vec&, const number& k, Vec&) const;
        /// this <- this + k*vec
        void add_multiple(const Vec& vec, const number& k);
    };

    // non-member functions
    template <typename number>
    Vector<number> operator +(const Vector<number>&, const Vector<number>&);

    // aliases
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

        using SparseMatrix = std::vector<vector>;
        using Mat = Matrix<number>;

        int row_dim;
        SparseMatrix mat;     // stores the column vectors

    public:
        /// constructs an empty matrix with the given number of rows and columns
        explicit Matrix(const int& rows=0, const int& cols=0);
        /// constructs a matrix from a list of (dense) row vectors
        Matrix(std::initializer_list<std::vector<number>>);
        /// constructs a matrix from a list of column vectors
        template <typename... Vecs>
        Matrix(const vector&, Vecs const&...);
        /// constructs a matrix from a single column vector
        Matrix(const vector&);

        // COPY/MOVE operations
        // copy
        Matrix(const Mat&);
        Mat& operator =(const Mat&);
        // move
        Matrix(Mat&&);
        Mat& operator =(Mat&&);

        // comparison

        /// equality operator
        bool operator ==(const Mat&) const;

        // ELEMENT ACCESS

        /// access element at position row_n, col_n
        number operator ()(const int& rowN, const int& colN) const;
        /// access the k-th column vector
        const vector& operator [](const int& colN) const;

        // PROPERTIES

        /// returns the number of rows
        int rows() const;
        /// returns the number of columns
        int cols() const;
        /// check if the matrix has any elements
        int empty() const { return rows() == 0 || cols() == 0; }

        // RESHAPING

        /// changes the dimensions of the matrix, clears the matrix in the process
        void resize(const int& rows, const int& cols);
        /// reshapes the matrix and puts ones on the diagonal
        void make_identity(const int& dim);

        // OPERATIONS

        /// transpose
        void transpose(Mat&) const;

        /// mulitplication
        void multiply(const Mat&, Mat&) const;
        void multiply(const vector&, vector&) const;

        /// decompose into the kernel and image
        void decompose(Mat& kernel, Mat& image) const;
    };

    template <typename number>
    Matrix<number> transpose(const Matrix<number>&);

    template <typename number>
    Matrix<number> operator *(const Matrix<number>&, const Matrix<number>&);

    template <typename number>
    void multiply(const Matrix<number>&, const Matrix<number>&, Matrix<number>&);

    template <typename number>
    void multiply(const Matrix<number>&, const Vector<number>&, Vector<number>&);

    // I/O
    template <typename number>
    std::ostream& operator <<(std::ostream&, const Vector<number>&);
    template <typename number>
    std::ostream& operator <<(std::ostream&, const Matrix<number>&);

    using BinaryMatrix = Matrix<binary>;
    using TernaryMatrix = Matrix<ternary>;
}

#include "linalg.hpp"

#endif
