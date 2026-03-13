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



vector<double> getColumn(const vector<vector<double>> &mat_data,
                              unsigned index) {
    vector<double> column_vector{};
    for (int i = 0; i < mat_data.size(); i++) {
        column_vector.push_back(mat_data[i][index]);
    }
    return column_vector;
}

double dotProduct(const vector<double> &x, const vector<double> &y) {
    // Assert they are the same size.
    double sum(0.0);
    for (int i = 0; i < x.size(); i++) {
        sum += x[i] * y[i];
    }
    return sum;
}