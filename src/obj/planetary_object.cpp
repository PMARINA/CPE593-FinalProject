#include "planetary_object.hh"

#include <iostream>
#include <vector>

#include "../math/gravity_equations_vectors.cpp"
// #include "../math/rkf45_implement.cpp"
#include "../math/constants.cpp"
#include "../math/predictor_corrector.cpp"
// #include "../res/FileReaderWriter.cpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

uint64_t Planetary_Object::timestamp = 0;
uint64_t Planetary_Object::end_simulation_timestamp = 0;
double Planetary_Object::G = 0;
double Planetary_Object::tolerance = 0;
uint64_t *dim_arr = new uint64_t[2];
uint64_t Planetary_Object::timestep = 0;
FileReaderWriter *Planetary_Object::frw = nullptr;

Planetary_Object::Planetary_Object(uint64_t index, string name,
                                   double graphics_radius, double rot_rate,
                                   double obliquity_to_orbit, double mass,
                                   double *position, double *velocity) {
  this->index = index;
  this->name = name;
  this->graphics_radius = graphics_radius;
  this->rot_rate = rot_rate;
  this->obliquity_to_orbit = obliquity_to_orbit;
  this->mass = mass;
  this->position = new Matrix();
  this->velocity = new Matrix();
  this->acceleration = new Matrix();
  // Assuming index 0 is the latest data. Index num_dims-1 is the stalest data.
  uint64_t fresh_data_index = 0;
  for (uint64_t i = 0; i < num_dims; i++) {
    // dims = x, y, z...
    // index = # timesteps into the past...
    this->position->setAtPoint(i, fresh_data_index, position[i]);
    this->velocity->setAtPoint(i, fresh_data_index, velocity[i]);
  }
}

// predictor(double &initial_value, uint64_t &step, double &fn0, double &fn1,
// double &fn2, double &fn3, double &return_to) corrector(double &initial_value,
// uint64_t &step, double &fn0, double &fn1, double &fn2, double &f1, double
// &return_to)
void Planetary_Object::compute_accelerations(
    vector<Planetary_Object *> *planets) {
  // Store the forces_aggregate calculated while summing in this array.
  double forces_aggregate[planets->size()];
  // Calculation variables that will be set by retrieval functions...
  double our_coord, their_coord, temp_force;
  uint64_t zero = 0;
  for (uint64_t i = 0; i < num_dims; i++) {
    memset(forces_aggregate, 0, sizeof(double) * planets->size());
    for (uint64_t j = 0; j < planets->size(); j++) {
      planets->at(j)->position->retrieveAtPoint(i, zero, our_coord);
      for (uint64_t k = j + 1; k < planets->size(); k++) {
        planets->at(k)->position->retrieveAtPoint(i, zero, their_coord);
        gravitational_force_component(planets->at(j)->mass,
                                      planets->at(k)->mass, Planetary_Object::G,
                                      our_coord, their_coord, temp_force);
        forces_aggregate[j] += temp_force;
        forces_aggregate[k] += temp_force;
      }
      planets->at(j)->acceleration->stepForward();
      forces_aggregate[j] /= planets->at(j)->mass;
      planets->at(j)->acceleration->setAtPoint(i, zero, forces_aggregate[j]);
    }
  }
}
void Planetary_Object::predict_nth_order(uint64_t order_n) {
  Matrix *reffed_order = nullptr;
  Matrix *next_order = nullptr;
  if (order_n == 0)
    reffed_order = position;
  else if (order_n == 1)
    reffed_order = velocity;
  else if (order_n == 2)
    reffed_order = acceleration;
  assert(reffed_order != nullptr);

  if (order_n == 0)
    next_order = velocity;
  else if (order_n == 1)
    next_order = acceleration;
  assert(next_order != nullptr);

  reffed_order->stepForward();
  // method needs 4 next_order_values
  double next_order_values[pc_num_derivs];
  for (uint64_t i = 0; i < num_dims; i++) {
    for (uint64_t j = 0; j < pc_num_derivs; j++)
      next_order->retrieveAtPoint(i, j, *(next_order_values + j));
    predictor(reffed_order->data[reffed_order->get_offset(i, 1)],
              Planetary_Object::timestep, next_order_values[0],
              next_order_values[1], next_order_values[2], next_order_values[3],
              reffed_order->data[reffed_order->get_offset(i, 0)]);
  }
}
void Planetary_Object::correct_nth_order(uint64_t order_n) {
  Matrix *reffed_order = nullptr;
  Matrix *next_order = nullptr;
  if (order_n == 0)
    reffed_order = position;
  else if (order_n == 1)
    reffed_order = velocity;
  else if (order_n == 2)
    reffed_order = acceleration;
  assert(reffed_order != nullptr);

  if (order_n == 0)
    next_order = velocity;
  else if (order_n == 1)
    next_order = acceleration;
  assert(next_order != nullptr);

  reffed_order->stepForward();
  // method needs 4 next_order_values
  double next_order_values[pc_num_derivs];
  for (uint64_t i = 0; i < num_dims; i++) {
    for (uint64_t j = 0; j < pc_num_derivs; j++)
      next_order->retrieveAtPoint(i, j, *(next_order_values + j));
    corrector(reffed_order->data[reffed_order->get_offset(i, 1)],
              Planetary_Object::timestep, next_order_values[1],
              next_order_values[2], next_order_values[3], next_order_values[0],
              reffed_order->data[reffed_order->get_offset(i, 0)]);
  }
}
vector<Planetary_Object *> *Planetary_Object::read_config(string filepath) {
  Planetary_Object::frw = new FileReaderWriter(filepath);
  // cout << "Got frw" << endl;
  vector<Planetary_Object *> *a = new vector<Planetary_Object *>();
  // cout << "Vector made" << endl;
  frw->in_file >> Planetary_Object::timestamp;
  frw->in_file >> Planetary_Object::end_simulation_timestamp;
  // cout << "timestamp" << endl;
  frw->in_file >> Planetary_Object::G;
  // cout << "G" << endl;
  frw->in_file >> Planetary_Object::tolerance;
  // cout << "tolerance" << endl;
  frw->in_file >> Planetary_Object::timestep;
  // cout << "timestep" << endl;
  // cin >> timestep;
  string name;
  uint64_t index;
  double graphics_radius;
  double radius;
  double rot_rate;
  double obliquity_to_orbit;
  double mass;
  frw->in_file >> index;
  while (!frw->in_file.eof()) {
    frw->in_file >> name;
    frw->in_file >> graphics_radius;
    frw->in_file >> radius;
    frw->in_file >> rot_rate;
    frw->in_file >> obliquity_to_orbit;
    frw->in_file >> mass;
    if (!frw->in_file.eof()) {
      double *position = new double[num_dims];
      double *velocity = new double[num_dims];
      for (int i = 0; i < num_dims; i++) {
        frw->in_file >> position[i];
        cout << position[i] << '\t';
      }
      cout << "\n";
      for (int i = 0; i < num_dims; i++) {
        frw->in_file >> velocity[i];
        cout << velocity[i] << '\t';
      }
      cout << "\n";
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
      frw->in_file >> index;
    }
  }
  return a;
}
void Planetary_Object::dump_data(vector<Planetary_Object *> *planets) {
  for (uint64_t i = 0; i < planets->size(); i++) {
    *(frw->out_file) << planets->at(i)->index << ": ";
    *(frw->out_file)
        << planets->at(i)
               ->position->data[planets->at(i)->position->get_offset(0, 0)]
        << " ";
    *(frw->out_file)
        << planets->at(i)
               ->position->data[planets->at(i)->position->get_offset(1, 0)]
        << " ";
    *(frw->out_file)
        << planets->at(i)
               ->position->data[planets->at(i)->position->get_offset(2, 0)]
        << "\n";
  }
  *(frw->out_file) << "\n";
  // *(frw->out_file)->flush();
}

void Planetary_Object::close() {
  Planetary_Object::frw->flush_writer();
  delete frw;
}