#include "matrix.hpp"
#include <gtest/gtest.h>

#include <memory>
#include <vector>
TEST(MathTests, MatrixWorks) {
    EXPECT_EQ(
        dotProduct(std::vector{1.0, 2.0, 3.0}, std::vector{1.0, 2.0, 3.0}),
        14.0);
    EXPECT_EQ(
        dotProduct(std::vector{2.0, 2.0, 3.0}, std::vector{1.0, 2.0, 3.0}),
        15.0);
}
class MatrixTest : public ::testing::Test {
  protected:
    std::unique_ptr<Matrix> mat;
    void SetUp() override { mat = std::make_unique<Matrix>(3, 3); }
};

TEST_F(MatrixTest, CanInitialiseMatrix) {

    EXPECT_EQ(mat->x, 3);
    EXPECT_EQ(mat->y, 3);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_DOUBLE_EQ((*mat)[i][j], 0.0)
                << "Failure at index [" << i << "][" << j << "]";
        }
    }
    EXPECT_EQ(mat->operator[]().size(), 9);
}

TEST_F(MatrixTest, CanAssignDataCorrectly) {
    mat->assignData({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

    EXPECT_EQ(mat->newData, (std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9}));
}
