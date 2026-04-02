#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <optional>
#include <ostream>
#include <print>
#include <span>
#include <string>
#include <tuple>
#include <vector>

class Matrix;

double dotProduct(std::span<const double> x, std::span<const double> y);

std::vector<double> getColumn(Matrix &matrix, unsigned index);

/// @brief Matrix class to be used for numerical computation.
class Matrix {
  public:
    Matrix(unsigned xSize, unsigned ySize, bool zeros = true)
        : data(xSize, std::vector<double>(ySize, 0.0)), newData(xSize * ySize),
          x{xSize}, y{ySize} {}

    std::vector<std::vector<double>> data;
    std::vector<double> newData;
    unsigned x;
    unsigned y;

    /// @brief Operator overloading to allow nice printing of Matrix class
    /// @param os Current output stream
    /// @param obj Matrix to print
    /// @return a stream to print
    friend std::ostream &operator<<(std::ostream &os, Matrix &obj) {
        // Currently inefficient
        std::vector<unsigned> max_values(obj.y, 0U);

        for (int i = 0; i < obj.x; i++) {
            for (int j = 0; j < obj.y; j++) {
                if (std::to_string(obj[i][j]).size() > max_values[j]) {
                    max_values[j] = std::to_string(obj[i][j]).size();
                }
            }
        }
        for (int i = 0; i < obj.x; i++) {
            std::print("|");
            for (int j = 0; j < obj.y; j++) {
                int number_size = std::to_string(obj[i][j]).size();
                if (j == 0) {
                    number_size += 1;
                }
                std::string full_string =
                    std::string(max_values[j] - number_size + 1, ' ');
                std::print("{}{}", full_string, obj[i][j]);
            }
            std::println("|");
        }

        return os;
    }

    // INDEXING and GETTING DATA

    std::span<double> operator[](std::optional<int> idx = std::nullopt) {
        std::span<double> full_span(newData);
        if (idx.has_value()) {

            std::span<double> return_data = full_span.subspan((*idx * y), y);
            return return_data;
        } else {
            return full_span;
        }
    }

    std::span<double> get_all_data() { return operator[](); }

    // TRANSPOSING

    /// @brief Transposes the matrix, swapping rows and columns
    /// @return A transpose of the current matrix
    Matrix transpose() {
        Matrix temp(y, x);
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp[j][i] = (*this)[i][j];
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
    Matrix operator+(Matrix &obj) {
        if (x != obj.x || y != obj.y) {
            throw std::invalid_argument("Shape mismatch between 2 matrices");
        }
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp[i][j] = (*this)[i][j] + obj[i][j];
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
                temp[i][j] = data[i][j] + scalar;
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
    Matrix operator-(Matrix &obj) {
        if (x != obj.x || y != obj.y) {
            throw std::invalid_argument("Shape mismatch between 2 matrices");
        }
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp[i][j] = data[i][j] - obj[i][j];
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
                temp[i][j] = data[i][j] - scalar;
            }
        }

        return temp;
    }
    /// @brief Allows scalar addition to be performed when scalar is on the left
    /// @param scalar The scalar to add
    /// @param obj A reference to the matrix on the right.
    /// @return The matrix with addition performed on it
    friend Matrix operator-(double scalar, Matrix &obj) {
        Matrix temp(obj.x, obj.y);

        for (int i = 0; i < obj.x; i++) {
            for (int j = 0; j < obj.y; j++) {
                temp[i][j] = scalar - obj[i][j];
            }
        }

        return temp;
    }

    // MULTIPLICATION

    /// @brief Performs scalar multiplication on each value of matrix
    /// @param scalar The scalar to multiply by
    /// @return The matrix that has been multiplied
    Matrix operator*(double scalar) {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp[i][j] = (*this)[i][j] * scalar;
            }
        }
        return temp;
    }
    /// @brief Allows scalar multiplication to be performed when scalar is on
    /// the left
    /// @param scalar The scalar to multiply by
    /// @param obj The matrix to apply multiplication
    /// @return The matrix that has been multiplied
    friend Matrix operator*(double scalar, Matrix &obj) { return obj * scalar; }

    // Note that there is currently no operation for regular multiplication by
    // item with 2 equally sized n by n matrices. Potentially move matmul to a
    // regular function and use * operator for itemwise multiplication

    /// @brief Performs matrix multiplication between 2 matrices with sizes n *
    /// m, m * p
    /// @param obj Other matrix
    /// @return Returns matrix with size n * p
    Matrix operator*(Matrix &obj) {
        if (y != obj.x) {
            throw std::invalid_argument(
                "Shape mismatch, inner shapes must be the same");
        }
        // Assert the size of the inner ones is equal
        // Inefficient
        Matrix temp(x, obj.y);
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < obj.y; j++) {
                temp[i][j] = dotProduct(
                    (*this)[i],
                    getColumn(obj, j)); // getColumn might be inefficient
            }
        }

        return temp;
    }

    // DIVISION

    /// @brief Performs scalar division on each value of matrix
    /// @param scalar The scalar to divide by
    /// @return The matrix that has been divided
    Matrix operator/(double scalar) {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp[i][j] = (*this)[i][j] * scalar;
            }
        }
        return temp;
    }
    // Note that there is no division of scalar by matrix given it is generally
    // undefined in linear algebra Could potentially make it multiply by the
    // inverse of the matrix.

    void operator()(int x, int y) { std::println("{} ", (*this)[x][y]); }

    void display();

    void assignData(const std::vector<std::vector<double>> &data);
    void assignData(std::initializer_list<std::initializer_list<double>> mat);
};

Matrix identityMatrix(unsigned size);

// TRACE

double matrixTrace(Matrix &mat);

// MATRIX SOLVING

std::tuple<Matrix, Matrix> createUpperDiagonalSingularMatrix(const Matrix &A,
                                                             const Matrix &b);

std::vector<double> createMainDiagonalMatrix(const Matrix &A, const Matrix &b);

std::vector<double> guassianSolve(const Matrix &A, const Matrix &b);

// LU

std::tuple<Matrix, Matrix> LUDecomposition(const Matrix &A);

// DETERMINANTS

double determinantLeibniz(Matrix &mat);

double determinantLU(Matrix &mat);

// PERMUTATION

int factorial(int number);

bool isEvenPermutation(std::vector<double> &permutation);

void swapValues(int i, int j, std::vector<double> &Array);

void heapsAlgorithm(int k, std::vector<int> &A,
                    std::vector<std::vector<int>> &allPermutations);