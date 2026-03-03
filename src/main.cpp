#include <print>

#include "matrix.hpp"
using std::println;

int main() {
    Matrix mat{3, 3};
    mat.display();
    mat.assignData({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    mat.display();
}