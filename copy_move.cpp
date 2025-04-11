#include "Matrix.h"
#include <utility> // for std::move

int main() {
    std::cout << "Creating A\n";
    Matrix<int> A(2, 2);

    std::cout << "\nCreating B as a copy of A\n";
    Matrix<int> B = A; // copy constructor

    std::cout << "\nAssigning B = A\n";
    B = A; // copy assignment

    std::cout << "\nCreating C from a temporary (move)\n";
    Matrix<int> C = Matrix<int>(2, 2); // move constructor

    std::cout << "\nCreating D, then assigning it a temporary (move)\n";
    Matrix<int> D(2, 2);
    D = Matrix<int>(2, 2); // move assignment

    std::cout << "\nMoving A into E\n";
    Matrix<int> E = std::move(A); // move constructor

    std::cout << "\nDone!\n";
    return 0;
}