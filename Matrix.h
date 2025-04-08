/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Matrix.h
Latest Revision: 22-Feb-2024
Synopsis: Header File for matrix class and routines
*/

#ifndef MATRIX_H
#define MATRIX_H

    #include <cstdlib>
    #include <iostream>

    template <typename T>
    class Matrix {
        T** data;
        size_t size;
        public:
        // Constructors
            Matrix();
            Matrix(size_t n);
            Matrix(Matrix<T> &A);
            Matrix(std::initializer_list<std::initializer_list<T>> init);
            ~Matrix();
        // IO
            void show();
            template <typename U> 
            friend std::ostream& operator<<(std::ostream& out, Matrix<U>& A);
        
        };

        template <typename T>
        Matrix<T>::Matrix() : size(3)
        {
            data = new T *[size];
            for (size_t i = 0; i < size; i++)
            {
                data[i] = new T[size];
                for (size_t j = 0; j < size; j++)
                {
                    data[i][j] = 0;
                }
            }
        }

        template <typename T>
        Matrix<T>::Matrix(size_t n) : size(n)
        {
            data = new T *[size];
            for (size_t i = 0; i < size; i++)
            {
                data[i] = new T[size];
                for (size_t j = 0; j < size; j++)
                {
                    data[i][j] = 0;
                }
            }
        }

        template <typename T>
        Matrix<T>::Matrix(Matrix<T> &A) : size(A.size)
        {
            data = new T *[size];
            for (size_t i = 0; i < size; i++)
            {
                data[i] = new T[size];
                for (size_t j = 0; j < size; j++)
                {
                    data[i][j] = A.data[i][j];
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
            size = init.size();
            data = new T* [rows];
            for (size_t i = 0; i < size; i++)
            {
                data[i] = new T[size];
            }
            size_t i = 0;
            for (const auto &row : init)
            {
                size_t j = 0;
                for (const auto &val : row)
                    data[i][j++] = val;
                ++i;
            }
        }

        template <typename T>
        Matrix<T>::~Matrix()
        {
            for (size_t i = 0; i < size; i++)
            {
                delete[] data[i];
            }
            delete[] data;
        }

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

#endif



