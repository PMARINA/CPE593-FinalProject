#include <iostream>
#include <vector>
#include <string.h>

#include "obj/planetary_object.hh"
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
    //TODO: increment timestamp by timestep
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