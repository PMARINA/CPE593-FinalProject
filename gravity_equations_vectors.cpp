#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

//Calculates single gravitational acceleration vector

void gravitational_acceleration_vector(double &subject_mass, double &foreign_object_mass, 
    double &gravitational_constant, vector<double> &position_subject, 
    vector<double> &position_foreign_object, vector<double> &resultant_force) {
        for (int i = 0; i < position_subject.size(); i++) {
            resultant_force[i] = ((subject_mass*foreign_object_mass*gravitational_constant)
                /((position_subject[i] - position_foreign_object[i])
                *(position_subject[i] - position_foreign_object[i])));
            resultant_force[i] /= subject_mass;
        }
};