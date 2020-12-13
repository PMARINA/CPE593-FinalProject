#include "../math/constants.cpp"
#include <cstdint>
#include <string>

using std::string;
class Matrix {
private:
    double* data;
    uint64_t x;
    uint64_t y;
public:
    Matrix(uint64_t amount_to_archive = 5, uint64_t dimensions = num_dims) : x(amount_to_archive), y(dimensions) {
        uint64_t counter = 0;
        for (int i = 0; i < dimensions; i++) {
            for (int j = 0; j < amount_to_archive; j++, counter++)
                data[counter] = 0;
        }
    };
    void setAtPoint(uint64_t &dimension, uint64_t &index, double &input_data) {
        uint64_t location = (((dimension - 1) * x) + index) - 1;
        data[location] = input_data;
    }
    void setAtPoint(string &dimensionl, uint64_t &index, double &input_data) {
        double dimension;
        if (dimensionl == "x") {
            dimension = 1;
        } else if (dimensionl == "y") {
            dimension = 2;
        } else if (dimensionl == "z") {
            dimension = 3;
        }
        uint64_t location = (((dimension - 1) * x) + index) - 1;
        data[location] = input_data;
    }
    void retrieveAtPoint(uint64_t &dimension, uint64_t &index, double &output_data) {
        uint64_t location = (((dimension - 1) * x) + index) - 1;
        output_data = data[location];
    }
    void retrieveAtPoint(string &dimensionl, uint64_t &index, double &output_data) {
        double dimension;
        if (dimensionl == "x") {
            dimension = 1;
        } else if (dimensionl == "y") {
            dimension = 2;
        } else if (dimensionl == "z") {
            dimension = 3;
        }
        uint64_t location = (((dimension - 1) * x) + index) - 1;
        output_data = data[location];
    }
};