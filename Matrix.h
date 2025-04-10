/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Matrix.h
Latest Revision: 22-Feb-2024
Synopsis: Header and implementation file for templated matrix class and member routines
*/

#ifndef MATRIX_H
#define MATRIX_H

    #include <cstdlib>
    #include <iostream>

    // CLASS DEFINITION AND MEMBER FUNCTION DECLARATIONS
    // TODO: Upgrade to allow for non-square matrices
    template <typename T>
    class Matrix {
        T** data;
        size_t size;
        // Memory management
        T** allocate(const size_t n);
        void deallocate(T** del, const size_t n);

        public:
        // Constructors
            Matrix();
            Matrix(const size_t n);
            Matrix(const Matrix<T> &A);
            Matrix(std::initializer_list<std::initializer_list<T>> init);
            ~Matrix();
        // IO
            void show();
            template <typename U> 
            friend std::ostream& operator<<(std::ostream& out, Matrix<U>& A);
        // ACCESSORS
            T at(const size_t i, const size_t j) const;
            size_t getSize() const;
        // MUTATORS
            void set(const size_t i, const size_t j, const T& val);
            void resize(const size_t n);
            void clear();
        // OPERATORS
            template <typename U>
            friend Matrix<U> operator+(const Matrix<U>& A, const Matrix<U>& B);
            template <typename U>
            friend Matrix<U> operator*(const U& C, const Matrix<U>& A);
            template <typename U>
            friend Matrix<U> operator*(const Matrix<U>& A, const U& C);
            template <typename U>
            friend Matrix<U> operator*(const Matrix<U>& A, const Matrix<U>& B);

    };

    // MEMBER FUNCTION DEFINITIONS

    // MEMORY MANAGEMENT
    template <typename T>
    T** Matrix<T>::allocate(const size_t n)
    {
        T** newData = new T *[n];
        for (size_t i = 0; i < n; i++)
        {
            newData[i] = new T[n];
            for (size_t j = 0; j < n; j++)
            {
                newData[i][j] = (T)0;
            }
        }
        return newData;
    }

    template <typename T>
    void Matrix<T>::deallocate(T** del, const size_t n)
    {
        for (size_t i = 0; i < n; i++)
        {
            delete[] del[i];
        }
        delete[] del;
    }

    // CONSTRUCTORS
    template <typename T>
    Matrix<T>::Matrix()
    {
        this->size = 3;
        this->data = allocate(3);
    }

    template <typename T>
    Matrix<T>::Matrix(const size_t n)
    {
        this->size = n;
        this->data = allocate(n);
    }

    template <typename T>
    Matrix<T>::Matrix(const Matrix<T> &A)
    {
        this->data = allocate(A.size);
        this->size = A.size;
        for (size_t i = 0; i < A.size; i++)
        {
            for (size_t j = 0; j < A.size; j++)
            {
                this->data[i][j] = A.data[i][j];
            }
        }
    }

    template <typename T>
    Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init) {
        size_t rows = init.size();
        size_t cols = init.begin()->size();
        if (rows != cols) {
            throw std::invalid_argument("Matrix initializer list must be square!\n");
        }
        for (const auto& row: init) {
            if (row.size() != cols) {
                throw std::invalid_argument("Rows for initializer list must be of equal lengths!\n");
            }
        }
        this->size = init.size();
        this->data = allocate(this->size);
        size_t i = 0;
        for (const auto &row : init)
        {
            size_t j = 0;
            for (const auto &val : row)
                this->data[i][j++] = val;
            i++;
        }
    }

    template <typename T>
    Matrix<T>::~Matrix()
    {
        deallocate(this->data, this->size);
        this->size = 0;
    }

    // IO
    template <typename T>
    void Matrix<T>::show()
    {
        for (size_t i = 0; i < this->size; i++)
        {
            std::cout << '[';
            for (size_t j = 0; j < this->size - 1; j++)
            {
                std::cout << this->data[i][j] << ',' << ' ';
            }
            std::cout << this->data[i][size - 1];
            std::cout << ']' << '\n';
        }
    }

    template <typename U>
    std::ostream &operator<<(std::ostream &out, Matrix<U> &A)
    {
        out << '[';
        for (size_t i = 0; i < A.size; i++)
        {
            out << '[';
            for (size_t j = 0; j < A.size - 1; j++)
            {
                out << A.data[i][j] << ',' << ' ';
            }
            if (i != A.size - 1)
                out << A.data[i][A.size - 1] << ']' << ',';
            else
                out << A.data[i][A.size - 1] << ']';
        }
        out << ']' << '\n';
        return out;
    }

    // ACCESSORS
    template<typename T>
    size_t Matrix<T>::getSize() const
    {
        return this->size;
    }

    template <typename T>
    T Matrix<T>::at(const size_t i, const size_t j) const {
        if (i > this->size || j > this->size) {
            std::cerr << "ERROR: Out of range [at()]\n";
            return T(-1);
        }
        return this->data[i][j];
    }

    // MUTATORS
    template <typename T>
    void Matrix<T>::set(const size_t i, const size_t j, const T& val)
    {
        if (i > this->size || j > this->size) {
            std::cerr << "ERROR: Out of range! [set()]\n";
            return;
        }
        this->data[i][j] = val;
    }

    template <typename T>
    void Matrix<T>::resize(const size_t n) {
        size_t copy_lim = (n < this->size) ? n : this->size;

        T** newData = allocate(n);
        for (size_t i = 0; i < copy_lim; i++)
        {
            for (size_t j = 0; j < copy_lim; j++) {
                newData[i][j] = this->data[i][j];
            }
        }
        deallocate(this->data, this->size); // delete old array
        this->data = newData;               // replace with new array
        this->size = n;
    }

    template<typename T>
    void Matrix<T>::clear()
    {
        for (size_t i = 0; i < this->size; i++) {
            for (size_t j = 0; j < this->size; j++) {
                this->data[i][j] = (T)0;
            }
        }
    }

    // OPERATORS
    template <typename U>
    Matrix<U> operator+(const Matrix<U>& A, const Matrix<U>& B) 
    {
        if (A.size != B.size) {
            std::cerr << "ERROR: Matrices must be the same shape for addition! [operator+]\n";
            return A;
        }
        Matrix<U> sum(A.size);
        for (size_t i = 0; i < A.size; i++) {
            for (size_t j = 0; j < A.size; j++) {
                sum.set(i, j, A.at(i,j) + B.at(i, j));
            }
        }
        return sum;
    }

    template <typename U>
    Matrix<U> operator*(const Matrix<U>& A, const U& C) 
    {
        Matrix<U> product(A.size);
        for (size_t i = 0; i < A.size; i++) {
            for (size_t j = 0; j < A.size; j++) {
                product.set(i, j, C * A.at(i,j));
            }
        }
        return product;
    }

    template <typename U>
    Matrix<U> operator*(const U& C, const Matrix<U>& A) 
    {
        Matrix<U> product(A.size);
        for (size_t i = 0; i < A.size; i++) {
            for (size_t j = 0; j < A.size; j++) {
                product.set(i, j, C * A.at(i,j));
            }
        }
        return product;
    }

    template <typename U>
    Matrix<U> operator*(const Matrix<U>& A, const Matrix<U>& B) 
    {
        // TODO: IMPLEMENT
    }


#endif