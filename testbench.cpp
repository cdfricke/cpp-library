#include "Matrix.h"

int main() {

    Matrix<int> A = {{1, 2, 3}, {1, 2, 3}, {2, 3, 4}};
    Matrix<int> B = {{1, 1, 1}, {1, 1, 1}, {2, 2, 2}};

    Matrix<int> sum = A + B;

    Matrix<int> product = sum * 6;

    product.show();

    return 0;
}