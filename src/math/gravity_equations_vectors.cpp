#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include "constants.cpp"

using std::cout;
using std::endl;
using std::vector;

//Calculates single gravitational acceleration vector

void gravitational_force_component(double &subject_mass, double &foreign_object_mass, 
    double &gravitational_constant, double& position_subject, 
    double& position_foreign_object, double& resultant_f) {
      // difference in positions
      double dx = position_subject-position_foreign_object;
      
      //G*m1*m2
      double numerator = gravitational_constant * subject_mass * foreign_object_mass;

      resultant_f = ((numerator)/(dx*dx)); //TODO: Is this okay because r is normally 3d distance, not 1d distance...?
};