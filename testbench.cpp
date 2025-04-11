#include "Matrix.h"

template<typename U>
void checkState(const Matrix<U>& A) {
    std::cout << A.rows() << 'x' << A.cols() << '\n';
    A.show();
}

int main() {

    Matrix<int> A = {{1, 2, 3, 2}, {1, 2, 3, 1}, {2, 3, 4, 8}};
    Matrix<int> B;
    checkState(A);
    checkState(B);

    Matrix<int> sum = A + B;
    checkState(sum);

    A.clear();
    checkState(A);

    A.set(2, 2, 10.3);
    checkState(A);

    Matrix<double> C = {{1.0, 3.0, 5.0}, {2.0, 4.0, 6.0}, {3.0, 5.0, 7.0}};
    checkState(C*5.0);

    C.resize(3, 4);
    checkState(C);

    C = 5.0 * C;
    checkState(C);

    return 0;
}