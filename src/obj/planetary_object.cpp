#include "planetary_object.hh"
#include <iostream>
#include <vector>

// #include "../math/gravity_equations_vectors.cpp"
// #include "../math/rkf45_implement.cpp"
#include "../math/predictor_corrector.cpp"
#include "../math/constants.cpp"
#include "../res/FileReaderWriter.cpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

uint64_t Planetary_Object::timestamp = 0;
double Planetary_Object::G = 0;
double Planetary_Object::tolerance = 0;
uint64_t *dim_arr = new uint64_t[2];
uint64_t Planetary_Object::timestep = 0;

Planetary_Object::Planetary_Object(uint64_t index, string name, double graphics_radius,
                   double rot_rate, double obliquity_to_orbit, double mass,
                   double* position, double* velocity)
{
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
  for (uint64_t i = 0; i < num_dims; i++)
  {
    // dims = x, y, z...
    // index = # timesteps into the past...
    this->position->setAtPoint(i, fresh_data_index, position[i]);
    this->velocity->setAtPoint(i, fresh_data_index, velocity[i]);
  }
}

//predictor(double &initial_value, uint64_t &step, double &fn0, double &fn1, double &fn2, double &fn3, double &return_to)
//corrector(double &initial_value, uint64_t &step, double &fn0, double &fn1, double &fn2, double &f1, double &return_to)
#if 0
Planetary_Object::update_acceleration(vector<Planetary_Object*>* bodies){
  // First step the time forward to avoid overwriting existing acceleration
  // data (unsure if need to maintain, but won't hurt to have)
  acceleration->stepForward();
  // Allocate temporary vars.
  double acceleration_this_dim, our_coord, their_coord, temp_acceleration;
  for(uint64_t i = 0; i<num_dims; i++){
    // assume 0 acceleration to start with...
    acceleration_this_dim = 0;
    // get the coordinate of position of the this object, as it's shared for
    // all calculations with other planets...
    this->position->retrieveAtPoint(i, 0, our_coord);
    for(uint64_t j = 0; j<bodies->size(); j++){
      if(this != bodies->at(j)){
        bodies->at(j)->position->retrieveAtPoint(i, 0, their_coord);
        gravitational_acceleration_component(this->mass, bodies->at(j)->mass, Planetary_Object::G, our_coord, their_coord, temp_acceleration);
        acceleration_this_dim = temp_acceleration;
      }
    }
    acceleration_this_dim += temp_acceleration;
    acceleration->setAtPoint(i,0, acceleration_this_dim);
  }
#if 0
  for(uint64_t i = 0; i<num_dims; i++){
    
    // assume 0 acceleration to start with...
    acceleration_this_dim = 0;
    // get the coordinate of position of the this object, as it's shared for
    // all calculations with other planets...
    this->position->retrieveAtPoint(i, 0, our_coord);
    for(uint64_t j = 0; j<bodies->size(); j++){
      if(this != bodies->at(j)){
        bodies->at(j)->position->retrieveAtPoint(i, 0, their_coord);
        gravitational_acceleration_component(this->mass, bodies->at(j)->mass, Planetary_Object::G, our_coord, their_coord, temp_acceleration);
        acceleration_this_dim = temp_acceleration;
      }
    }
    acceleration_this_dim += temp_acceleration;
    acceleration->setAtPoint(i,0, acceleration_this_dim);
  }
#endif
}
#endif

void Planetary_Object::predict_next_position(){
  position->stepForward();
  // method needs 4 velocities
  double velocities[pc_num_derivs];
  for (uint64_t i = 0; i < num_dims; i++)
  {
    for (uint64_t j = 0; j < pc_num_derivs; j++)
      velocity->retrieveAtPoint(i, j, *(velocities + j));
    predictor(position->data[position->get_offset(i, 1)], Planetary_Object::timestep, velocities[0], velocities[1], velocities[2], velocities[3], position->data[position->get_offset(i, 0)]);
  }
}

vector<Planetary_Object *> *Planetary_Object::read_config(string filepath)
{
  FileReaderWriter frw(filepath);
  // cout << "Got frw" << endl;
  vector<Planetary_Object *> *a = new vector<Planetary_Object *>();
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
  while (!frw.in_file.eof())
  {
    frw.in_file >> name;
    frw.in_file >> graphics_radius;
    frw.in_file >> radius;
    frw.in_file >> rot_rate;
    frw.in_file >> obliquity_to_orbit;
    frw.in_file >> mass;
    if (!frw.in_file.eof())
    {
      double *position = new double[num_dims];
      double *velocity = new double[num_dims];
      for (int i = 0; i < num_dims; i++)
      {
        frw.in_file >> position[i];
      }
      for (int i = 0; i < num_dims; i++)
      {
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