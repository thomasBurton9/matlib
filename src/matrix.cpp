#include <print>
#include <vector>

#include "matrix.hpp"

using std::initializer_list;
using std::print;
using std::println;
using std::vector;

/// @brief Method to print matrix in nice way
void Matrix::display() {

    // Currently inefficient
    std::vector<unsigned> max_values(y, 0U);
    // Compare here to keep perf
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (std::to_string(data[i][j]).size() > max_values[j]) {
                max_values[j] = std::to_string(data[i][j]).size();
            }
        }
    }
    for (int i = 0; i < x; i++) {
        std::print("|");
        for (int j = 0; j < y; j++) {
            int number_size = std::to_string(data[i][j]).size();
            if (j == 0) {
                number_size += 1;
            }
            std::string full_string =
                std::string(max_values[j] - number_size + 1, ' ');
            std::print("{}{}", full_string, data[i][j]);
        }
        std::println("|");
    }
}
/// @brief Method to add date to matrices
/// @param mat A 2d array of double's. Error is thrown if the shape isn't equal
/// to the base matrix.
void Matrix::assignData(initializer_list<initializer_list<double>> mat) {
    // Assuming the correct shape is pased currently
    if (mat.size() != x) {
        throw std::invalid_argument(
            "Data must be the same length as base matrix");
    }

    for (initializer_list<double> vector : mat) {
        if (vector.size() != y) {
            throw std::invalid_argument(
                "Data must be the same length and shape as in the base matrix");
        }
    }
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
/// @brief Creates an identity matrix I of size size x size with 1's on the main
/// diagonal and 0's everywhere.
/// @param size How large the Matrix is
/// @return A matrix class with x = size, y = size and 1's along the main
/// diagonal
Matrix identityMatrix(unsigned size) {

    Matrix temp = Matrix(size, size);
    for (int i = 0; i < size; i++) {
        temp.data[i][i] = 1;
    }

    return temp;
}

vector<double> getColumn(const vector<vector<double>> &mat_data,
                         unsigned index) {
    vector<double> column_vector{};
    for (int i = 0; i < mat_data.size(); i++) {
        column_vector.push_back(mat_data[i][index]);
    }
    return column_vector;
}
/// @brief Basic dot product method between 2 vectors of same length
/// @param x Reference to vector with same length as y
/// @param y reference to vector with same length as x
/// @return Scalar product of the input vectors.
double dotProduct(const vector<double> &x, const vector<double> &y) {
    // Assert they are the same size.
    if (x.size() != y.size()) {
        throw std::invalid_argument("Both x and y must be the same length");
    }
    double sum(0.0);
    for (int i = 0; i < x.size(); i++) {
        sum += x[i] * y[i];
    }
    return sum;
}