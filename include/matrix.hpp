#pragma once

#include <cmath>
#include <ostream>
#include <print>
#include <string>
#include <vector>


inline std::string operator*(std::string_view s, std::size_t n) {
    std::string result{};

    for (std::size_t i = 0; i < n; i++) {
        result += s;
    }
    return result;
}

class Matrix {
  public:
    Matrix(int xSize, int ySize, bool zeros = true)
        : x{xSize}, y{ySize}, data(xSize, std::vector<double>(ySize, 0.0)) {}

    std::vector<std::vector<double>> data;

    friend std::ostream &operator<<(std::ostream &os, const Matrix &obj) {
        uint32_t max = 0; // Compare here to keep perf
        for (std::vector<double> vector : obj.data) {
            for (double scalar : vector) {
                if (std::to_string(scalar).size() > max) {
                    max = std::to_string(scalar).size();
                }
            }
        }

        for (std::vector<double> vector : obj.data) {
            for (double scalar : vector) {
                int number_size = std::to_string(scalar).size();
                std::string full_string = std::string(" ") * static_cast<size_t>(max - number_size + 1);
                std::print("{}", full_string);
                std::print("{}", scalar);
            }
            std::println("");
        }
        return os;
    };

    Matrix operator+(const Matrix &obj) {
        Matrix temp(x, y);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                temp.data[i][j] = data[i][j] + obj.data[i][j];
            }
        }
        return temp;
    }
    void operator()(int x, int y) const { std::println("{} ", data[x][y]); }

    void display();
    void assignData(std::initializer_list<std::initializer_list<double>> mat);

  private:
    int x;
    int y;
};
