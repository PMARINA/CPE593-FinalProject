#define num_dims 3
#include <string>

using std::string;

class Planetary_Object {
 public:
  string name;
  double graphics_radius;
  double radius;
  double rot_rate;
  double obliquity_to_orbit;
  double mass;
  double* position;
  double* velocity;

 public:
  Planetary_Object(string name, double graphics_radius, double rot_rate,
                   double obliquity_to_orbit, double mass, double* position,
                   double* velocity) {
    this->name = name;
    this->graphics_radius = graphics_radius;
    this->rot_rate = rot_rate;
    this->obliquity_to_orbit = obliquity_to_orbit;
    this->mass = mass;
    this->position = position;
    this->velocity = velocity;
  }
  ~Planetary_Object() {
    delete[] position;
    delete[] velocity;
  }
};