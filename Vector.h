/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Vector.h
Latest Revision: 11-Apr-2025
Synopsis: Header and implementation file for templated Vector class and member routines
*/

#ifndef VECTOR_H
#define VECTOR_H

    #include <cstdlib>
    #include <iostream>

    // CLASS DEFINITION AND MEMBER FUNCTION DECLARATIONS
    template <typename T>
    class Vector
    {
        T *data;
        size_t N;
        bool isRow;

        T *allocate(const size_t n);
        void deallocate(T *del);

    public:
    // CONSTRUCTORS
        Vector();                               // default
        Vector(const size_t N);                 // sized
        Vector(const Vector<T>& V);             // copy
        Vector(const Vector<T>&& V);            // move
        Vector(std::initializer_list<T>& init); // initializer
        ~Vector();                              // destructor
    // IO
        void show() const;
        template <typename U>
        friend std::ostream &operator<<(std::ostream &out, Vector<U> &V);
    // ACCESSORS
        T at(const size_t n) const;
        bool isRow() const;
    // MUTATORS
        void set(const size_t n, const T &val);
        void resize(const size_t N);
        void clear();
    // OPERATORS
        Vector<T>& operator=(const Vector<T> &V);
        Vector<T>& operator=(Vector<T>&& V);
        template <typename U>
        friend Vector<U> operator+(const Vector<U>& A, const Vector<U>& B);
    };

#endif