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
            if (std::to_string((*this)[i][j]).size() > max_values[j]) {
                max_values[j] = std::to_string((*this)[i][j]).size();
            }
        }
    }
    for (int i = 0; i < x; i++) {
        std::print("|");
        for (int j = 0; j < y; j++) {
            int number_size = std::to_string((*this)[i][j]).size();
            if (j == 0) {
                number_size += 1;
            }
            std::string full_string =
                std::string(max_values[j] - number_size + 1, ' ');
            std::print("{}{}", full_string, (*this)[i][j]);
        }
        std::println("|");
    }
}
/// @brief Method to add Data to matrix via a vector
/// @param new_data A 2d vector of doubles. Error is thrown if the shape isn't
/// equal to the base matrix or data is missing.
void Matrix::assignData(const std::vector<std::vector<double>> &new_data) {
    if (new_data.size() != x) {
        throw std::invalid_argument(
            "Data must be the same length as base matrix");
    }

    for (vector<double> vector : new_data) {
        if (vector.size() != y) {
            throw std::invalid_argument(
                "Data must be the same length and shape as in the base matrix");
        }
    }

    // For storing data in a flat vector instead of 2d vector.
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            newData[(i)*y + (j)] = new_data[i][j];
        }
    }

    data = new_data;
}
/// @brief Method to add data to matrices via an initialization list
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
            newData[(i)*y + (j)] = scalar; // For storing data in a flat vector
                                           // instead of 2d vector.
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

/// @brief Function to get the trace (sum of elements on main diagonal)
/// @param mat n x n matrix to get trace off
/// @return The sum of elements on main diagonal
double matrixTrace(Matrix &mat) {
    if (mat.x != mat.y) {
        throw std::invalid_argument("Matrix must be of size n x n");
    }

    double sum = 0.0;
    for (int i = 0; i < mat.x; i++) {
        sum += mat[i][i];
    }

    return sum;
}

vector<double> getColumn(Matrix &matrix, unsigned index) {
    vector<double> column_vector{};
    for (int i = 0; i < matrix.x; i++) {
        column_vector.push_back(matrix[i][index]);
    }
    return column_vector;
}

/// @brief Basic dot product method between 2 vectors of same length
/// @param x Reference to vector with same length as y
/// @param y reference to vector with same length as x
/// @return Scalar product of the input vectors.
double dotProduct(std::span<const double> x, std::span<const double> y) {
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
tuple<Matrix, Matrix> createUpperDiagonalSingularMatrix(const Matrix &A,
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
                        auto tempRow = tempA[m];
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
    auto [upperA, upperb] = createUpperDiagonalSingularMatrix(A, b);
    vector<double> resultVector = getCoefficientSolutions(upperA, upperb);

    return resultVector;
}

std::tuple<Matrix, Matrix> LUDecomposition(const Matrix &A) {
    return {A, A};
}

/// @brief Function to get the factorial of an integer below 12
/// @param number Number to get the factorial off
/// @return The factorial of the number
int factorial(int number) {
    int sum = 1;

    for (int i = 1; i <= number; i++) {
        sum *= i;
    }
    return sum;
}
/// @brief Get the determinant of a matrix using the inefficient leibniz method
/// @param mat n x n matrix to get determinant off
/// @return The determinant of the matrix.
double determinantLeibniz(Matrix &mat) {
    // Check if arguments have correct shape.
    if (mat.x != mat.y) {
        throw std::invalid_argument("Mat must be a square matrix");
    }
    vector<int> S_n(mat.x);

    iota(S_n.begin(), S_n.end(),
         1); // Create vector from 1 to n where n is length of matrix mat

    vector<vector<int>> Permutations;
    Permutations.reserve(factorial(mat.x));

    heapsAlgorithm(mat.x, S_n, Permutations);

    double determinant = 0.0;

    for (int i = 0; i < Permutations.size(); i++) {
        double tempValue = 1.0;
        if (i % 2 != 0) {
            tempValue = -1;
        }
        for (int j = 0; j < Permutations[i].size(); j++) {
            int x = Permutations[i][j];
            tempValue = tempValue * (mat[j][Permutations[i][j] - 1]);
        }

        determinant += tempValue;
    }
    return determinant; // Temp value
}
/// @brief Swap function to swap 2 values in an array
/// @param i Index 1
/// @param j Index 2
/// @param Array Array to perform the operation on
void swapValues(int i, int j, vector<int> &Array) {
    int temp = Array[i];
    Array[i] = Array[j];
    Array[j] = temp;
}

/// @brief Performs Heaps algorithm returning all possible permutations of a
/// list of int's
/// @param k How many elements to operate on (Use k = A.size() as a default)
/// @param A Vector of int's to permute
/// @param allPermutations Array to add all the permutations to
void heapsAlgorithm(int k, vector<int> &A,
                    std::vector<std::vector<int>> &allPermutations) {
    if (k == 1) {
        allPermutations.push_back(A);
        return;
    } else {
        heapsAlgorithm(k - 1, A, allPermutations);

        for (int i = 0; i < k - 1; i++) {
            if (k % 2 == 0) {
                swapValues(i, k - 1, A);
            } else {
                swapValues(0, k - 1, A);
            }
            heapsAlgorithm(k - 1, A, allPermutations);
        }
    }
}

/// @brief Function to determine if a permutation is even, via counting
/// inversions
/// @param permutation Permutation to calculate
/// @return True if even, false if odd permutation
bool isEvenPermutation(vector<int> &permutation) {
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

/// @brief Get the determinant of matrix via LU decomposition
/// @param mat n x n matrix to get determinant of
/// @return scalar value of determinant
double determinantLU(Matrix &mat) {
    // Check if arguments have correct shape.
    if (mat.x != mat.y) {
        throw std::invalid_argument("A must be a square matrix");
    }
    int P = 1;

    // Create temporary matrices to allow modification
    Matrix tempA = mat;
    // Go row by row
    int i = 0;
    while (i < mat.x) {
        // Initially make all the values to the left of the main diagonal in the
        // current row 0
        for (int k = 0; k < i; k++) { // Go through all values that need to be 0
            double A_i_k = tempA[i][k] / tempA[k][k]; // Value to remove.
            for (int l = k; l < mat.x; l++) {
                tempA[i][l] = tempA[i][l] - A_i_k * tempA[k][l];
            }
        }

        if (tempA[i][i] == 0) {
            bool swapped = false;
            for (int m = i + 1; m < mat.x; m++) {
                if (tempA[m][i] != 0) {
                    // Swap Rows
                    auto tempRow = tempA[m];
                    tempA[m] = tempA[i];
                    tempA[i] = tempRow;
                    swapped = true;
                    P *= -1; // Track swaps
                    break;
                }
            }
            if (!swapped) {
                throw std::runtime_error("Not implemented yet");
            }
        } else {
            i++;
        }
    }
    double determinant = 1;
    for (int i = 0; i < mat.x; i++) {
        determinant *= tempA[i][i];
    }
    determinant *= P;
    return determinant;
}