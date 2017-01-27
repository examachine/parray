//
//
// C++ Implementation for module: Parallel_Arrays
//
// Description: 
//
//
// Author: exa
//
//

#include "Parallel_Arrays.hxx"

// namespace Parallel_Arrays
// {
begin_namespace(Parallel_Arrays);

  void initialize() {
    num_processors = MPI::COMM_WORLD.Get_size();
    processor_id = MPI::COMM_WORLD.Get_rank();
    node_str = "[node ";
    ostrstream str;
    str << processor_id;
    node_str += str.str();
    node_str += "] ";
    nlog << node_str << "Parallel Arrays initialized" << endl;
  }

  void finalize() {
    nlog << node_str << "Parallel Arrays finished" << endl;
  }

  int num_processors;
  int processor_id;
  string node_str;

  istream & pin = cin;
  ostream & pout = cout;

end_namespace();
// } // namespace_end
