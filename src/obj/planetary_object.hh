#define CONFIG_PATH "../src/data_process/Reformatted_Input.txt"
#include <cstdint>
#include <string>
#include <vector>
#include  "matrix.cpp"
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
  Matrix* position;     // Only first time step is init
  Matrix* velocity;     // Only first time step is init
  Matrix* acceleration; // going to be unitialized until done by main loop using RKF45

 public:
  Planetary_Object(uint64_t index, string name, double graphics_radius,
                   double rot_rate, double obliquity_to_orbit, double mass,
                   double* position, double* velocity);
  static vector<Planetary_Object*>* read_config(string filepath = CONFIG_PATH);
  ~Planetary_Object() {
    delete[] position;
    delete[] velocity;
  }
  void predict_next_position();
};