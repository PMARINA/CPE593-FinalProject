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

void predictor(double &initial_value, uint64_t &step, double &fn0, double &fn1, double &fn2, double &fn3, double &return_to){ //predictor: fn0 = fn, fn1 = fn-1, fn2 = fn-2, fn3 = fn-3
    return_to = (initial_value + ((step/24.0) * (-9.0*fn3 + 37.0*fn2 - 59.0*fn1 + 55.0*fn0)));
}

void corrector(double &initial_value, uint64_t &step, double &fn0, double &fn1, double &fn2, double &f1, double &return_to){ //corrector: fn0 = fn, fn1 = fn-1, fn2 = fn-2, f1 = fn+1
    return_to = (initial_value + ((step/24.0) * (fn2 - 5.0*fn1 + 19.0*fn0 + 9.0*f1)));
}

void predictor_corrector(double* initial_value, double* deriv_array, uint64_t &timestep, double &return_to){
//deriv_array contains fn-3, fn-2, fn-1, fn
//initial_value contains starting yn

//do acc first
//for acc: initial_value = last "real" value used for acc | deriv_array -> gives vel
//for vel: initial_value = last "real" value used for vel | deriv_array -> gives pos
for (int i = 0; i < 2; i++){ //loop 3 times for x,y,z
    double prediction = predictor (initial_value, timestep, deriv_array[i][0], deriv_array[i][1], deriv_array[i][2], deriv_array[i][3]);
    double correction = corrector (initial_value, timestep, deriv_array[i][0], deriv_array[i][1], deriv_array[i][2], deriv_array[i][3]);
    return_to = correction;
}



/*     
    double t = 0; //initial time
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
    } 
*/
}