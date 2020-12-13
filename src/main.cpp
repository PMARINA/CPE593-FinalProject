#include <iostream>

#include "obj/planetary_object.hh"
using std::cout;
using std::endl;
int main() {
  cout << "Starting" << endl;
  vector<Planetary_Object*>* planets = Planetary_Object::read_config();
  cout << "Finished" << endl;
  return 0;
}