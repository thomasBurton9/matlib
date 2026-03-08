#include <print>
#include <vector>


#include "matrix.hpp"

using std::print;
using std::println;
using std::vector;
using std::initializer_list;

void Matrix::display() {
    for (vector<double> vector : data) {
        for (double scalar : vector) {
            print("{} ", scalar);
        }
        println("");
    }
}

void Matrix::assignData(initializer_list<initializer_list<double>> mat) {
    // Assuming the correct shape is pased currently
    int i = 0;
    for (initializer_list<double> vector : mat) {
        int j = 0;
        for (double scalar : vector) {
            data[i][j] = scalar;
            j++;
        }
        i++;
    }
}
