#include <iostream>
#include <bits/stdc++.h>

using std::cout;
using std::endl;

/*
    f(t_n, y_n) returns the derivative of y at time t_n.
    For our purposes, we need f() to return two derivatives: The first derivative of position, v, and the second, a.
    Accordingly, we start with the equation for acceleration and our initial data, and use RKF45 to approximate the 
    integrals of a and v, rather than starting from x and finding the derivatives.

    Our process then becomes:

    1) Start with initial data (position coordinates and velocity vectors)
        PRIMARY LOOP:
    2) Determine the gravitational force on each object by summing the g-force vectors induced by each other
    object in the system.
    3) Divide by each object's mass to arrive at the acceleration vector for each body.
    4) Use RKF45 to approximate its integral, finding the velocity one (adaptive) timestep later.
    5) Use RKF45 to approximate the integral of the velocity at the initial time, finding the position of the
    body one (adaptive) timestep later.
        END LOOP.
*/

double RKF45k1(double &timestep, double &deriv) {
    return (timestep * deriv);
};
double RKF45k2(double &timestep, double &deriv, double &initialValue) {
    double timestep_new = (.25*timestep);
    double deriv_est = ((.25*RKF45k1(timestep_new, deriv)));
    return (timestep * EulerEstimate(timestep_new, deriv_est, initialValue));
};
double RKF45k3(double &timestep, double &deriv, double &initialValue) {
    double timestep_new = ((3.0/8.0)*timestep);
    double deriv_est = ((3.0/32.0)*RKF45k1(timestep_new, deriv))+((9.0/32.0)*RKF45k2(timestep_new, deriv, initialValue));
   return (timestep * EulerEstimate(timestep_new, deriv_est, initialValue));
};
double RKF45k4(double &timestep, double &deriv, double &initialValue) {
    
};
double RKF45k5(double &timestep, double &deriv, double &initialValue);
double RKF45k6(double &timestep, double &deriv, double &initialValue);

double EulerEstimate(double &timestep, double &deriv, double &initialValue) {
    return (initialValue + (timestep * deriv));
};

double RKF45IntegralEstimate(double &timestep, double &deriv, double &initialValue) {

}