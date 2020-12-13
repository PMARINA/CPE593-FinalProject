#include <iostream>
#include <vector>

#include "obj/planetary_object.hh"
using std::cout;
using std::endl;
using std::vector;

int main()
{
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
    for (int i = 0; i < planets->size(); i++)
    {
      // Predict( p(n+1) given velocity matrix in each planet)
    }
    for (int i = 0; i < planets->size(); i++)
    {
      //Use gravity equations to get p'' given p(n+1) for each planet
    }
    for (int i = 0; i < planets->size(); i++)
    {
      //use correction alg C'(n+1)
    }
    for (int i = 0; i < planets->size(); i++)
    {
      // Use correction alg C(n+1)
    }
  }
}

/*
int main() {
  cout << "Starting" << endl;
  vector<Planetary_Object*>* planets = Planetary_Object::read_config();
  cout << "Finished" << endl;
  return 0;
}
*/