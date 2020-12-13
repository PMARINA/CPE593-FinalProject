#define CONFIG_PATH "../src/data_process/Reformatted_Input.txt"
#include <cstdint>
#include <string>

using std::string;
using std::vector;

class Planetary_Object {
 public:
  static uint64_t timestamp;
  static double G;
  static double tolerance;
  static double timestep;
  uint64_t index;
  string name;
  double graphics_radius;
  double radius;
  double rot_rate;
  double obliquity_to_orbit;
  double mass;
  double* position;
  double* velocity;

 public:
  Planetary_Object(uint64_t index, string name, double graphics_radius,
                   double rot_rate, double obliquity_to_orbit, double mass,
                   double* position, double* velocity) {
    this->index = index;
    this->name = name;
    this->graphics_radius = graphics_radius;
    this->rot_rate = rot_rate;
    this->obliquity_to_orbit = obliquity_to_orbit;
    this->mass = mass;
    this->position = position;
    this->velocity = velocity;
  }
  static vector<Planetary_Object*>* read_config(string filepath = CONFIG_PATH);
  ~Planetary_Object() {
    delete[] position;
    delete[] velocity;
  }
};