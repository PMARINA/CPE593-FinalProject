#include <iostream>
#include <vector>
#include <string.h>

#include "obj/planetary_object.hh"
#include "math/gravity_equations_vectors.cpp"
using std::cout;
using std::endl;
using std::vector;

int main()
{
  // Note that this main is ONLY for running P-C, it may be beneficial to
  // rename this function to something else and call from main depending on
  // what simulation is being run.
  vector<Planetary_Object *> *planets = Planetary_Object::read_config();

  // TODO: RKF45 first 5 vel (only 4 required)
  while (true)
  {
     /**
      * 1. Predict( p(n+1) given velocity matrix in each planet)
      * 2. Use gravity equations to get p'' given p(n+1) for each planet
      * 3. use correction alg C'(n+1)
      * 4. Use correction alg C(n+1)
      */ 
    
    // Predict( p(n+1) given velocity matrix in each planet)
    for (int i = 0; i < planets->size(); i++)
    {
      planets->at(i)->predict_next_position();
    }
    
    //Use gravity equations to get p'' given p(n+1) for each planet

    // Store the accelerations calculated while summing in this array.
    double accelerations[planets->size()];
    // Calculation variables that will be set by retrieval functions...
    double our_coord, their_coord, temp_force;
    uint64_t zero = 0;
    for (uint64_t i = 0; i < num_dims; i++)
    {
      memset(accelerations, 0, sizeof(double)*planets->size());
      for(uint64_t j = 0; j<planets->size(); j++){
        planets->at(j)->position->retrieveAtPoint(i, zero, our_coord);
        for(uint64_t k = j+1; k<planets->size(); k++){
          planets->at(k)->position->retrieveAtPoint(i, zero, their_coord);
          gravitational_force_component(planets->at(j)->mass, planets->at(k)->mass, Planetary_Object::G, our_coord, their_coord, temp_force);
          accelerations[j] += temp_force/planets->at(j)->mass;
          accelerations[k] += temp_force/planets->at(k)->mass;
        }
        planets->at(j)->acceleration->stepForward();
        planets->at(j)->acceleration->setAtPoint(i, zero, accelerations[j]);
      }
      // planets->at(i)->update_acceleration(planets);
    }
    for (int i = 0; i < planets->size(); i++)
    {
      //use correction alg C'(n+1)
    }
    for (int i = 0; i < planets->size(); i++)
    {
      // Use correction alg C(n+1)
    }
    // increment timestamp by timestep
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