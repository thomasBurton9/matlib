#include <print>
#include <iostream>

#include "matrix.hpp"
using std::println;
using std::cout;

int main() {
    Matrix mat{3, 3};
    cout << (mat);
    mat.assignData({{1000, 2, 3}, {4, 1000, 1}, {7, 8, 6}});
    Matrix mat2{3, 3};
    mat2.assignData({{9, 1, 7}, {6, 5, 4}, {3, 2, 1}});
    cout << (mat);
    mat(2, 1);

    Matrix tempMat = mat + mat2;
    cout << tempMat;
}