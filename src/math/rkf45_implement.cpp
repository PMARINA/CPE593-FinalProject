#include <bits/stdc++.h>

#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

/*
    f(t_n, y_n) returns the derivative of y at time t_n.
    For our purposes, we need f() to return two derivatives: The first
   derivative of position, v, and the second, a. Accordingly, we start with the
   equation for acceleration and our initial data, and use RKF45 to approximate
   the integrals of a and v, rather than starting from x and finding the
   derivatives.

    Our process then becomes:

    1) Start with initial data (position coordinates and velocity vectors)
        PRIMARY LOOP:
    2) Determine the gravitational force on each object by summing the g-force
   vectors induced by each other object in the system. 3) Divide by each
   object's mass to arrive at the acceleration vector for each body. 4) Use
   RKF45 to approximate its integral, finding the velocity one (adaptive)
   timestep later. 5) Use RKF45 to approximate the integral of the velocity at
   the initial time, finding the position of the body one (adaptive) timestep
   later. END LOOP.
*/

// Equations used internally within the integral estimates.

double RKF45k1(double &timestep, double &deriv) { return (timestep * deriv); };
double RKF45k2(double &timestep, double &deriv, double &initialValue) {
  double timestep_new = (.25 * timestep);
  double deriv_est = ((.25 * RKF45k1(timestep_new, deriv)));
  return (initialValue + deriv_est);
};
double RKF45k3(double &timestep, double &deriv, double &initialValue) {
  double timestep_new = ((3.0 / 8.0) * timestep);
  double deriv_est =
      ((3.0 / 32.0) * RKF45k1(timestep_new, deriv)) +
      ((9.0 / 32.0) * RKF45k2(timestep_new, deriv, initialValue));
  return (initialValue + deriv_est);
};
double RKF45k4(double &timestep, double &deriv, double &initialValue) {
  double timestep_new = ((12.0 / 13.0) * timestep);
  double deriv_est =
      ((1932.0 / 2197.0) * RKF45k1(timestep_new, deriv)) -
      ((7200.0 / 2197.0) * RKF45k2(timestep_new, deriv, initialValue)) +
      ((7296.0 / 2197.0) * RKF45k3(timestep_new, deriv, initialValue));
  return (initialValue + deriv_est);
};
double RKF45k5(double &timestep, double &deriv, double &initialValue) {
  double deriv_est =
      ((439.0 / 216.0) * RKF45k1(timestep, deriv)) -
      (8.0 * RKF45k2(timestep, deriv, initialValue)) +
      ((3680.0 / 513.0) * RKF45k3(timestep, deriv, initialValue)) -
      ((845.0 / 4104.0) * RKF45k4(timestep, deriv, initialValue));
  return (initialValue + deriv_est);
};
double RKF45k6(double &timestep, double &deriv, double &initialValue) {
  double timestep_new = .5 * timestep;
  double deriv_est =
      ((-8.0 / 27.0) * RKF45k1(timestep_new, deriv)) +
      (2.0 * RKF45k2(timestep_new, deriv, initialValue)) -
      ((3544.0 / 2565.0) * RKF45k3(timestep_new, deriv, initialValue)) +
      ((18959.0 / 4104.0) * RKF45k4(timestep_new, deriv, initialValue)) -
      ((11.0 / 40.0) * RKF45k5(timestep_new, deriv, initialValue));
  return (initialValue + deriv_est);
};

// The integral estimates themselves.

void RKF45_integral_estimate_fourth_order(double &timestep, double &deriv,
                                          double &initial_value,
                                          double &return_result_to) {
  return_result_to =
      (initial_value + ((25.0 / 216.0) * RKF45k1(timestep, deriv)) +
       ((1408.0 / 2565.0) * RKF45k3(timestep, deriv, initial_value)) +
       ((2197.0 / 4104.0) * RKF45k4(timestep, deriv, initial_value)) -
       (.2 * RKF45k5(timestep, deriv, initial_value)));
};

void RKF45_integral_estimate_fifth_order(double &timestep, double &deriv,
                                         double &initial_value,
                                         double &return_result_to) {
  return_result_to =
      (initial_value + ((16.0 / 135.0) * RKF45k1(timestep, deriv)) +
       ((6656.0 / 12825.0) * RKF45k3(timestep, deriv, initial_value)) +
       ((28561.0 / 56430.0) * RKF45k4(timestep, deriv, initial_value)) -
       (.18 * RKF45k5(timestep, deriv, initial_value)) +
       ((22.0 / 55.0) * RKF45k6(timestep, deriv, initial_value)));
};

// Function to return adaptive timestep coefficient.

void RKF45_fifth_order_timestep_coefficient(double &timestep, double &deriv,
                                            double &initial_value,
                                            double &tolerance,
                                            double &return_coefficient_to) {
  double error, fifth, fourth;
  RKF45_integral_estimate_fifth_order(timestep, deriv, initial_value, fifth);
  RKF45_integral_estimate_fourth_order(timestep, deriv, initial_value, fourth);
  error = fifth - fourth;
  return_coefficient_to = pow((tolerance * timestep) / (2 * abs(error)), 0.25);
};
