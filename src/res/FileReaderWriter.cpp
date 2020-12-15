// Nicholas Dodd, Pridhvi Myneni, Joey Rupert
#define FILE_TO_COMPRESS "data/cantrbry/asyoulik.txt"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

using std::ifstream;
using std::ofstream;
using std::string;

// From https://stackoverflow.com/a/10376445
class my_ctype : public std::ctype<char> {
  mask my_table[table_size];

 public:
  my_ctype(size_t refs = 0) : std::ctype<char>(&my_table[0], false, refs) {
    std::copy_n(classic_table(), table_size, my_table);
    my_table['\n'] = (mask)space;
    my_table[','] = (mask)space;
  }
};

class FileReaderWriter {
 public:
  string in_file_name = "";
  string out_file_name = "";
  ifstream in_file;
  ofstream* out_file;

  FileReaderWriter(string input_file_name = "", string output_file_name = "") {
    if (input_file_name == "") {
      input_file_name = FILE_TO_COMPRESS;
    }
    if (output_file_name == "") {
      output_file_name = input_file_name + string("OUTPUT");
    }
    this->in_file_name = input_file_name;
    this->in_file = ifstream(input_file_name, std::ios::binary);
    this->out_file = new ofstream(output_file_name);
    std::locale x(std::locale::classic(), new my_ctype);
    in_file.imbue(x);
  }
  // Taken from https://stackoverflow.com/a/2602060
  string read_all() {
    string str;
    in_file.seekg(0, std::ios::end);
    str.reserve(in_file.tellg());
    in_file.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(in_file)),
               std::istreambuf_iterator<char>());
    return str;
  }
  // void write(string s) { out_file << s; }
  void flush_writer() { out_file->flush(); }
  ~FileReaderWriter() {
    in_file.close();
    out_file->close();
  }
};
// // An example of using the FileReaderWriter class.
// void copy_file() {
//   FileReaderWriter *frw = new FileReaderWriter();
//   string s = frw->read_all();
//   frw->write(s);
//   delete frw;
// }
