#define CONFIG_PATH "../src/data_process/Reformatted_Input.txt"
#include <cstdint>
#include <string>
#include <vector>

#include "../res/FileReaderWriter.cpp"
#include "matrix.cpp"
using std::string;
using std::vector;
class Planetary_Object {
 public:
  static double timestamp;
  static double end_simulation_timestamp;
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
  Matrix* position;      // Only first time step is init
  Matrix* velocity;      // Only first time step is init
  Matrix* acceleration;  // going to be unitialized until done by main loop
                         // using RKF45
  static FileReaderWriter* frw;

 public:
  Planetary_Object(uint64_t index, string name, double graphics_radius,
                   double rot_rate, double obliquity_to_orbit, double mass,
                   Matrix* p, Matrix* v);
  static vector<Planetary_Object*>* read_config(string filepath = CONFIG_PATH);
  ~Planetary_Object() {
    delete[] position;
    delete[] velocity;
  }
  void predict_nth_order(uint64_t order_n);
  void correct_nth_order(uint64_t order_n);
  static void compute_accelerations(vector<Planetary_Object*>* planets, uint64_t timestep_number=0);
  static void dump_data(vector<Planetary_Object*>* planets);
  static void close();
  // void update_acceleration(vector<Planetary_Object*>* bodies);
};
