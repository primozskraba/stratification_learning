#ifndef _MATRIX_H
#define _MATRIX_H

#include <vector>
#include <initializer_list>
#include <iostream>

#include "num.h"
#include "tstep.h"

namespace la {

    using namespace num;
    using namespace ts;

    // forward declarations
    template <typename number> class Matrix;

    ////////////////////////////////////////////
    /// Sparse vector implementation
    template <typename number>
    class Vector {
        friend class Matrix<number>;
    private:
        using SparseEntry = std::pair<int,number>;
        using Vec = Vector<number>;

        using vector = std::vector<SparseEntry>;

        vector vect;
        int dimension;
    public:
        explicit Vector(const int& dim);

        Vector(std::initializer_list<number>);
        /// constructs a vector with a single non-zero entry
        Vector(const int& dim, const SparseEntry&);

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
    /// Single entry returned from a matrix
    template <typename number>
    class MatrixEntry {
    private:
        number val;
        tstep tm;
    public:
        constexpr MatrixEntry(const number& value, const tstep& time=0);

        constexpr const number& value() const { return val; }
        constexpr const tstep& time() const { return tm; }
    };

    // equality operator
    template <typename number>
    constexpr bool operator ==(const MatrixEntry<number>&, const MatrixEntry<number>&);
    // equality operator, assumes the time of the second operand is 0
    template <typename number>
    constexpr bool operator ==(const MatrixEntry<number>&, const number&);
    // equality operator, assumes the time of the first operand is 0
    template <typename number>
    constexpr bool operator ==(const number&, const MatrixEntry<number>&);
    template <typename number>
    constexpr bool operator ==(const int&, const MatrixEntry<number>&);

    // write to stream operator
    template <typename number>
    std::ostream& operator <<(std::ostream&, const MatrixEntry<number>&);


    ////////////////////////////////////////////
    /// Sparse matrix implementation
    template <typename number>
    class Matrix {
    private:
        // type aliases
        using Vec = Vector<number>;
        using Mat = Matrix<number>;
        using Entry = MatrixEntry<number>;

        using SparseEntry = typename Vec::SparseEntry;
        using SparseMatrix = std::vector<Vec>;

        SparseMatrix mat;     // stores the column vectors
        std::vector<tstep> row_times;
        std::vector<tstep> col_times;

    public:
        /// constructs an empty matrix with the given number of rows and columns
        /// all the rows and columns are given time 0
        explicit Matrix(const int& rows=0, const int& cols=0);
        /// constructs an empty matrix with the given number of rows and columns
        /// also sets the row and col times
        explicit Matrix(const int& rows, const int& cols,
                std::vector<tstep>& row_times, std::vector<tstep>& col_times);

        /// constructs a matrix from a list of (dense) row vectors
        /// all times are assumed to be 0
        Matrix(std::initializer_list<std::vector<number>>);
        /// constructs a matrix from a list of (dense) row vectors, where each entry also specifies
        /// the difference between the column and row times
        /// the first row and column are assumed to have time 0
        Matrix(std::initializer_list<std::vector<std::pair<number,tstep>>>);
        /// constructs a matrix from a list of column vectors
        template <typename... Vecs> Matrix(const Vec&, Vecs const&...);

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
        Entry operator ()(const int& rowN, const int& colN) const;
        /// returns the time associated with the (rowN,colN)-th entry
        tstep getEntryTime(const int& rowN, const int& colN) const;
        /// access the k-th column vector
        const Vec& operator [](const int& colN) const;  // TODO do we want this???

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
        void resize(const int& rows, const int& cols, std::vector<tstep>& row_times, std::vector<tstep>& col_times);
        /// reshapes the matrix and puts ones on the diagonal, sets all times to 0
        void make_identity(const int& dim);

        // OPERATIONS

        /// transpose
        void transpose(Mat&) const;

        /// mulitplication
        void multiply(const Mat&, Mat&) const;
        void multiply(const Vec&, Vec&) const;

        /// decompose into the kernel and image
        void decompose(Mat& kernel, Mat& image) const;

    private:
        // helper constructors
        template <typename... Vecs>
        Matrix(const int& vec_count, const Vec&, Vecs const&...);
        Matrix(const int& vec_count, const Vec&);
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
