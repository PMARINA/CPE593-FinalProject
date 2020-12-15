#include <iostream>
#include <vector>
#include <string.h>

#include "obj/planetary_object.hh"
#include "math/rkf45_implement.cpp"

using std::cout;
using std::endl;
using std::vector;

void simulate_RKF_then_PC() {
  vector<Planetary_Object *> *planets = Planetary_Object::read_config();

  // TODO: RKF45 first 5 vel (only 4 required)
  Planetary_Object* temp;
  uint64_t index;
  uint64_t old_index;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < planets->size(); i++) {
      temp = planets->at(i);
      temp->velocity->stepForward();
      temp->acceleration->stepForward();
      temp->position->stepForward();
      for (int k = 0; i < num_dims; k++) {
        index = temp->velocity->get_offset(k, 0);
        old_index = temp->velocity->get_offset(k, 1);
        RKF45_integral_estimate_fifth_order(temp->timestep, temp->acceleration->data[index], 
          temp->velocity->data[old_index], temp->velocity->data[index]);
        RKF45_integral_estimate_fifth_order(temp->timestep, temp->velocity->data[index], 
          temp->position->data[old_index], temp->position->data[index]);
      }
    }
  }
  while (Planetary_Object::timestamp < Planetary_Object::end_simulation_timestamp)
  {
     /**
      * Overall Predictor Corrector Algorithm
      * 1. Predict( p(n+1) given velocity matrix in each planet)
      * 2. Use gravity equations to get p'' given p(n+1) for each planet
      * 3. use correction alg C'(n+1)
      * 4. Use correction alg C(n+1)
      */
    
    // Predict( p(n+1) given velocity matrix in each planet)
    for (int i = 0; i < planets->size(); i++)
    {
      planets->at(i)->predict_nth_order(0);
    }
    
    // Use gravity equations to get p'' given p(n+1) for each planet
    Planetary_Object::compute_accelerations(planets);

    for (int i = 0; i < planets->size(); i++)
    { 
      //use correction alg C'(n+1)
      planets->at(i)->correct_nth_order(1);
    }
    for (int i = 0; i < planets->size(); i++)
    {
      // Use correction alg C(n+1)
      planets->at(i)->correct_nth_order(0);
    }
    Planetary_Object::timestamp += Planetary_Object::timestep;
  }
};

void simulate_RKF_only() {
  vector<Planetary_Object *> *planets = Planetary_Object::read_config();
  Planetary_Object* temp;
  uint64_t index;
  uint64_t old_index;
  while (Planetary_Object::timestamp < Planetary_Object::end_simulation_timestamp) {
    for (int i = 0; i < planets->size(); i++) {
      temp = planets->at(i);
      temp->velocity->stepForward();
      temp->acceleration->stepForward();
      temp->position->stepForward();
      for (int k = 0; i < num_dims; k++) {
        index = temp->velocity->get_offset(k, 0);
        old_index = temp->velocity->get_offset(k, 1);
        RKF45_integral_estimate_fifth_order(temp->timestep, temp->acceleration->data[index], 
          temp->velocity->data[old_index], temp->velocity->data[index]);
        RKF45_integral_estimate_fifth_order(temp->timestep, temp->velocity->data[index], 
          temp->position->data[old_index], temp->position->data[index]);
      }
    }
    Planetary_Object::timestamp += Planetary_Object::timestep;
  }
};

int main() {
  if (WHICH_SIM == 0) {
    simulate_RKF_only();
  } else {
    simulate_RKF_then_PC();
  }
}

#if 0
int main() {
  cout << "Starting" << endl;
  vector<Planetary_Object*>* planets = Planetary_Object::read_config();
  cout << "Finished" << endl;
  return 0;
}
#endif