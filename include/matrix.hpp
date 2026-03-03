#pragma once

#include <vector>

class Matrix {
  public:
    Matrix(int xSize, int ySize, bool zeros = true)
        : x{xSize}, y{ySize}, data(xSize, std::vector<double>(ySize, 0.0)) {}

    std::vector<std::vector<double>> data;
    void display();
    void assignData(std::initializer_list<std::initializer_list<double>> mat);

  private:
    int x;
    int y;
};
