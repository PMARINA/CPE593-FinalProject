#include "planetary_object.hh"
#include <iostream>
#include "../math/gravity_equations_vectors.cpp"
#include "../math/rkf45_implement.cpp"
#include "../math/predictor_corrector.cpp"
#include "../math/constants.cpp"
#include "../res/FileReaderWriter.cpp"
using std::cin;
using std::cout;
using std::endl;
// #include ""
uint64_t Planetary_Object::timestamp = 0;
double Planetary_Object::G = 0;
double Planetary_Object::tolerance = 0;
double Planetary_Object::timestep = 0;
vector<Planetary_Object*>* Planetary_Object::read_config(string filepath) {
  FileReaderWriter frw(filepath);
  // cout << "Got frw" << endl;
  vector<Planetary_Object*>* a = new vector<Planetary_Object*>();
  // cout << "Vector made" << endl;
  frw.in_file >> Planetary_Object::timestamp;
  // cout << "timestamp" << endl;
  frw.in_file >> Planetary_Object::G;
  // cout << "G" << endl;
  frw.in_file >> Planetary_Object::tolerance;
  // cout << "tolerance" << endl;
  frw.in_file >> Planetary_Object::timestep;
  // cout << "timestep" << endl;
  // cin >> timestep;
  string name;
  uint64_t index;
  double graphics_radius;
  double radius;
  double rot_rate;
  double obliquity_to_orbit;
  double mass;
  frw.in_file >> index;
  while (!frw.in_file.eof()) {
    frw.in_file >> name;
    frw.in_file >> graphics_radius;
    frw.in_file >> radius;
    frw.in_file >> rot_rate;
    frw.in_file >> obliquity_to_orbit;
    frw.in_file >> mass;
    if (!frw.in_file.eof()) {
      double* position = new double[3];
      double* velocity = new double[3];
      for (int i = 0; i < num_dims; i++) {
        frw.in_file >> position[i];
      }
      for (int i = 0; i < num_dims; i++) {
        frw.in_file >> velocity[i];
      }
      a->emplace_back(new Planetary_Object(index, name, graphics_radius,
                                           rot_rate, obliquity_to_orbit, mass,
                                           position, velocity));
#if 0
      cout << "Read planet" << endl;
      cout << "Planet index: " << index << endl;
      cout << "Planet Name:  " << name << endl;
      cout << "Radius Graph: " << graphics_radius << endl;
      cout << "Radius:       " << radius << endl;
      cout << "Rot_rate:     " << rot_rate << endl;
      cout << "Obl Orb:      " << obliquity_to_orbit << endl;
      cout << "Mass:         " << mass << endl;
      cin >> index;
#endif
      frw.in_file >> index;
    }
  }
  return a;
}