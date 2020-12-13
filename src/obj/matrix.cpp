#include "../math/constants.cpp"
#include <cstdint>
#include <string>
#include <string.h>

#if 0
#include <iostream>

using std::cout;
using std::endl;
#endif

using std::string;
class Matrix {
private:
    double* data;
    uint64_t timesteps_stored;
    uint64_t dims;
    uint64_t get_offset(const uint64_t& dimension, const uint64_t index){
        return dimension*dims + index;
    }
    uint64_t* get_dim_index(const uint64_t& offset){
        uint64_t* dim_arr = new uint64_t[2];
        dim_arr[0] = offset/dims;
        dim_arr[1] = offset%dims;
        return dim_arr;
    }
public:
    Matrix(uint64_t amount_to_archive = 5, uint64_t dimensions = num_dims) : timesteps_stored(amount_to_archive), dims(dimensions) {
        uint64_t num_elements = dims*timesteps_stored;
        data = new double[num_elements];
        memset(data, 0, num_elements*sizeof(double));
    };
    void setAtPoint(uint64_t &dimension, uint64_t &index, double &input_data) {
        data[get_offset(dimension, index)] = input_data;
    }
    void setAtPoint(string &dimensionl, uint64_t &index, double &input_data) {
        uint64_t dimension;
        if (dimensionl == "x") {
            dimension = 0;
        } else if (dimensionl == "y") {
            dimension = 1;
        } else if (dimensionl == "z") {
            dimension = 2;
        }
        data[get_offset(dimension, index)] = input_data;
    }
    void retrieveAtPoint(uint64_t &dimension, uint64_t &index, double &output_data) {
        output_data = data[get_offset(dimension, index)];
    }
    void retrieveAtPoint(string &dimensionl, uint64_t &index, double &output_data) {
        uint64_t dimension;
        if (dimensionl == "x") {
            dimension = 0;
        } else if (dimensionl == "y") {
            dimension = 1;
        } else if (dimensionl == "z") {
            dimension = 2;
        }
        // uint64_t location = (((dimension - 1) * x) + index) - 1;
        output_data = data[get_offset(dimension, index)];
    }
};
#if 0
int main(){
    const uint64_t x = 2;
    const uint64_t y = 100;
    Matrix m(2,100);
    uint64_t dim, index;
    dim = index = 0;
    for(double i = 0; i<x*y; i++){
        index++;
        if(index>=y){index = 0;dim++;}
        if(dim>=x){dim = 0;}
        m.setAtPoint(dim, index, i);
    }
    dim = index = 0;
    double output_var;
    for(double i = 0; i<x*y; i++){
        index++;
        if(index>=y){index = 0;dim++;}
        if(dim>=x){dim = 0;}
        m.retrieveAtPoint(dim, index, output_var);
        cout << i <<": " << output_var<<endl;
    }
    return 0;
}
#endif