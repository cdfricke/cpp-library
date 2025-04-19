#include "Matrix.h"

template<typename U>
void checkState(const Matrix<U>& A) {
    std::cout << A.rows() << 'x' << A.cols() << '\n';
    A.show();
}

int main() {

    Vector<double> test1(5);
    Vector<double> test2(test1);
    Vector<double> test3;

    test1.show();
    test2.show();
    test3.show();

    std::cout << test1 << '\n';
    std::cout << test2 << '\n';
    std::cout << test3 << std::endl;

    return 0;
}