#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include <vector>
#include "../obj/planetary_object.hh"

using std::cout;
using std::endl;
using std::vector;

//Calculates single gravitational acceleration vector

void gravitational_acceleration_vector(double &subject_mass, double &foreign_object_mass, 
    double &gravitational_constant, double* position_subject, 
    double* position_foreign_object, double* resultant_a) {
        for (int i = 0; i < num_dims; i++) {
            resultant_a[i] = ((subject_mass*foreign_object_mass*gravitational_constant)
                /((position_subject[i] - position_foreign_object[i])
                *(position_subject[i] - position_foreign_object[i])));
            resultant_a[i] /= subject_mass;
        }
};