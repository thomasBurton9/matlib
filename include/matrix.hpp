#pragma once

#include <cmath>
#include <iostream>
#include <ostream>
#include <print>
#include <string>
#include <vector>
#include <tuple>

std::vector<double> getColumn(const std::vector<std::vector<double>> &mat_data,
                              unsigned index);

double dotProduct(const std::vector<double> &x, const std::vector<double> &y);

/// @brief Matrix class to be used for numerical computation.
class Matrix {
  public:
    Matrix(unsigned xSize, unsigned ySize, bool zeros = true)
        : x{xSize}, y{ySize}, data(xSize, std::vector<double>(ySize, 0.0)) {}

    std::vector<std::vector<double>> data;

    unsigned x;
    unsigned y;

    /// @brief Operator overloading to allow nice printing of Matrix class
    /// @param os Current output stream
    /// @param obj Matrix to print
    /// @return a stream to print
    friend std::ostream &operator<<(std::ostream &os, const Matrix &obj) {
        // Currently inefficient
        std::vector<unsigned> max_values(obj.y, 0U);
        // unsigned maximum_value = 0; // Compare here to keep perf
        for (int i = 0; i < obj.x; i++) {
            for (int j = 0; j < obj.y; j++) {
                if (std::to_string(obj.data[i][j]).size() > max_values[j]) {
                    max_values[j] = std::to_string(obj.data[i][j]).size();
                }
            }
        }
        for (int i = 0; i < obj.x; i++) {
            std::print("|");
            for (int j = 0; j < obj.y; j++) {
                int number_size = std::to_string(obj.data[i][j]).size();
                if (j == 0) {
                    number_size += 1;
                }
                std::string full_string =
                    std::string(max_values[j] - number_size + 1, ' ');
                std::print("{}{}", full_string, obj.data[i][j]);
            }
            std::println("|");
        }

        return os;
    }

    // INDEXING DATA

    std::vector<double>& operator[](int idx) {
        return data[idx];
    }

    // TRANSPOSING

    /// @brief Transposes the matrix, swapping rows and columns
    /// @return A transpose of the current matrix
    Matrix transpose() {
        Matrix temp(y, x);
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[j][i] = data[i][j];
            }
        }
        return temp;
    }
    /// @brief Shortened version to transpose matrix
    /// @return A transpose of the current matrix
    Matrix T() { return transpose(); }

    // ADDITION

    /// @brief Performs addition of 2 matrices with the same dimensions
    /// @param obj Reference to other matrix
    /// @return A matrix that is the sum of the 2 matrices
    Matrix operator+(const Matrix &obj) const {
        if (x != obj.x || y != obj.y) {
            throw std::invalid_argument("Shape mismatch between 2 matrices");
        }
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] + obj.data[i][j];
            }
        }
        return temp;
    }
    /// @brief Adds a scalar to each value in matrix
    /// @param scalar The scalar to add
    /// @return The matrix with addition performed on it
    Matrix operator+(double scalar) const {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] + scalar;
            }
        }

        return temp;
    }
    /// @brief Allows scalar addition to be performed when scalar is on the left
    /// @param scalar The scalar to add
    /// @param obj A reference to the matrix on the right.
    /// @return The matrix with addition performed on it
    friend Matrix operator+(double scalar, const Matrix &obj) {
        return obj + scalar;
    }

    // SUBTRACTION

    /// @brief Performs subtraction of 2 matrices with the same dimensions
    /// @param obj Reference to other matrix
    /// @return A matrix that is the difference of the 2 matrices
    Matrix operator-(const Matrix &obj) const {
        if (x != obj.x || y != obj.y) {
            throw std::invalid_argument("Shape mismatch between 2 matrices");
        }
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] - obj.data[i][j];
            }
        }
        return temp;
    }

    /// @brief Subtracts a scalar from each value in matrix
    /// @param scalar The scalar to add
    /// @return The matrix with subtraction performed on it
    Matrix operator-(double scalar) const {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] - scalar;
            }
        }

        return temp;
    }
    /// @brief Allows scalar addition to be performed when scalar is on the left
    /// @param scalar The scalar to add
    /// @param obj A reference to the matrix on the right.
    /// @return The matrix with addition performed on it
    friend Matrix operator-(double scalar, const Matrix &obj) {
        Matrix temp(obj.x, obj.y);

        for (int i = 0; i < obj.x; i++) {
            for (int j = 0; j < obj.y; j++) {
                temp.data[i][j] = scalar - obj.data[i][j];
            }
        }

        return temp;
    }

    // MULTIPLICATION

    /// @brief Performs scalar multiplication on each value of matrix
    /// @param scalar The scalar to multiply by
    /// @return The matrix that has been multiplied
    Matrix operator*(double scalar) const {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] * scalar;
            }
        }
        return temp;
    }
    /// @brief Allows scalar multiplication to be performed when scalar is on
    /// the left
    /// @param scalar The scalar to multiply by
    /// @param obj The matrix to apply multiplication
    /// @return The matrix that has been multiplied
    friend Matrix operator*(double scalar, const Matrix &obj) {
        return obj * scalar;
    }

    // Note that there is currently no operation for regular multiplication by item with 2 equally sized n by n matrices.
    // Potentially move matmul to a regular function and use * operator for itemwise multiplication

    /// @brief Performs matrix multiplication between 2 matrices with sizes n *
    /// m, m * p
    /// @param obj Other matrix
    /// @return Returns matrix with size n * p
    Matrix operator*(Matrix &obj) const {
        if (y != obj.x) {
            throw std::invalid_argument(
                "Shape mismatch, inner shapes must be the same");
        }
        // Assert the size of the inner ones is equal
        // Inefficient
        Matrix temp(x, obj.y);
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < obj.y; j++) {
                temp.data[i][j] = dotProduct(
                    data[i],
                    getColumn(obj.data, j)); // getColumn might be inefficient
            }
        }

        return temp;
    }

    // DIVISION

    /// @brief Performs scalar division on each value of matrix
    /// @param scalar The scalar to divide by
    /// @return The matrix that has been divided
    Matrix operator/(double scalar) const {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] * scalar;
            }
        }
        return temp;
    }
    // Note that there is no division of scalar by matrix given it is generally undefined in linear algebra
    // Could potentially make it multiply by the inverse of the matrix.

    void operator()(int x, int y) const { std::println("{} ", data[x][y]); }

    void display();
    void assignData(std::initializer_list<std::initializer_list<double>> mat);

};

Matrix identityMatrix(unsigned size);

// MATRIX SOLVING

std::tuple<Matrix, Matrix> createUpperDiagonalMatrix(const Matrix &A, const Matrix &b);

std::vector<double> createMainDiagonalMatrix(const Matrix &A, const Matrix &b);

std::vector<double> guassianSolve(const Matrix &A, const Matrix &b);

// DETERMINANTS

double determinantLeibniz(Matrix &mat);

// PERMUTATION

int factorial(int number);

bool isEvenPermutation(std::vector<double> &permutation);


void swapValues(int i, int j, std::vector<double> &Array);

void heapsAlgorithm(int k, std::vector<int> &A, std::vector<std::vector<int>> &allPermutations);