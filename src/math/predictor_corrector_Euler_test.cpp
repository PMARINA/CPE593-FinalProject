// this is not to be used in the final program because we are using RKF45
// the purpose is to familiarize with the concept of a predictor corrector in
// practice

// example equation: dy/dx = 4.4x^3 - 2y^2
#include <bits/stdc++.h>

#include <iostream>

using std::cout;
using std::endl;

double function(double x, double y) {
  double ans = (4.4 * x * x * x) - (2 * y * y);
  return ans;
}

double predict(double x, double y, double dt) {
  double y_predict = y + (dt * function(x, y));
  return y_predict;
}

double correct(double x, double y, double dt, double y_predict) {
  double y_correct = y_predict;
  double d = x;
  y_correct =
      y + ((dt * 0.5) * (function(y_predict, x += dt) + function(y, x)));
  return y_correct;
}

int main() {
  double x = 0, y = 0.4, dt = 0.1;  // dt is time step

  double y_predict = predict(x, y, dt);
  double y_correct = correct(x, y, dt, y_predict);
  cout << y_predict << endl;
  cout << y_correct << endl;

  return 0;
}
