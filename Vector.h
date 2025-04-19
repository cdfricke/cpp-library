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
        Vector();                                       // default
        Vector(const size_t N);                         // sized
        Vector(const size_t N, bool isRow);             // sized with row control
        Vector(const Vector<T>& V);                     // copy
        Vector(const Vector<T>&& V);                    // move
        Vector(const std::initializer_list<T>& init);   // initializer
        ~Vector();                                      // destructor
    // IO
        void show() const;
        template <typename U>
        friend std::ostream &operator<<(std::ostream &out, Vector<U> &V);
    // ACCESSORS
        T at(const size_t n) const;
        size_t size() const;
        bool row() const;
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

    // MEMBER FUNCTION DEFINITIONS

    // MEMORY MANAGEMENT
    template <typename T>
    T* Vector<T>::allocate(const size_t N) 
    {
        if (N == 0) {
            std::cout << "WARNING: No memory allocated for empty vector.\n";
            return nullptr;
        }
        T* newData = new T[N];
        for (size_t i = 0; i < N; i++)
        {
            newData[i] = (T)0;
        }
        return newData;
    }

    template <typename T>
    void Vector<T>::deallocate(T* del) 
    {
        if (!del) {return;} // safeguard
        // free up memory
        delete[] del;

        if (del == this->data) {this->data = nullptr;}
    }

    // CONSTRUCTORS
    template <typename T>
    Vector<T>::Vector()
    {
        this->N = 0;
        this->isRow = false;
        this->data = allocate(0);
    }

    template <typename T>
    Vector<T>::Vector(const size_t N)
    {
        this->N = N;
        this->isRow = false;
        this->data = allocate(N);
    }

    template <typename T>
    Vector<T>::Vector(const size_t N, bool isRow)
    {
        this->N = N;
        this->isRow = isRow;
        this->data = allocate(N);
    }

    template <typename T>
    Vector<T>::Vector(const Vector<T>& V)
    {
        this->N = V.N;
        this->isRow = V.isRow;
        this->data = allocate(V.N);
        for (size_t i = 0; i < this->N; i++)
        {
            this->data[i] = V.data[i];
        }
    }

    template <typename T>
    Vector<T>::Vector(const Vector<T>&& V)
    {
        // Steal the data
        this->N = V.N;
        this->isRow = V.isRow;
        this->data = V.data;

        // Disconnect V ownership
        V.data = nullptr;
    }

    template <typename T>
    Vector<T>::Vector(const std::initializer_list<T>& init)
    {
        this->N = init.size();
        this->isRow = false;
        this->data = allocate(init.size());

        size_t i = 0;
        for (const auto& val : init)
        {
            this->data[i] = val;
            i++;
        }
    }

    template <typename T>
    Vector<T>::~Vector()
    {
        deallocate(this->data);
    }

    // IO
    template <typename T>
    void Vector<T>::show() const
    {
        if (!this->data) {
            // empty vector
            std::cout << "[ ]" << std::endl;
            return;
        }
        std::cout << '[';
        for (size_t i = 0; i < this->N - 1; i++)
        {
            std::cout << this->data[i] << ',' << ' ';
        }
        std::cout << this->data[this->N - 1];
        std::cout << ']' << std::endl;
    }

    template <typename U>
    std::ostream& operator<<(std::ostream& out, Vector<U>& V)
    {
        if (!V.data) {
            // empty vector
            out << "[ ]";
            return out;
        }
        out << '[';
        for (size_t i = 0; i < V.N - 1; i++)
        {
            out << V.data[i] << ',' << ' ';
        }
        out << V.data[V.N - 1];
        out << ']';
        return out;
    }

    // ACCESSORS
    template <typename T>
    T Vector<T>::at(const size_t n) const {
        if (n > this->N - 1) {
            std::cerr << "ERROR: Out of range [at()]\n";
            return (T)0;
        }
        return this->data[n];
    }

    template <typename T>
    size_t Vector<T>::size() const {return this->N;}

    template <typename T>
    bool Vector<T>::row() const {return this->isRow;}

    // MUTATORS
    template <typename T>
    void Vector<T>::set(const size_t n, const T &val)
    {
        if (n > this->N - 1) {
            std::cerr << "ERROR: Out of range! [set()]\n";
            return;
        }
        this->data[n] = val;
    }
    
    template <typename T>
    void Vector<T>::resize(const size_t N)
    {
        size_t copy_lim = (N < this->N) ? N : this->N;

        T* newData = allocate(N);
        for (size_t i = 0; i < copy_lim; i++) {
            newData[i] = this->data[i];
        }
        deallocate(this->data);
        this->N = N;
        this->data = newData;
    }
        
    template <typename T>
    void Vector<T>::clear()
    {
        for (size_t i = 0; i < this->N; i++) {
            this->data[i] = (T)0;
        }
    }

    // OPERATORS
    template<typename U>
    Vector<U>& Vector<U>::operator=(const Vector<U> &V)
    {

    }

    template <typename U>
    Vector<U>& Vector<U>::operator=(Vector<U>&& V)
    {

    }
        
    template <typename U>
    Vector<U> operator+(const Vector<U>& A, const Vector<U>& B)
    {
        
    }

#endif