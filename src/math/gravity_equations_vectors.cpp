#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include "../obj/planetary_object.hh"

using std::cout;
using std::endl;
using std::vector;

//Calculates single gravitational acceleration vector

void gravitational_acceleration_component(double &subject_mass, double &foreign_object_mass, 
    double &gravitational_constant, double position_subject, 
    double position_foreign_object, double resultant_a) {
        resultant_a = ((subject_mass*foreign_object_mass*gravitational_constant)
            /((position_subject - position_foreign_object)
            *(position_subject - position_foreign_object)));
};