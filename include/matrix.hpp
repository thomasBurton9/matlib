#pragma once

#include <cmath>
#include <iostream>
#include <ostream>
#include <print>
#include <string>
#include <vector>

std::vector<double> getColumn(const std::vector<std::vector<double>> &mat_data,
                              unsigned index);

double dotProduct(const std::vector<double> &x, const std::vector<double> &y);

class Matrix {
  public:
    Matrix(unsigned xSize, unsigned ySize, bool zeros = true)
        : x{xSize}, y{ySize}, data(xSize, std::vector<double>(ySize, 0.0)) {}

    std::vector<std::vector<double>> data;

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

    Matrix transpose() {
        Matrix temp(y, x);
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[j][i] = data[i][j];
            }
        }
        return temp;
    }

    Matrix T() { return transpose(); }

    Matrix operator+(const Matrix &obj) const {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] + obj.data[i][j];
            }
        }
        return temp;
    }

    Matrix operator+(double scalar) const {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] + scalar;
            }
        }

        return temp;
    }

    friend Matrix operator+(double scalar, const Matrix &obj) {
        return obj + scalar;
    }
    // When the scalar is on the right
    Matrix operator*(int scalar) const {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] * scalar;
            }
        }
        return temp;
    }
    // When the scalar is to the left.
    friend Matrix operator*(int scalar, const Matrix &obj) {
        return obj * scalar;
    }

    // The other matrix is on the right.
    Matrix operator*(Matrix &obj) const {
        // Assert the size of the inner ones is equal
        // Inefficient
        Matrix temp(x, obj.y);
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < obj.y; j++) {
                temp.data[i][j] = dotProduct(data[i], getColumn(obj.data, j));
            }
        }

        return temp;
    }

    void operator()(int x, int y) const { std::println("{} ", data[x][y]); }

    void display();
    void assignData(std::initializer_list<std::initializer_list<double>> mat);

  private:
    unsigned x;
    unsigned y;
};
