/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Matrix.h
Latest Revision: 10-Apr-2025
Synopsis: Header and implementation file for templated matrix class and member routines
*/

#ifndef MATRIX_H
#define MATRIX_H

    #include "Vector.h" // dependency

    // CLASS DEFINITION AND MEMBER FUNCTION DECLARATIONS
    template <typename T>
    class Matrix {
        T** data;
        size_t I, J;
        // Memory management
        T** allocate(const size_t I, const size_t J);
        void deallocate(T** del, const size_t I);

        public:
        // CONSTRUCTORS
            Matrix();                               // default
            Matrix(const size_t N);                 // square sized
            Matrix(const size_t I, const size_t J); // rectangular sized
            Matrix(const Matrix<T> &A);             // copy
            Matrix(Matrix<T>&& A);                  // move
            Matrix(const std::initializer_list<std::initializer_list<T>> init);
            ~Matrix();                              // destructor
        // IO
            void show() const;
            template <typename U> 
            friend std::ostream& operator<<(std::ostream& out, Matrix<U>& A);
        // ACCESSORS
            T at(const size_t i, const size_t j) const;
            Vector<T> getRow(const size_t i) const;
            Vector<T> getCol(const size_t j) const;
            size_t rows() const;
            size_t cols() const;
        // MUTATORS
            void set(const size_t i, const size_t j, const T& val);
            void resize(const size_t I, const size_t J);
            void clear();
        // OPERATORS
            Matrix<T>& operator=(const Matrix<T>& A);
            Matrix<T>& operator=(Matrix<T>&& A);
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
    T** Matrix<T>::allocate(const size_t I, const size_t J)
    {
        if (I == 0 || J == 0)
        {
            std::cerr << "WARNING: No memory allocated for empty matrix.\n";
            return nullptr;
        }
        T** newData = new T*[I];
        for (size_t i = 0; i < I; i++)
        {
            newData[i] = new T[J];
            for (size_t j = 0; j < J; j++)
            {
                newData[i][j] = (T)0;
            }
        }
        return newData;
    }

    template <typename T>
    void Matrix<T>::deallocate(T** del, const size_t I)
    {
        if (!del) {return;} // safeguard
        // free up memory
        for (size_t i = 0; i < I; i++)
        {
            delete[] del[i];
        }
        delete[] del;

        if (del == this->data) {this->data = nullptr;} 
    }

    // CONSTRUCTORS
    template <typename T>
    Matrix<T>::Matrix()
    {
        this->I = 0;
        this->J = 0;
        this->data = allocate(0, 0);
    }

    template <typename T>
    Matrix<T>::Matrix(const size_t N)
    {
        this->I = N;
        this->J = N;
        this->data = allocate(N, N);
    }

    template<typename T>
    Matrix<T>::Matrix(const size_t I, const size_t J) 
    {
        this->I = I;
        this->J = J;
        this->data = allocate(I, J);
    }

    template <typename T>
    Matrix<T>::Matrix(const Matrix<T> &A)
    {
        this->data = allocate(A.I, A.J);
        this->I = A.I;
        this->J = A.J;
        for (size_t i = 0; i < A.I; i++)
        {
            for (size_t j = 0; j < A.J; j++)
            {
                this->data[i][j] = A.data[i][j];
            }
        }
    }

    template <typename U>
    Matrix<U>::Matrix(Matrix<U>&& A)
    {
        // Steal the data
        this->data = A.data;
        this->I = A.I;
        this->J = A.J;

        // Disconnect A ownership
        A.data = nullptr;
    }

    template <typename T>
    Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init) {
        for (const auto& row: init) {
            if (row.size() != init.begin()->size()) {
                std::cerr << "ERROR: Rows of initializer list must be of equal lengths!\n";
                Matrix();
            }
        }

        this->I = init.size();
        this->J = init.begin()->size();
        this->data = allocate(this->I, this->J);
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
        deallocate(this->data, this->I);
    }

    // IO
    template <typename T>
    void Matrix<T>::show() const
    {
        if (!this->data) { 
            // empty matrix
            std::cout << "[[ ]]" << std::endl; 
            return;
        } 

        for (size_t i = 0; i < this->I; i++)
        {
            std::cout << '[';
            for (size_t j = 0; j < this->J - 1; j++)
            {
                std::cout << this->data[i][j] << ',' << ' ';
            }
            std::cout << this->data[i][this->J - 1];
            std::cout << ']' << std::endl;
        }
    }

    template <typename U>
    std::ostream &operator<<(std::ostream &out, Matrix<U> &A)
    {
        if (!A.data) { 
            // empty matrix
            out << "[[ ]]"; 
            return out;
        } 
        out << '[';
        for (size_t i = 0; i < A.I; i++)
        {
            out << '[';
            for (size_t j = 0; j < A.J - 1; j++)
            {
                out << A.data[i][j] << ',' << ' ';
            }
            if (i != A.J - 1)
                out << A.data[i][A.J - 1] << ']' << ',';
            else
                out << A.data[i][A.J - 1] << ']';
        }
        out << ']';
        return out;
    }

    // ACCESSORS
    template<typename T>
    size_t Matrix<T>::rows() const
    {
        return this->I;
    }
    template<typename T>
    size_t Matrix<T>::cols() const
    {
        return this->J;
    }

    template <typename T>
    T Matrix<T>::at(const size_t i, const size_t j) const {
        if (i > (this->I - 1) || j > (this->J - 1)) {
            std::cerr << "ERROR: Out of range [at()]\n";
            return (T)0;
        }
        return this->data[i][j];
    }

    template <typename T>
    Vector<T> Matrix<T>::getRow(const size_t i) const
    {
        if (i > (this->I - 1)) {
            std::cerr << "ERROR: Out of range! [getRow()]\n";
            return Vector<T>(0, true);
        }
        Vector<T> row(this->J, true);
        for (size_t j = 0; j < this->J; j++) {
            row[j] = data[i][j];
        }
        return row;
    }

    template <typename T>
    Vector<T> Matrix<T>::getCol(const size_t j) const
    {
        if (j > (this->J - 1)) {
            std::cerr << "ERROR: Out of range! [getCol()]\n";
            return Vector<T>(0, false);
        }
        Vector<T> col(this->I, false);
        for (size_t i = 0; i < this->I; i++) {
            col[i] = data[i][j];
        }
        return col;
    }

    // MUTATORS
    template <typename T>
    void Matrix<T>::set(const size_t i, const size_t j, const T& val)
    {
        if (i > (this->I - 1) || j > (this->J - 1)) {
            std::cerr << "ERROR: Out of range! [set()]\n";
            return;
        }
        this->data[i][j] = val;
    }

    template <typename T>
    void Matrix<T>::resize(const size_t I, const size_t J) {
        size_t copy_lim_rows = (I < this->I) ? I : this->I;
        size_t copy_lim_cols = (J < this->J) ? J : this->J;

        T** newData = allocate(I, J);
        for (size_t i = 0; i < copy_lim_rows; i++)
        {
            for (size_t j = 0; j < copy_lim_cols; j++) {
                newData[i][j] = this->data[i][j];
            }
        }
        deallocate(this->data, this->I); // delete old array
        this->I = I;
        this->J = J;
        this->data = newData;            // replace with new array
    }

    template<typename T>
    void Matrix<T>::clear()
    {
        for (size_t i = 0; i < this->I; i++) {
            for (size_t j = 0; j < this->J; j++) {
                this->data[i][j] = (T)0;
            }
        }
    }

    // OPERATORS
    template <typename U>
    Matrix<U>& Matrix<U>::operator=(const Matrix<U>& A)
    {
        std::cout << "[COPY ASSIGNMENT]\n";
        if (this == &A) {
            return *this;
        }

        deallocate(this->data, this->I);
        this->I = A.I;
        this->J = A.J;
        this->data = allocate(this->I, this->J);

        for (size_t i = 0; i < this->I; i++) {
            for (size_t j = 0; j < this->J; j++) {
                this->data[i][j] = A.data[i][j];
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<U>& Matrix<U>::operator=(Matrix<U>&& A) 
    {
        std::cout << "[MOVE ASSIGNMENT]\n";
        if (this == &A) {
            return *this;
        }

        // clean up
        deallocate(this->data, this->I);

        // steal data from A
        this->data = A.data;
        this->I = A.I;
        this->J = A.J;

        // disconnect A from ownership
        A.data = nullptr;
        A.I = 0;
        A.J = 0;

        return *this;
    }

    template <typename U>
    Matrix<U> operator+(const Matrix<U>& A, const Matrix<U>& B) 
    {
        if ((A.I != B.I) || (A.J != B.J)) {
            std::cerr << "ERROR: Matrices must be the same shape for addition! [operator+]\n";
            return A;
        }
        Matrix<U> sum(A.I, A.J);
        for (size_t i = 0; i < A.I; i++) {
            for (size_t j = 0; j < A.J; j++) {
                sum.set(i, j, A.at(i,j) + B.at(i, j));
            }
        }
        return sum;
    }

    template <typename U>
    Matrix<U> operator*(const Matrix<U>& A, const U& C) 
    {
        Matrix<U> product(A.I, A.J);
        for (size_t i = 0; i < A.I; i++) {
            for (size_t j = 0; j < A.J; j++) {
                product.set(i, j, C * A.at(i,j));
            }
        }
        return product;
    }

    template <typename U>
    Matrix<U> operator*(const U& C, const Matrix<U>& A) 
    {
        Matrix<U> product(A.I, A.J);
        for (size_t i = 0; i < A.I; i++) {
            for (size_t j = 0; j < A.J; j++) {
                product.set(i, j, C * A.at(i,j));
            }
        }
        return product;
    }

    template <typename U>
    Matrix<U> operator*(const Matrix<U>& A, const Matrix<U>& B) 
    {
        if (A.J != B.I) {
            std::cerr << "Invalid dimensions for matrix multiplication! [operator*]\n";
            return A;
        }

        // TODO: IMPLEMENT
    }


#endif