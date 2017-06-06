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

        /// resizes the vector
        void resize(const int& dim);
        /// makes the vector [0,0,...,0]
        void makeZero();
        /// returns true if this is a zero vector
        bool isZero() const;

        int dim() const { return dimension; }
        /// returns the index of the highest non-zero dimension
        int pivotDim() const;
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
        void addMultiple(const Vec& vec, const number& k);
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
        tstep tm {0};
    public:
        /// default constructor, sets time and value
        constexpr MatrixEntry(const number& val, const tstep& time=0);
        /// constructor which takes value as int and sets time to 0
        constexpr MatrixEntry(const int& val);

        constexpr const number& value() const { return val; }
        constexpr const tstep& time() const { return tm; }
    };

    // equality operator
    template <typename number>
    constexpr bool operator ==(const MatrixEntry<number>&, const MatrixEntry<number>&);
    template <typename number>
    constexpr bool operator !=(const MatrixEntry<number>&, const MatrixEntry<number>&);
    // equality operator, assumes the time of the second operand is 0
    template <typename number>
    constexpr bool operator ==(const MatrixEntry<number>&, const number&);
    template <typename number>
    constexpr bool operator ==(const MatrixEntry<number>&, const std::pair<number,tstep>&);
    // equality operator, assumes the time of the first operand is 0
    template <typename number>
    constexpr bool operator ==(const number&, const MatrixEntry<number>&);
    template <typename number>
    constexpr bool operator ==(const int&, const MatrixEntry<number>&);
    template <typename number>
    constexpr bool operator ==(const std::pair<number,tstep>&, const MatrixEntry<number>&);

    // write to stream operator
    template <typename number>
    std::ostream& operator <<(std::ostream&, const MatrixEntry<number>&);

    // type definitions
    using BinaryEntry = MatrixEntry<binary>;
    using TernaryEntry = MatrixEntry<ternary>;



    //=======================================================
    // TIME VECTOR
    //=======================================================

    ////////////////////////////////////////////
    /// A base vector class which defines the public
    /// interface which all vectors use
    template <typename number>
    class IVector {
    protected:
        using Vec = Vector<number>;

    public:
        virtual ~IVector() {}

        /// get element
        MatrixEntry<number> operator [](const int&) const;
        tstep getTime(const int&) const;

        /// returns the vectors dimension
        int dim() const { return getVector().dim(); }
        /// returns true if the vector contains only zeros
        bool isZero() const { return getVector().isZero(); }
        /// returns the dimension of the pivot
        int pivotDim() const { return getVector().pivotDim(); }

        virtual const Vec& getVector() const = 0;
        virtual const std::vector<tstep>& getSimplexTimes() const = 0;
        virtual const tstep& getTimeStep() const = 0;
    };


    ////////////////////////////////////////////
    /// A wrapper for a vector, holds a reference to
    /// the vector itself and the entry times
    template <typename number>
    class VectorWrapper : public IVector<number> {
    private:
        using Vec = typename IVector<number>::Vec;

        const Vec& vec;
        const std::vector<tstep>& simplex_times;
        const tstep& vector_time;
    public:
        VectorWrapper(const Vec&, const std::vector<tstep>&, const tstep&);

        const Vec& getVector() const { return vec; }
        const std::vector<tstep>& getSimplexTimes() const { return simplex_times; }
        const tstep& getTimeStep() const { return vector_time; }
    };

    using BinaryVectorWrapper = VectorWrapper<binary>;
    using TernaryVectorWrapper = VectorWrapper<ternary>;


    ////////////////////////////////////////////
    /// A vector which also encodes the time step
    /// of each of its siplices.
    template <typename number>
    class TimeVector : public IVector<number> {
    private:
        using Vec = typename IVector<number>::Vec;

        Vec vec;
        std::vector<tstep> simplex_times;
        tstep vector_time;
    public:
        TimeVector(const int& dim);
        TimeVector(const Vec&, const std::vector<tstep>&, const tstep&);
        // TODO add constructor to convert a VectorWrapper to TimeVector

        /// resizes the vector and sets the time steps
        void resize(const int& dim, const std::vector<tstep>&, const tstep&);
        /// overrides the vector to contain only zeros
        void makeZero();

        const Vec& getVector() const { return vec; }
        Vec& getVector() { return vec; }

        const std::vector<tstep>& getSimplexTimes() const { return simplex_times; }
        const tstep& getTimeStep() const { return vector_time; }
    };

    using BinaryTimeVector = TimeVector<binary>;
    using TernaryTimeVector = TimeVector<ternary>;


    /// vector comparison
    template <typename number>
    bool operator ==(const IVector<number>&, const IVector<number>&);




    //=======================================================
    // MATRIX
    //=======================================================

    ////////////////////////////////////////////
    /// Sparse matrix implementation
    template <typename number>
    class Matrix {
    private:
        // type aliases
        using Vec = Vector<number>;
        using Mat = Matrix<number>;

        using SparseEntry = typename Vec::SparseEntry;
        using SparseMatrix = std::vector<Vec>;

        SparseMatrix mat;     // stores the column vectors
        std::vector<tstep> row_times;
        std::vector<tstep> col_times;

    public:
        // types which are output by the matrix
        using Entry = MatrixEntry<number>;
        using TmVector = TimeVector<number>;

        /// constructs an empty matrix with the given number of rows and columns
        /// all the rows and columns are given time 0
        explicit Matrix(const int& rows=0, const int& cols=0);
        /// constructs an empty matrix with the given number of rows and columns
        /// also sets the row and col times
        explicit Matrix(const int& rows, const int& cols,
                const std::vector<tstep>& row_times, const std::vector<tstep>& col_times);

        /// constructs a matrix from a list of (dense) row vectors
        /// all times are assumed to be 0
        Matrix(std::initializer_list<std::vector<number>>);
        /// constructs a matrix from a list of (dense) row vectors
        /// the row and column times are specified as the second and third argument
        Matrix(std::initializer_list<std::vector<number>> vals, const std::vector<tstep>& row_times,
                const std::vector<tstep>& col_times);
        /// constructs a matrix from a list of column vectors
        /// all times are set to 0
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
        bool operator !=(const Mat&) const;

        // ELEMENT ACCESS

        /// access element at position row_n, col_n
        Entry operator ()(const int& rowN, const int& colN) const;
        /// returns the time of the vector
        tstep getColTime(const int&) const;
        /// returns the time step of the simplex
        tstep getRowTime(const int&) const;
        /// returns the time associated with the (rowN,colN)-th entry
        tstep getEntryTime(const int& rowN, const int& colN) const;
        /// access the k-th column vector
        VectorWrapper<number> operator [](const int& colN) const;

        // PROPERTIES

        /// returns the number of rows
        int rows() const;
        /// returns the number of columns
        int cols() const;
        /// check if the matrix has any elements
        int empty() const { return rows() == 0 || cols() == 0; }
        /// checks if the columns of the vector are either linearly independent or zero
        bool isReducedForm() const;

        // RESHAPING

        /// changes the dimensions of the matrix, clears the matrix in the process
        void resize(const int& rows, const int& cols);
        void resize(const int& rows, const int& cols,
               const std::vector<tstep>& row_times, const std::vector<tstep>& col_times);
        /// reshapes the matrix and puts ones on the diagonal, sets all times to 0
        void make_identity(const int& dim);

        // OPERATIONS

        /// sets all the linearly dependent vectors to 0
        void reduce();

        /// mulitplication
        void multiply(const Mat&, Mat&) const;
        void multiply(const IVector<number>&, TmVector&) const;

        /// decompose into the kernel and image
        void decompose(Mat& kernel, Mat& image) const;

        /// solves the system A*X = B
        void solve(const Mat& B, Mat& X) const;

    private:
        // helper constructors
        template <typename... Vecs>
        Matrix(const int& vec_count, const Vec&, Vecs const&...);
        Matrix(const int& vec_count, const Vec&);

        void transpose(SparseMatrix&) const;
    };

    template <typename number>
    Matrix<number> operator *(const Matrix<number>&, const Matrix<number>&);

    template <typename number>
    TimeVector<number> operator *(const Matrix<number>&, const IVector<number>&);

    template <typename number>
    void multiply(const Matrix<number>&, const Matrix<number>&, Matrix<number>&);

    template <typename number>
    void multiply(const Matrix<number>&, const IVector<number>&, TimeVector<number>&);

    /// solves the system A*X = B
    template <typename number>
    void solve(const Matrix<number>& A, Matrix<number>& X, const Matrix<number>& B);

    // I/O
    template <typename number>
    std::ostream& operator <<(std::ostream&, const Vector<number>&);
    template <typename number>
    std::ostream& operator <<(std::ostream&, const IVector<number>&);
    template <typename number>
    std::ostream& operator <<(std::ostream&, const Matrix<number>&);

    using BinaryMatrix = Matrix<binary>;
    using TernaryMatrix = Matrix<ternary>;
}

#include "linalg.hpp"

#endif
