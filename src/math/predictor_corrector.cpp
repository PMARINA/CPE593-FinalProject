#include <iostream>
#include <bits/stdc++.h>

using std::cout;
using std::endl;


/*
The predictor corrector will need the inital 4 steps to begin calculations yn, yn-1, yn-2, yn-3

We need to find a way to give each function to this (ie: velocity for each body) and it will calculate the next step value
*/


/* double function(double t, double p){ //function that gives derivative of p at time t
} */

void predictor(double &y, uint64_t &h, double &f0, double &f1, double &f2, double &f3, double &return_to){ //predictor: y, h step, f0 = fn, f1 = fn-1, f2 = fn-2, f3 = fn-3
    return_to = (y + ((h/24.0) * (-9.0*f3 + 37.0*f2 - 59.0*f1 + 55.0*f0)));
}

void corrector(double &y, uint64_t &h, double &f0, double &f1, double &f2, double &f3, double &return_to){ //corrector: y, h step, f0 = fn, f1 = fn-1, f2 = fn-2, f3 = fn-3
    return_to = (y + ((h/24.0) * (f2 - 5.0*f1 + 19.0*f0 + 9.0*f1)));
}

void predictor_corrector(double* initial_value, double* deriv_array, uint64_t &timestep, double &return_to){
//deriv_array contains fn-3, fn-2, fn-1, fn

//initial_value contains starting yn


/*     double t = 0; //initial time
    double h = 0.2; //step
    double f0 = 0, f1 = 0, f2 = 0, f3 = 0;

    // run function first 4 times for initial values
    f0 = function (t, p); //1

    t++;
    f1 = f0;
    f0 = function (t, p); //2

    f2 = f1;
    f1 = f0;
    t++;
    f0 = function (t, p); //3

    f3 = f2;
    f2 = f1;
    f1 = f0;
    t++;
    f0 = function (t, p); //4

    double initial_value = initial_y; //this should be the current value (ie: current velocity)


    // loop now for each new step
    while (true){ //will define for how long to run simulation
        p = predictor (y, h, f0, f1, f2, f3);
        c = corrector (y, h, f0, f1, f2, f3);
        y = c;

        f3 = f2;
        f2 = f1;
        f1 = f0;
        t++;
        f0 = function (t, p);
    } */
}