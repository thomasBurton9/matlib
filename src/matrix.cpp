#include <numeric>
#include <print>
#include <tuple>
#include <vector>

#include "matrix.hpp"

using std::initializer_list;
using std::iota;
using std::print;
using std::println;
using std::tuple;
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
        temp[i][i] = 1;
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
/// @brief Takes in an upper diagonal matrix and solves the equation Ax = b for
/// x.
/// @param A Upper Diagonal matrix - 1s along main diagonal and 0s below it of
/// shape n x n
/// @param b The solution vector of shape n x 1
/// @return The solution "x" in Ax = b.
vector<double> getCoefficientSolutions(const Matrix &A, const Matrix &b) {

    // Check if arguments have correct shape.
    if (A.x != A.y) {
        throw std::invalid_argument("A must be a square matrix");
    } else if (A.x != b.x) {
        throw std::invalid_argument("b must be same length as A");
    } else if (b.y != 1) {
        throw std::invalid_argument("b must be of size n x 1");
    }

    Matrix tempA = A;
    Matrix tempb = b;

    vector<double> results(b.x, 0);

    for (int i = b.x - 1; i >= 0; i--) {
        for (int j = i + 1; j < b.x; j++) {
            tempb[i][0] = tempb[i][0] - tempb[j][0] * tempA[i][j];
            tempA[i][j] = tempA[i][j] - tempA[i][j];
        }
    }

    for (int i = 0; i < b.x; i++) {
        results[i] = tempb[i][0];
    }

    return results;
}
/// @brief Creates an upper diagonal matrix to help in solving Ax = b
/// @param A Square "coefficient" matrix A size n x n
/// @param b Result matrix, should be size n x 1
/// @return Resulting upper diagonal Matrix tempA, which has 1s along the main
/// diagonal and 0s below it.
/// @return Matrix tempb will be the resulting b matrix after all the
/// mathematical manipulating
tuple<Matrix, Matrix> createUpperDiagonalMatrix(const Matrix &A,
                                                const Matrix &b) {
    // Check if arguments have correct shape.
    if (A.x != A.y) {
        throw std::invalid_argument("A must be a square matrix");
    } else if (A.x != b.x) {
        throw std::invalid_argument("b must be same length as A");
    } else if (b.y != 1) {
        throw std::invalid_argument("b must be of size n x 1");
    }
    // Create temporary matrices to allow modification
    Matrix tempA = A;
    Matrix tempb = b;
    // Go row by row
    for (int i = 0; i < A.x; i++) {
        // Initially make all the values to the left of the main diagonal in the
        // current row 0.
        for (int k = 0; k < i; k++) {
            double A_i_k = tempA[i][k];
            for (int l = k; l < A.x; l++) {
                tempA[i][l] = tempA[i][l] - A_i_k * tempA[k][l];
            }
            tempb[i][0] = tempb[i][0] - A_i_k * tempb[k][0];
        }
        // Normalize main diagonal to 1 in the current row
        double A_i_i = tempA[i][i];
        for (int j = 0; j < A.y; j++) {
            if (tempA[i][i] == 0.0) {
                bool swapped = false;
                for (int m = i + 1; m < A.x; m++) {
                    if (tempA[m][i] != 0) {
                        // Swap Rows
                        vector<double> tempRow = tempA[m];
                        double tempSol = tempb[m][0];
                        tempA[m] = tempA[i];
                        tempb[m][0] = tempb[i][0];

                        tempA[i] = tempRow;
                        tempb[i][0] = tempSol;

                        swapped = true;
                        break;
                    }
                }
                if (!swapped) {
                    throw std::runtime_error("Not implemented yet");
                }
            }
            tempA[i][j] = tempA[i][j] / A_i_i; // Convert the main number to a 1
        }
        tempb[i][0] = tempb[i][0] / A_i_i; // Do the same to b
    }
    return {tempA, tempb};
}
/// @brief Solves Ax = b using guassian elimination
/// @param A Matrix A of shape n x n
/// @param b Matrix b of shape n x 1
/// @return The solutions ("x") to the equation Ax = b
vector<double> guassianSolve(const Matrix &A, const Matrix &b) {
    auto [upperA, upperb] = createUpperDiagonalMatrix(A, b);
    vector<double> resultVector = getCoefficientSolutions(upperA, upperb);

    return resultVector;
}

double determinant(const Matrix &mat) {
    // Check if arguments have correct shape.
    if (mat.x != mat.y) {
        throw std::invalid_argument("Mat must be a square matrix");
    }
    vector<double> S_n(mat.x);

    iota(S_n.begin(), S_n.end(),
         1); // Create vector from 1 to n where n is length of matrix mat

    return 0.0; // Temp value
}
/// @brief Function to determine if a permutation is even, via counting
/// inversions
/// @param permutation Permutation to calculate
/// @return True if even, false if odd permutation
bool isEvenPermutation(vector<double> &permutation) {
    // O(n^2)
    int inversionCount = 0;

    for (int i = 0; i < permutation.size() - 1; i++) {
        for (int j = i + 1; j < permutation.size(); j++) {
            if (permutation[i] > permutation[j]) {
                inversionCount++;
            }
        }
    }

    return inversionCount % 2 == 0;
}